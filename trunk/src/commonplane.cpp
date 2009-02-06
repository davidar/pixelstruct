// Copyright (c) 2009 David Roberts <dvdr18@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "commonplane.h"

#include <cstdlib>
#include <ctime>

#include <algorithm>

using std::vector;

CommonPlane::CommonPlane() {
}

CommonPlane::CommonPlane(const Camera& c1, const Camera& c2, const VisiblePoint* points) {
	vector<CGAL_Point> cgal_points;
	for(vector<int>::const_iterator i = c1.visible_points().begin(); i != c1.visible_points().end(); i++)
		cgal_points.push_back(CGAL_Point(points[*i].x(), points[*i].y(), points[*i].z()));
	for(vector<int>::const_iterator i = c2.visible_points().begin(); i != c2.visible_points().end(); i++)
		cgal_points.push_back(CGAL_Point(points[*i].x(), points[*i].y(), points[*i].z()));
	
	ransac(cgal_points);
}

CommonPlane::CommonPlane(const vector<CGAL_Point>& cgal_points) {
	best_fit_plane(cgal_points);
}

CommonPlane::~CommonPlane() {
}

void CommonPlane::best_fit_plane(const vector<CGAL_Point>& cgal_points) {
	CGAL::Cartesian<double>::Plane_3 plane;
	CGAL_Point centroid;
	m_fitting_quality = linear_least_squares_fitting_3(cgal_points.begin(), cgal_points.end(), plane, centroid);
	m_a = plane.a(); m_b = plane.b(); m_c = plane.c(); m_d = -plane.d();
}

void CommonPlane::ransac(const vector<CGAL_Point>& cgal_points) {
	srand(time(NULL));
	
	const int num_points = cgal_points.size();
	
	const unsigned int MIN_POINTS = 5;
	const int MAX_ITERATIONS = 1000;
	const double ERROR_THRESHOLD = 0.1;
	const unsigned int REQD_POINTS = 0.2*num_points;
	
	double m_fitting_quality = 0.0;
	
	for(int i = 0; i < MAX_ITERATIONS; i++) {
		vector<CGAL_Point> maybe_inliers; // MIN_POINTS random elements from cgal_points
		while(maybe_inliers.size() < MIN_POINTS) {
			CGAL_Point p = cgal_points[rand() % num_points];
			if(find(maybe_inliers.begin(), maybe_inliers.end(), p) == maybe_inliers.end()) // unique element
				maybe_inliers.push_back(p);
		}
		
		CommonPlane maybe_model(maybe_inliers);
		vector<CGAL_Point> consensus_set = maybe_inliers;
		
		for(vector<CGAL_Point>::const_iterator p = cgal_points.begin(); p != cgal_points.end(); p++) {
			if(find(consensus_set.begin(), consensus_set.end(), *p) != consensus_set.end()) // *p is already in consensus_set
				continue;
			double error = maybe_model.shortest_sqdist(Point(p->x(), p->y(), p->z()));
			if(error < ERROR_THRESHOLD)
				consensus_set.push_back(*p);
		}
		
		if(consensus_set.size() >= REQD_POINTS) {
			CommonPlane better_model(consensus_set);
			double this_quality = better_model.fitting_quality();
			if(this_quality > m_fitting_quality) {
				m_fitting_quality = this_quality;
				m_a = better_model.a();
				m_b = better_model.b();
				m_c = better_model.c();
				m_d = better_model.d();
			}
		}
	}
}

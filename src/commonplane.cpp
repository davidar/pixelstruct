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

#include <CGAL/Cartesian.h>
#include <CGAL/linear_least_squares_fitting_3.h>
#include <set>

using std::set;

CommonPlane::CommonPlane() {
}

CommonPlane::CommonPlane(const Camera& c1, const Camera& c2, const VisiblePoint* points) {
	set<CGAL::Cartesian<double>::Point_3> cgal_points;
	for(set<int>::const_iterator i = c1.visible_points().begin(); i != c1.visible_points().end(); i++)
		cgal_points.insert(CGAL::Cartesian<double>::Point_3(points[*i].x(), points[*i].y(), points[*i].z()));
	for(set<int>::const_iterator i = c2.visible_points().begin(); i != c2.visible_points().end(); i++)
		cgal_points.insert(CGAL::Cartesian<double>::Point_3(points[*i].x(), points[*i].y(), points[*i].z()));
	
	CGAL::Cartesian<double>::Plane_3 plane;
	CGAL::Cartesian<double>::Point_3 centroid;
	m_fitting_quality = linear_least_squares_fitting_3(cgal_points.begin(), cgal_points.end(), plane, centroid);
	m_a = plane.a(); m_b = plane.b(); m_c = plane.c(); m_d = -plane.d();
}

CommonPlane::~CommonPlane() {
}

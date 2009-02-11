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

#include "triangulation.h"

using std::vector;
using std::cerr;
using std::endl;

Triangulation::Triangulation() {
}

Triangulation::Triangulation(const Camera& c, const VisiblePoint* points) {
	for(vector<int>::const_iterator i = c.visible_points().begin(); i != c.visible_points().end(); i++) {
		const Point& p = c.point_w2l(points[*i]);
		const double x = p.x();
		const double y = p.y();
		const double z = p.z();
		insert_point(x/z, y/z, z); // divide by z to correct for perspective
	}
}

Triangulation::~Triangulation() {
	m_dt.clear();
}

void Triangulation::insert_point(double x, double y, double z) {
// 	cerr << "[Triangulation::insert_point] Inserting point (" << x << "," << y << "," << z << ")" << endl;
	try {
		m_dt.insert(CGAL_Point(x, y, z));
	} catch(const std::logic_error& e) {
		// this seems to be (maybe) a bug in CGAL which throws an error when the same point is inserted twice,
		// even though the documentation says that the library handles such situations
		cerr << "[Triangulation::insert_point] caught std::logic_error from DelaunayTriangulation::insert" << endl;
	}
}

Point Triangulation::get_point(const face_iterator& f, int i) const {
	const DelaunayTriangulation::Point p = f->vertex(i)->point();
	return Point(p.x()*p.z(), p.y()*p.z(), p.z());
}

void Triangulation::add_image_corners(double maxx, double maxy) {
	double average_depth = 0;
	int num_points = 0;
	
	DelaunayTriangulation::Vertex_circulator vc = m_dt.incident_vertices(m_dt.infinite_vertex()), vc_init = vc;
	if(vc != 0) do { // traverse the convex hull
		average_depth += vc->point().z();
		num_points++;
	} while(++vc != vc_init);
	average_depth /= num_points;
	
	insert_point(+maxx,+maxy,average_depth);
	insert_point(-maxx,+maxy,average_depth);
	insert_point(-maxx,-maxy,average_depth);
	insert_point(+maxx,-maxy,average_depth);
}

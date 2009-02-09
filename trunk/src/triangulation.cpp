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

Triangulation::Triangulation(const Camera& c, const VisiblePoint* points) {
	for(vector<int>::const_iterator i = c.visible_points().begin(); i != c.visible_points().end(); i++) {
		const double x = points[*i].x();
		const double y = points[*i].y();
		const double z = points[*i].z();
		m_dt.insert(CGAL_Point(x/z, y/z, z));
	}
	
	// TODO:
	std::cerr << "Triangulation contains " << m_dt.number_of_vertices() << " vertices:" << std::endl;
	for(DelaunayTriangulation::Finite_faces_iterator f = m_dt.finite_faces_begin(); f != m_dt.finite_faces_end(); f++) {
		const DelaunayTriangulation::Point p0 = f->vertex(0)->point();
		const DelaunayTriangulation::Point p1 = f->vertex(1)->point();
		const DelaunayTriangulation::Point p2 = f->vertex(2)->point();
		std::cerr << "(" << p0.x() << "," << p0.y() << "," << p0.z() << ")"
		          << "(" << p1.x() << "," << p1.y() << "," << p1.z() << ")"
		          << "(" << p2.x() << "," << p2.y() << "," << p2.z() << ")" << std::endl;
	}
}

Triangulation::~Triangulation() {
}

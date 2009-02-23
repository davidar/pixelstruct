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

#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "camera.h"
#include "visiblepoint.h"
#include "commonplane.h"

#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_euclidean_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>

class Triangulation {
	public:
	typedef CGAL::Exact_predicates_inexact_constructions_kernel::Point_3 CGAL_Point;
	typedef CGAL::Delaunay_triangulation_2<CGAL::Triangulation_euclidean_traits_xy_3<
		CGAL::Exact_predicates_inexact_constructions_kernel> > DelaunayTriangulation;
	
	Triangulation();
	Triangulation(const Camera&, const VisiblePoint*);
	~Triangulation();
	
	typedef DelaunayTriangulation::Finite_faces_iterator face_iterator;
	face_iterator faces_begin() const {return m_dt.finite_faces_begin();}
	face_iterator faces_end() const   {return m_dt.finite_faces_end();}
	
	int num_vertices() const {return m_dt.number_of_vertices();}
	
	void insert_point(double x, double y, double z);
	Point get_point(const face_iterator& f, int i) const;
	void add_image_corners(double maxx, double maxy);
	
	private:
	DelaunayTriangulation m_dt;
};

#endif

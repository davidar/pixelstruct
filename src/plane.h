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

#ifndef PLANE_H
#define PLANE_H

#include "point.h"

class Plane {
	public:
	Plane();
	Plane(double, double, double, double);
	~Plane();
	
	Plane transform(const double*);
	double distance(const Point&);
	double shortest_sqdist(const Point&);
	
	double a() {return m_a;}
	double b() {return m_b;}
	double c() {return m_c;}
	double d() {return m_d;}
	
	protected:
	double m_a, m_b, m_c, m_d; // m_a*x + m_b*y + m_c*z = m_d
};

#endif

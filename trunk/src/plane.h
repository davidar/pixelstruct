// Copyright (c) 2009 David Roberts <d@vidr.cc>
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
    
    Plane transform(const double*) const;
    double distance(const Point&) const;
    double shortest_sqdist(const Point&) const;
    
    double a() const {return m_a;}
    double b() const {return m_b;}
    double c() const {return m_c;}
    double d() const {return m_d;}
    
    bool is_valid() const {return !(m_a == 0 && m_b == 0 && m_c == 0);}
    
    protected:
    double m_a, m_b, m_c, m_d; // m_a*x + m_b*y + m_c*z = m_d
};

#endif

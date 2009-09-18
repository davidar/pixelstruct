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

#include "plane.h"

Plane::Plane() {
}

Plane::Plane(double a, double b, double c, double d)
    : m_a(a), m_b(b), m_c(c), m_d(d) {
}

Plane::~Plane() {
}

Plane Plane::transform(const double* m) const {
    return Plane(
        m_a*m[ 0],
        m_b*m[ 5],
        m_c*m[10],
        m_d - m_a*m[12] - m_b*m[13] - m_c*m[14]
    );
}

double Plane::distance(const Point& p) const {
    /// For a line that begins at the origin, passes through Point p, and ends
    /// at this Plane. The length of the line is equal to distance(p) times the
    /// distance between p and the origin.
    return m_d / (m_a*p.x() + m_b*p.y() + m_c*p.z());
}

double Plane::shortest_sqdist(const Point& p) const {
    /// square of shortest distance between this Plane and Point p
    double n = m_a*p.x() + m_b*p.y() + m_c*p.z() - m_d;
    return (n*n) / (m_a*m_a + m_b*m_b + m_c*m_c);
}

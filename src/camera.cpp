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

#include "camera.h"

const double IDENTITY_MATRIX_4[16] = {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};
const Camera Camera::Identity(-1, 1.0, 1.0, 1.0, IDENTITY_MATRIX_4);

Camera::Camera() {
}

Camera::Camera(const int index, const double f, const double k1,
               const double k2, const double* m1)
    : m_index(index), m_f(f), m_k1(k1), m_k2(k2), m_matrix_w2l(m1) {
    m_matrix_l2w = new double[16];
    double* m2 = m_matrix_l2w;
    
    /** 
     * m2 is the inverse of m1
     * therefore the rotation block of m2 is equal to the transpose of its
     * counterpart in m1, and the translation block of m2 is equal to the
     * negative of the rotation block of m2 multiplied by the translation block
     * of m1 the translation block of m2 is equal to the coordinates of the
     * camera in 3D space
     */
    m2[0] = m1[0]; m2[4] = m1[1]; m2[ 8] = m1[ 2]; m2[12] =
        -(m2[0]*m1[12] + m2[4]*m1[13] + m2[ 8]*m1[14]);
    m2[1] = m1[4]; m2[5] = m1[5]; m2[ 9] = m1[ 6]; m2[13] =
        -(m2[1]*m1[12] + m2[5]*m1[13] + m2[ 9]*m1[14]);
    m2[2] = m1[8]; m2[6] = m1[9]; m2[10] = m1[10]; m2[14] =
        -(m2[2]*m1[12] + m2[6]*m1[13] + m2[10]*m1[14]);
    m2[3] = 0.0;   m2[7] = 0.0;   m2[11] = 0.0;    m2[15] = 1.0;
}


Camera::~Camera() {
}

Point Camera::point_transform(const Point p, const double* m) const {
    double x, y, z;
    
    x = m[0]*p.x() + m[4]*p.y() + m[ 8]*p.z() + m[12];
    y = m[1]*p.x() + m[5]*p.y() + m[ 9]*p.z() + m[13];
    z = m[2]*p.x() + m[6]*p.y() + m[10]*p.z() + m[14];
    
    return Point(x, y, z);
}

void Camera::add_visible_point(Point p) {
    m_visible_points.push_back(p);
}

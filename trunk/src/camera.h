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

#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"

#include <vector>

class Camera {
    public:
    Camera();
    Camera(const int, const double, const double, const double, const double*);
    ~Camera();
    
    void add_visible_point(Point);
    
    int index() const {return m_index;}
    const double* matrix_w2l() const {return m_matrix_w2l;}
    const double* matrix_l2w() const {return m_matrix_l2w;}
    double focal_length() const {return m_f;}
    Point unit_x() const { //  first column of m_matrix_l2w
        return Point(m_matrix_l2w[ 0], m_matrix_l2w[ 1], m_matrix_l2w[ 2]);}
    Point unit_y() const { // second column of m_matrix_l2w
        return Point(m_matrix_l2w[ 4], m_matrix_l2w[ 5], m_matrix_l2w[ 6]);}
    Point unit_z() const { //  third column of m_matrix_l2w
        return Point(m_matrix_l2w[ 8], m_matrix_l2w[ 9], m_matrix_l2w[10]);}
    Point pos() const { // fourth column of m_matrix_l2w
        return Point(m_matrix_l2w[12], m_matrix_l2w[13], m_matrix_l2w[14]);}
    Point point_w2l(const Point p) const {
        return point_transform(p, m_matrix_w2l);}
    Point point_l2w(const Point p) const {
        return point_transform(p, m_matrix_l2w);}
    const std::vector<Point> &visible_points() const {return m_visible_points;}
    
    static const Camera Identity;
    
    protected:
    Point point_transform(const Point, const double*) const;
    
    private:
    int m_index;
    double m_f; // focal length
    double m_k1, m_k2; // radial distortion coefficients
    // transformation matrix: world coords to local coords
    // (for transforming the view)
    const double* m_matrix_w2l;
    // transformation matrix: local coords to world coords
    // (for transforming objects)
    double* m_matrix_l2w;
    std::vector<Point> m_visible_points;
};

#endif

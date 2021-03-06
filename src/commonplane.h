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

#ifndef COMMONPLANE_H
#define COMMONPLANE_H

#include "plane.h"

#include "camera.h"
#include "visiblepoint.h"

#include <vector>

#include <CGAL/Cartesian.h>
#include <CGAL/linear_least_squares_fitting_3.h>

class CommonPlane : public Plane {
    public:
    typedef CGAL::Cartesian<double>::Point_3 CGAL_Point;
    
    CommonPlane();
    CommonPlane(const Camera&, const Camera&);
    CommonPlane(const std::vector<CGAL_Point>&);
    ~CommonPlane();
    
    double fitting_quality() {return m_fitting_quality;}
    
    protected:
    void best_fit_plane(const std::vector<CGAL_Point>&);
    void ransac(std::vector<CGAL_Point>);
    
    private:
    double m_fitting_quality;
};

#endif

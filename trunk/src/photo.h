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

#ifndef PHOTO_H
#define PHOTO_H

#include <QGLWidget>

#include "triangulation.h"

class Photo {
    public:
    Photo(const int, const int, const Camera *);
    
    void paintSimple() const;
    void paintPlane(const Plane &) const;
    void paintTriangulation() const;
    
    const Camera *camera() const {return m_camera;}
    
    GLuint texture;
    double opacity;
    
    private:
    const int m_width, m_height;
    const Camera *m_camera;
    Triangulation m_triangulation;
};

#endif

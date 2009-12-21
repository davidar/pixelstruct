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

#include "photo.h"

using std::cerr;
using std::endl;

Photo::Photo(const int width, const int height, const Camera *camera)
    : m_width(width), m_height(height), m_camera(camera) {
    cerr << "Generating triangulation" << endl;
    m_triangulation = Triangulation(*m_camera);
    m_triangulation.add_image_corners(
        0.5 * m_width  / m_camera->focal_length(),
        0.5 * m_height / m_camera->focal_length());
}

void Photo::paintSimple() const {
    double maxx = 0.5 * m_width  / m_camera->focal_length();
    double maxy = 0.5 * m_height / m_camera->focal_length();
    glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex3d(-maxx,-maxy,-1.0);
        glTexCoord2d(1.0,0.0); glVertex3d(+maxx,-maxy,-1.0);
        glTexCoord2d(1.0,1.0); glVertex3d(+maxx,+maxy,-1.0);
        glTexCoord2d(0.0,1.0); glVertex3d(-maxx,+maxy,-1.0);
    glEnd();
}

void Photo::paintPlane(const Plane &local_plane) const {
    double maxx = 0.5 * m_width  / m_camera->focal_length();
    double maxy = 0.5 * m_height / m_camera->focal_length();
    double d0 = local_plane.distance(Point(-maxx,-maxy,-1.0));
    double d1 = local_plane.distance(Point(+maxx,-maxy,-1.0));
    double d2 = local_plane.distance(Point(+maxx,+maxy,-1.0));
    double d3 = local_plane.distance(Point(-maxx,+maxy,-1.0));
    
    if(!local_plane.is_valid() ||
       d0 <= 0.0 || d1 <= 0.0 || d2 <= 0.0 || d3 <= 0.0) {
        // if part or all of the image projected onto the common plane lies
        // behind the camera, then revert to simple image plane
        paintSimple();
        return;
    }
    
    glBegin(GL_QUADS);
        glTexCoord4d(0.0,0.0,0.0,d0); glVertex3d(d0*-maxx,d0*-maxy,-d0);
        glTexCoord4d( d1,0.0,0.0,d1); glVertex3d(d1*+maxx,d1*-maxy,-d1);
        glTexCoord4d( d2, d2,0.0,d2); glVertex3d(d2*+maxx,d2*+maxy,-d2);
        glTexCoord4d(0.0, d3,0.0,d3); glVertex3d(d3*-maxx,d3*+maxy,-d3);
    glEnd();
}

void Photo::paintTriangulation() const {
    double xrange = m_width  / m_camera->focal_length();
    double yrange = m_height / m_camera->focal_length();
    glBegin(GL_TRIANGLES);
    for(Triangulation::face_iterator f = m_triangulation.faces_begin();
        f != m_triangulation.faces_end(); f++) {
        for(int i = 0; i < 3; i++) {
            Point p = m_triangulation.get_point(f,i);
            double d = p.dist();
            double tx = (p.x() / -p.z()) / xrange + 0.5;
            double ty = (p.y() / -p.z()) / yrange + 0.5;
            glTexCoord4d(tx*d,ty*d,0.0,d);
            glVertex3d(p.x(), p.y(), p.z());
        }
    }
    glEnd();
}

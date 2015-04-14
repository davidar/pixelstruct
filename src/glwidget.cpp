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

#include "glwidget.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cfloat>
#include <cmath>
#include <GL/glu.h>

#include <QDebug>
#include <QKeyEvent>
#include <QImage>

using std::cerr;
using std::endl;

#define CAPPED_TEXTURE_SIZE 2048

GLWidget::GLWidget(BundleParser* parser, ImageList* imagelist, QWidget* parent)
    : QGLWidget(parent), m_parser(parser), m_imagelist(imagelist),
      m_transmode(0), m_cur_image(NULL), m_prev_image(NULL) {
    setFocusPolicy(Qt::ClickFocus);
}

GLWidget::~GLWidget() {
}

void GLWidget::setTransMode(int transmode) {
    m_transmode = transmode;
    updateGL();
}

void GLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    
    m_pointcloud = glGenLists(1);
    glLoadIdentity();
    glNewList(m_pointcloud, GL_COMPILE);
    glBegin(GL_POINTS);
        for(int i = 0; i < m_parser->num_points(); i++) {
            const VisiblePoint& p = m_parser->points()[i];
            qglColor(p.col());
            glVertex3d(p.x(), p.y(), p.z());
        }
    glEnd();
    glEndList();
    
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
    cerr << "GL_MAX_TEXTURE_SIZE = " << texSize << endl;
    
    handleGLError(__LINE__);
}

void GLWidget::paintImage(const Photo *image) {
    glPushMatrix();
    glMultMatrixd(image->camera()->matrix_l2w());
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image->texture);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND); glDisable(GL_DEPTH_TEST);
    glColor4f(1.0,1.0,1.0,image->opacity);
    
    switch(m_transmode) {
        case 0: image->paintSimple(); break;
        case 1: image->paintPlane(m_common_plane.transform(
                    image->camera()->matrix_w2l()));
                break;
        case 2: image->paintTriangulation(); break;
        default: image->paintSimple(); break;
    }
    
    glDisable(GL_BLEND); glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glCallList(m_pointcloud);
    
    handleGLError(__LINE__);
    
    if(m_transmode != -1) {
        if(m_prev_image && m_prev_image->opacity > 0.01)
            paintImage(m_prev_image);
        if(m_cur_image && m_cur_image->opacity > 0.01)
            paintImage(m_cur_image);
    }
    
    handleGLError(__LINE__);
}

void GLWidget::resizeGL (int width, int height) {
    cerr << "[GLWidget::resizeGL] Resizing GLWidget to " << width << "x"
         << height << endl;
    
    if(height == 0) height = 1; // prevent division by 0
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height); // entire window
    gluPerspective(60.0, (double)width/height, 1e-20, 1e5);
    
    glMatrixMode(GL_MODELVIEW);
    
    //cerr << "[GLWidget::resizeGL] end" << endl;
}

void GLWidget::keyPressEvent (QKeyEvent* event) {
    //cerr << "[GLWidget::keyPressEvent] begin" << endl;
    
    switch (event->key()) {
        case Qt::Key_Right:
            gotoDirection(0); break;
        case Qt::Key_Up:
            gotoDirection(1); break;
        case Qt::Key_PageDown:
            gotoDirection(2); break;
        case Qt::Key_Left:
            gotoDirection(3); break;
        case Qt::Key_Down:
            gotoDirection(4); break;
        case Qt::Key_PageUp:
            gotoDirection(5); break;
        case Qt::Key_Return:
            gotoNextCamera(); break;
    }
    
    //cerr << "[GLWidget::keyPressEvent] end" << endl;
}

void GLWidget::keyReleaseEvent (QKeyEvent* event) {}

void GLWidget::mousePressEvent (QMouseEvent* event) {}

void GLWidget::mouseMoveEvent (QMouseEvent* event) {}

void GLWidget::mouseReleaseEvent (QMouseEvent* event) {}

void GLWidget::focusInEvent(QFocusEvent* event) {
    //cerr << "[GLWidget::focusInEvent] begin" << endl;
    if(m_cur_image == NULL)
        gotoNextCamera();
    //cerr << "[GLWidget::focusInEvent] end" << endl;
}

void GLWidget::focusOutEvent(QFocusEvent* event) {}

void GLWidget::gotoCamera(int target_camera) {
    cerr << "[GLWidget::gotoCamera] Moving to camera "
         << target_camera << endl;
    
    const Camera& c1 = m_cur_image == NULL
                       ? Camera::Identity
                       : *(m_cur_image->camera());
    const Camera& c2 = m_parser->cameras()[target_camera];
    
    //glDeleteTextures(1, &(m_prev_image.texture));
    if(m_prev_image) {
        deleteTexture(m_prev_image->texture);
        delete m_prev_image;
    }
    m_prev_image = m_cur_image;
    
    QImage image = m_imagelist->loadImage(target_camera);
    m_cur_image = new Photo(image.width(), image.height(), &c2);
    int size = texSize < CAPPED_TEXTURE_SIZE ? texSize : CAPPED_TEXTURE_SIZE;
    image = image.scaled(size, size);
    //image = convertToGLFormat(image);
    //glGenTextures(1, &(m_cur_image.texture));
    //glBindTexture(GL_TEXTURE_2D, m_cur_image.texture);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(),
    //             0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    m_cur_image->texture = bindTexture(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    handleGLError(__LINE__);
    
    if(m_transmode == 1) {
        cerr << "[GLWidget::gotoCamera] Generating common plane" << endl;
        m_common_plane = CommonPlane(c1, c2);
    }
    
    for(double i = 0.0; i < 1.001; i += 0.1) {
        usleep(1000000/25); // sleep for 1 frame @ 25fps
        
        if(m_prev_image) m_prev_image->opacity = 1.0 - i;
        m_cur_image->opacity = i;
        
        double cx =     c1.pos().x()*(1.0-i) +     c2.pos().x()*i;
        double cy =     c1.pos().y()*(1.0-i) +     c2.pos().y()*i;
        double cz =     c1.pos().z()*(1.0-i) +     c2.pos().z()*i;
        double dx = -c1.unit_z().x()*(1.0-i) + -c2.unit_z().x()*i;
        double dy = -c1.unit_z().y()*(1.0-i) + -c2.unit_z().y()*i;
        double dz = -c1.unit_z().z()*(1.0-i) + -c2.unit_z().z()*i;
        double ux =  c1.unit_y().x()*(1.0-i) +  c2.unit_y().x()*i;
        double uy =  c1.unit_y().y()*(1.0-i) +  c2.unit_y().y()*i;
        double uz =  c1.unit_y().z()*(1.0-i) +  c2.unit_y().z()*i;
        
        glLoadIdentity();
        gluLookAt(cx,cy,cz, cx+dx,cy+dy,cz+dz, ux,uy,uz);
        updateGL();
    }
    
    //cerr << "[GLWidget::gotoCamera] end" << endl;
}

void GLWidget::gotoNextCamera() {
    cerr << "[GLWidget::gotoNextCamera] Moving to next camera" << endl;
    int target_camera = m_cur_image == NULL
                        ? 0 : m_cur_image->camera()->index() + 1;
    while(m_parser->cameras()[target_camera].focal_length() == 0.0)
        target_camera++;
    if(target_camera >= m_parser->num_cameras()) target_camera = 0;
    while(m_parser->cameras()[target_camera].focal_length() == 0.0)
        target_camera++;
    gotoCamera(target_camera);
    //cerr << "[GLWidget::gotoNextCamera] end" << endl;
}

void GLWidget::gotoDirection(int target_direction) {
    cerr << "[GLWidget::gotoDirection] Moving in direction "
         << target_direction << endl;
    
    double closest_dist = DBL_MAX;
    int closest_camera = -1;
    const Camera& c1 = *(m_cur_image->camera());
    
    for(int candidate_camera = 0; candidate_camera < m_parser->num_cameras();
        candidate_camera++) {
        if(m_cur_image == NULL) break;
        if(candidate_camera == m_cur_image->camera()->index()) continue;
        
        const Camera& c2 = m_parser->cameras()[candidate_camera];
        if(c2.focal_length() == 0.0) continue;
        
        Point relative_pos = c1.point_w2l(c2.pos());
        
        double dist = DBL_MAX;
        if(relative_pos.x() > 0.0 && target_direction == 0)
            dist = relative_pos.y()*relative_pos.y()
                 + relative_pos.z()*relative_pos.z();
        if(relative_pos.y() > 0.0 && target_direction == 1)
            dist = relative_pos.x()*relative_pos.x()
                 + relative_pos.z()*relative_pos.z();
        if(relative_pos.z() > 0.0 && target_direction == 2)
            dist = relative_pos.x()*relative_pos.x()
                 + relative_pos.y()*relative_pos.y();
        if(relative_pos.x() < 0.0 && target_direction == 3)
            dist = relative_pos.y()*relative_pos.y()
                 + relative_pos.z()*relative_pos.z();
        if(relative_pos.y() < 0.0 && target_direction == 4)
            dist = relative_pos.x()*relative_pos.x()
                 + relative_pos.z()*relative_pos.z();
        if(relative_pos.z() < 0.0 && target_direction == 5)
            dist = relative_pos.x()*relative_pos.x()
                 + relative_pos.y()*relative_pos.y();
        
        if(closest_dist > dist) {
            closest_dist = dist;
            closest_camera = candidate_camera;
        }
    }
    
    if(closest_camera >= 0) gotoCamera(closest_camera);
    
    //cerr << "[GLWidget::gotoDirection] end" << endl;
}

void GLWidget::handleGLError(int line) {
    GLenum errCode = glGetError();
    if(errCode != GL_NO_ERROR)
        cerr << "OpenGL Error (line " << line << "): "
             << errCode << ": " << gluErrorString(errCode) << endl;
}

#include "glwidget.moc"

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

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "bundleparser.h"
#include "imagelist.h"
#include "commonplane.h"
#include "triangulation.h"

class GLWidget : public QGLWidget {
    Q_OBJECT
    
    struct GLWImage {
        int width, height;
        GLuint texture;
        double opacity;
        int camera;
        Triangulation triangulation;
    };
    
    public:
    GLWidget(BundleParser*, ImageList*, QWidget* = 0);
    ~GLWidget();
    
    QSize minimumSizeHint() const {return QSize(50, 50);}
    
    void setTransMode(int transmode);
    
    protected:
    void initializeGL();
    void paintImageTriangulation(const GLWImage&);
    void paintImageCommonPlane(const GLWImage&);
    void paintImageSimple(const GLWImage&);
    void paintImage(const GLWImage&);
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void gotoCamera(int);
    void gotoNextCamera();
    void gotoDirection(int);
    void reloadTexture();
    
    private:
    void handleGLError(int);
    
    GLuint m_pointcloud;
    BundleParser* m_parser;
    ImageList* m_imagelist;
    GLWImage m_cur_image;
    GLWImage m_prev_image;
    CommonPlane m_common_plane;
    int m_transmode;
};

#endif

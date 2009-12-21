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
#include "photo.h"

class GLWidget : public QGLWidget {
    Q_OBJECT
    
    public:
    GLWidget(BundleParser*, ImageList*, QWidget* = 0);
    ~GLWidget();
    
    QSize minimumSizeHint() const {return QSize(50, 50);}
    
    void setTransMode(int transmode);
    
    protected:
    void initializeGL();
    void paintImage(const Photo *);
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
    
    private:
    void handleGLError(int);
    
    GLuint m_pointcloud;
    BundleParser* m_parser;
    ImageList* m_imagelist;
    int m_transmode;
    Photo *m_cur_image;
    Photo *m_prev_image;
    CommonPlane m_common_plane;
    GLint texSize;
};

#endif

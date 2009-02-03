// Copyright (c) 2009 David Roberts <dvdr18@gmail.com>
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

#include <QDebug>
#include <QKeyEvent>
#include <QImage>

using std::cerr;
using std::endl;

GLWidget::GLWidget(BundleParser* parser, ImageList* imagelist, QWidget* parent)
	: QGLWidget(parent), m_parser(parser), m_imagelist(imagelist) {
	setFocusPolicy(Qt::ClickFocus);
	m_cur_image.camera = -1;
	m_prev_image.camera = -1;
}

GLWidget::~GLWidget() {
}

void GLWidget::initializeGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
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
}

void GLWidget::paintImagePlane(GLImage image) {
	glPushMatrix();
	
	if(image.camera > -1) {
		const Camera& c = m_parser->cameras()[image.camera];
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, image.texture);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND); glDisable(GL_DEPTH_TEST);
		
		glMultMatrixd(c.matrix_l2w());
		
		glColor4f(1.0,1.0,1.0,image.opacity);
		double maxx = 0.5*image.width, maxy = 0.5*image.height;
		
		glBegin(GL_QUADS);
			glTexCoord2d(0.0,0.0); glVertex3d(-maxx/c.focal_length(),-maxy/c.focal_length(),-1.0);
			glTexCoord2d(1.0,0.0); glVertex3d(+maxx/c.focal_length(),-maxy/c.focal_length(),-1.0);
			glTexCoord2d(1.0,1.0); glVertex3d(+maxx/c.focal_length(),+maxy/c.focal_length(),-1.0);
			glTexCoord2d(0.0,1.0); glVertex3d(-maxx/c.focal_length(),+maxy/c.focal_length(),-1.0);
		glEnd();
		
		glDisable(GL_BLEND); glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
	}
	
	glPopMatrix();
}

void GLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glCallList(m_pointcloud);
	
	paintImagePlane(m_cur_image);
	paintImagePlane(m_prev_image);
}

void GLWidget::resizeGL (int width, int height) {
	if(height == 0) height = 1; // prevent division by 0
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height); // entire window
	gluPerspective(60.0, float(width) / height, 0.001, 1000.0);
	
	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::keyPressEvent (QKeyEvent* event) {
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
}

void GLWidget::keyReleaseEvent (QKeyEvent* event) {}

void GLWidget::mousePressEvent (QMouseEvent* event) {}

void GLWidget::mouseMoveEvent (QMouseEvent* event) {}

void GLWidget::mouseReleaseEvent (QMouseEvent* event) {}

void GLWidget::focusInEvent(QFocusEvent* event) {
	if(m_cur_image.camera == -1)
		gotoNextCamera();
}

void GLWidget::focusOutEvent(QFocusEvent* event) {}

void GLWidget::gotoCamera(int target_camera) {
	cerr << "Moving to camera " << target_camera << endl;
	
	const Camera& c1 = m_cur_image.camera < 0 ? Camera::Identity : m_parser->cameras()[m_cur_image.camera];
	const Camera& c2 = m_parser->cameras()[target_camera];
	
	glDeleteTextures(1, &(m_prev_image.texture));
	m_prev_image = m_cur_image;
	m_cur_image.camera = target_camera;
	reloadTexture();
	
	for(double i = 0.0; i < 1.001; i += 0.1) {
		usleep(1000000/25); // sleep for 1 frame @ 25fps
		
		m_prev_image.opacity = 1.0 - i;
		m_cur_image.opacity = i;
		
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
}

void GLWidget::gotoNextCamera() {
	int target_camera = m_cur_image.camera + 1;
	while(m_parser->cameras()[target_camera].focal_length() == 0.0) target_camera++;
	if(target_camera >= m_parser->num_cameras()) target_camera = 0;
	while(m_parser->cameras()[target_camera].focal_length() == 0.0) target_camera++;
	gotoCamera(target_camera);
}

void GLWidget::gotoDirection(int target_direction) {
	double closest_dist = DBL_MAX;
	int closest_camera = -1;
	const Camera& c1 = m_parser->cameras()[m_cur_image.camera];
	
	for(int candidate_camera = 0; candidate_camera < m_parser->num_cameras(); candidate_camera++) { // TODO FIXME
		if(m_cur_image.camera < 0) break;
		if(candidate_camera == m_cur_image.camera) continue;
		
		const Camera& c2 = m_parser->cameras()[candidate_camera];
		if(c2.focal_length() == 0.0) continue;
		
		Point relative_pos = c1.point_w2l(c2.pos());
		
		double dist = DBL_MAX;
		if(relative_pos.x() > 0.0 && target_direction == 0) dist = relative_pos.y()*relative_pos.y() + relative_pos.z()*relative_pos.z();
		if(relative_pos.y() > 0.0 && target_direction == 1) dist = relative_pos.x()*relative_pos.x() + relative_pos.z()*relative_pos.z();
		if(relative_pos.z() > 0.0 && target_direction == 2) dist = relative_pos.x()*relative_pos.x() + relative_pos.y()*relative_pos.y();
		if(relative_pos.x() < 0.0 && target_direction == 3) dist = relative_pos.y()*relative_pos.y() + relative_pos.z()*relative_pos.z();
		if(relative_pos.y() < 0.0 && target_direction == 4) dist = relative_pos.x()*relative_pos.x() + relative_pos.z()*relative_pos.z();
		if(relative_pos.z() < 0.0 && target_direction == 5) dist = relative_pos.x()*relative_pos.x() + relative_pos.y()*relative_pos.y();
		
		if(closest_dist > dist) {
			closest_dist = dist;
			closest_camera = candidate_camera;
		}
	}
	
	if(closest_camera >= 0) gotoCamera(closest_camera);
}

void GLWidget::reloadTexture() {
	const QImage image = convertToGLFormat(m_imagelist->loadImage(m_cur_image.camera));
	m_cur_image.width = image.width();
	m_cur_image.height = image.height();
	glGenTextures(1, &(m_cur_image.texture));
	glBindTexture(GL_TEXTURE_2D, m_cur_image.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

#include "glwidget.moc"

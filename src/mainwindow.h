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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "glwidget.h"
#include "bundleparser.h"
#include "imagelist.h"

class MainWindow : public QMainWindow {
	Q_OBJECT
	
	public:
	MainWindow();
	~MainWindow();
	
	QSize minimumSizeHint() const {return QSize(50, 50);}
	QSize sizeHint() const {return QSize(800, 600);}
	
	protected:
	void createActions();
	void createMenus();
	
	protected Q_SLOTS:
	void openImageDirectory();
	void about();
	
	private:
	GLWidget* m_glwidget;
	BundleParser* m_bundleparser;
	ImageList* m_imagelist;
	
	QAction* m_openAct,* m_exitAct,* m_aboutAct,* m_aboutQtAct;
	QLabel* m_status;
};

#endif

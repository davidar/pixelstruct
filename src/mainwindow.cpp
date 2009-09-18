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

#include "mainwindow.h"

#include <QFileDialog>
#include <QApplication>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>

using std::cerr;
using std::endl;

MainWindow::MainWindow()
    : m_bundleparser(0), m_imagelist(0) {
    setWindowTitle(tr("PixelStruct"));
    resize(sizeHint());
    createActions();
    createMenus();
    
    m_transmode_2->setChecked(true);
    
    if(qApp->arguments().size() > 1)
        openImageDirectory(qApp->arguments().at(1));
}

MainWindow::~MainWindow() {
}

void MainWindow::openImageDirectory(const QString& image_directory) {
    cerr << "[MainWindow::openImageDirectory] opening "
         << image_directory.toStdString() << endl;
    
    if(m_bundleparser != 0) delete m_bundleparser;
    if(m_imagelist != 0) delete m_imagelist;
    
    m_bundleparser = new BundleParser(image_directory + "/bundle/bundle.out");
    m_imagelist = new ImageList(image_directory + "/list.txt", image_directory,
                                m_bundleparser->num_cameras());
    
    m_glwidget = new GLWidget(m_bundleparser, m_imagelist, this);
    setCentralWidget(m_glwidget);
    setTransMode(m_transmode_group->checkedAction());
    m_glwidget->show();
    m_glwidget->setFocus();
}

void MainWindow::createActions() {
    m_openAct = new QAction(tr("&Open image directory"), this);
    m_openAct->setShortcut(tr("Ctrl+O"));
    connect(m_openAct, SIGNAL(triggered()),
            this, SLOT(openImageDirectoryDialog()));
    
    m_exitAct = new QAction(tr("&Quit"), this);
    m_exitAct->setShortcut(Qt::Key_Escape);
    connect(m_exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
    
    m_transmode_group = new QActionGroup(this);
    m_transmode_none = new QAction(tr("Do&n't display images"), this);
    m_transmode_none->setCheckable(true);
    m_transmode_group->addAction(m_transmode_none);
    m_transmode_0 = new QAction(tr("&Simple transitions"), this);
    m_transmode_0->setCheckable(true);
    m_transmode_group->addAction(m_transmode_0);
    m_transmode_1 = new QAction(tr("&Common Plane transitions"), this);
    m_transmode_1->setCheckable(true);
    m_transmode_group->addAction(m_transmode_1);
    m_transmode_2 = new QAction(tr("&Triangulated transitions"), this);
    m_transmode_2->setCheckable(true);
    m_transmode_group->addAction(m_transmode_2);
    connect(m_transmode_group, SIGNAL(triggered(QAction*)),
            this, SLOT(setTransMode(QAction*)));
    
    m_aboutAct = new QAction(tr("&About"), this);
    connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    
    m_aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(m_aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus() {
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_openAct);
    fileMenu->addAction(m_exitAct);
    
    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addActions(m_transmode_group->actions());
    
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(m_aboutAct);
    helpMenu->addAction(m_aboutQtAct);
}

void MainWindow::openImageDirectoryDialog() {
    QString image_directory = QFileDialog::getExistingDirectory(this,
        tr("Select image directory"), "", QFileDialog::ShowDirsOnly);
    if(image_directory.isEmpty()) return;
    openImageDirectory(image_directory);
}

void MainWindow::setTransMode(QAction* act) {
    if(act == m_transmode_none)
        m_glwidget->setTransMode(-1);
    else if(act == m_transmode_0)
        m_glwidget->setTransMode(0);
    else if(act == m_transmode_1)
        m_glwidget->setTransMode(1);
    else if(act == m_transmode_2)
        m_glwidget->setTransMode(2);
}

void MainWindow::about() {
    QMessageBox::about(this, "PixelStruct 0.2",
    "\nCopyright (c) 2009 David Roberts <d@vidr.cc>\n\n"
    "This program is free software; you can redistribute it and/or modify\n"
    "it under the terms of the GNU General Public License as published by\n"
    "the Free Software Foundation; either version 2 of the License, or\n"
    "(at your option) any later version.\n\n"
    "For more information visit the PixelStruct homepage at:\n"
    "http://da.vidr.cc/projects/pixelstruct/\n");
}

#include "mainwindow.moc"

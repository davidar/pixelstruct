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

#include "bundleparser.h"

#include <fstream>
#include <iostream>
#include <string>

#include <QDebug>

using std::cerr;
using std::endl;
using std::ifstream;
using std::string;
using std::getline;

BundleParser::BundleParser(const char* file) {
    parse(file);
}

BundleParser::BundleParser(QString file) {
    parse(file.toStdString().c_str());
}


BundleParser::~BundleParser() {
    delete [] m_cameras;
    delete [] m_points;
}

void BundleParser::parse(const char* file) {
    ifstream fin(file);
    if(!fin) {
        cerr << "[BundleParser::parse] Unable to open file " << file << endl;
        exit(1);
    } else {
        cerr << "[BundleParser::parse] Successfully opened file " << file
             << endl;
    }
    
    string header;
    getline(fin, header);
    cerr << "[BundleParser::parse] Bundle header: " << header << endl;
    
    fin >> m_num_cameras >> m_num_points;
    
    cerr << "[BundleParser::parse] " << m_num_cameras << " cameras" << endl;
    cerr << "[BundleParser::parse] " << m_num_points << " points" << endl;
    
    m_cameras = new Camera[m_num_cameras];
    for(int i = 0; i < m_num_cameras; i++) {
        double f, k1, k2;
        fin >> f >> k1 >> k2;
        
        double* m = new double[16];
        
        /// given rotation matrix R and translation vector t,
        /// column-major matrix m is equal to:
        /// [ R11 R12 R13 t.x ]
        /// | R21 R22 R23 t.y |
        /// | R31 R32 R33 t.z |
        /// [ 0.0 0.0 0.0 1.0 ]
        
        fin >> m[0]     >> m[4]     >> m[ 8]
            >> m[1]     >> m[5]     >> m[ 9]
            >> m[2]     >> m[6]     >> m[10];
               m[3] = 0.0; m[7] = 0.0; m[11] = 0.0;
        
        fin >> m[12]
            >> m[13]
            >> m[14];
               m[15] = 1.0;
        
        Camera camera(i, f, k1, k2, m);
        m_cameras[i] = camera;
    }
    
    cerr << "[BundleParser::parse] Finished loading cameras" << endl;
    
    m_points = new VisiblePoint[m_num_points];
    for(int i = 0; i < m_num_points; i++) {
        double pos_x, pos_y, pos_z;
        fin >> pos_x >> pos_y >> pos_z;
        
        int col_r, col_g, col_b;
        fin >> col_r >> col_g >> col_b;
        
        int num_views;
        fin >> num_views;
        
        for(int j = 0; j < num_views; j++) {
            int camera, key;
            float px, py;
            fin >> camera >> key >> px >> py;
            // TODO store this information somewhere
            m_cameras[camera].add_visible_point(i);
        }
        
        VisiblePoint point(Point(pos_x, pos_y, pos_z),
                           QColor(col_r, col_g, col_b));
        m_points[i] = point;
    }
    
    cerr << "[BundleParser::parse] Finished loading points" << endl;
    
    fin.close();
}

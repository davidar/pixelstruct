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

#include "imagelist.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

using std::string;
using std::cerr;
using std::endl;
using std::getline;
using std::ifstream;

ImageList::ImageList(const char* file, const char* directory, int num_images)
	: m_num_images(num_images) {
	m_filenames = new string[m_num_images];
	parse(file, directory);
}

ImageList::ImageList(QString file, QString directory, int num_images)
	: m_num_images(num_images) {
	m_filenames = new string[m_num_images];
	parse(file.toStdString().c_str(), directory.toStdString().c_str());
}


ImageList::~ImageList() {
}

void ImageList::parse(const char* file, const char* directory) {
	ifstream fin(file);
	if(!fin) {
		cerr << "[ImageList::parse] Unable to open file " << file << endl;
		exit(1);
	} else {
		cerr << "[ImageList::parse] Successfully opened file " << file << endl;
	}
	
	for(int i = 0; i < m_num_images; i++) {
		fin >> m_filenames[i];
		if(m_filenames[i][0] == '.' && m_filenames[i][1] == '/')
			m_filenames[i] = string(directory) + "/" + m_filenames[i];
		
		string image_args;
		getline(fin, image_args);
	}
}

const QImage ImageList::loadImage(int index) const {
	return QImage(m_filenames[index].c_str());
}

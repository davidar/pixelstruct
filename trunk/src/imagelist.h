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

#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <string>

#include <QString>
#include <QImage>

class ImageList {
	public:
	ImageList(const char*, const char*, int);
	ImageList(QString, QString, int);
	~ImageList();
	
	const QImage loadImage(int) const;
	
	protected:
	void parse(const char*, const char*);
	
	private:
	int m_num_images;
	std::string* m_filenames;
};

#endif

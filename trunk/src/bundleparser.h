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

#ifndef BUNDLEPARSER_H
#define BUNDLEPARSER_H

#include "camera.h"
#include "visiblepoint.h"

#include <QString>

class BundleParser {
	public:
	BundleParser(const char* file);
	BundleParser(QString file);
	~BundleParser();
	const Camera* cameras() const {return m_cameras;}
	const VisiblePoint* points() const {return m_points;}
	int num_cameras() const {return m_num_cameras;}
	int num_points() const {return m_num_points;}
	
	protected:
	void parse(const char* file);
	
	private:
	Camera* m_cameras;
	VisiblePoint* m_points;
	int m_num_cameras, m_num_points;
};

#endif

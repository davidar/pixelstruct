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

#ifndef POINT_H
#define POINT_H

class Point {
	public:
	Point();
	Point(const double, const double, const double);
	~Point();
	double x() const {return m_x;}
	double y() const {return m_y;}
	double z() const {return m_z;}
	double sqdist() const {return m_x*m_x + m_y*m_y + m_z*m_z;} // square of the distance from the origin
	
	private:
	double m_x, m_y, m_z;
};

#endif

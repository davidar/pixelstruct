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

#ifndef COMMONPLANE_H
#define COMMONPLANE_H

#include "plane.h"

#include "camera.h"
#include "visiblepoint.h"

class CommonPlane : public Plane {
	public:
	CommonPlane();
	CommonPlane(const Camera&, const Camera&, const VisiblePoint*);
	~CommonPlane();
	
	private:
	double m_fitting_quality;
};

#endif

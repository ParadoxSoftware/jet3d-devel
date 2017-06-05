/*!
	@file jeVec3d.cpp

	@author Anthony Rufrano (paradoxnj)
	@brief 3D Vector math

	@par License
	The contents of this file are subject to the Jet3D Public License
	Version 1.02 (the "License"); you may not use this file except in
	compliance with the License. You may obtain a copy of the License at
	http://www.jet3d.com

	@par
	Software distributed under the License is distributed on an "AS IS"
	basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
	the License for the specific language governing rights and limitations
	under the License.

	@par
	The Original Code is Jet3D, released December 12, 1999.
	Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved
*/
#include <math.h>
#include "Basetype.h"
#include "jeVec3d.h"

namespace jet3d {

jeVec3d jeVec3d::UP = jeVec3d(0.0f, 1.0f, 0.0f);
jeVec3d jeVec3d::DOWN = jeVec3d(0.0f, -1.0f, 0.0f);
jeVec3d jeVec3d::LEFT = jeVec3d(-1.0f, 0.0f, 0.0f);
jeVec3d jeVec3d::RIGHT = jeVec3d(1.0f, 0.0f, 0.0f);
jeVec3d jeVec3d::ZERO = jeVec3d(0.0f, 0.0f, 0.0f);
jeVec3d jeVec3d::X_AXIS = jeVec3d(1.0f, 0.0f, 0.0f);
jeVec3d jeVec3d::Y_AXIS = jeVec3d(0.0f, 1.0f, 0.0f);
jeVec3d jeVec3d::Z_AXIS = jeVec3d(0.0f, 0.0f, 1.0f);

jeVec3d::jeVec3d()
{
	x = y = z = 0.0f;
}

jeVec3d::jeVec3d(jeFloat _x, jeFloat _y, jeFloat _z)
{
	x = _x;
	y = _y;
	z = _z;
}

jeVec3d::~jeVec3d()
{
}

} // namespace jet3d

/*!
	@file jeVec3d.h

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
#ifndef __JE_VEC3D_H__
#define __JE_VEC3D_H__

#include "Basetype.h"

namespace jet3d {

struct JETAPI jeVec3d
{
	jeVec3d();
	jeVec3d(jeFloat _x, jeFloat _y, jeFloat _z);
	virtual ~jeVec3d();

	jeFloat x, y, z;

	static jeVec3d UP;
	static jeVec3d DOWN;
	static jeVec3d LEFT;
	static jeVec3d RIGHT;
	static jeVec3d ZERO;

	static jeVec3d X_AXIS;
	static jeVec3d Y_AXIS;
	static jeVec3d Z_AXIS;

	jeVec3d &add(const jeVec3d &v1);
	jeVec3d &subtract(const jeVec3d &v1);
	jeVec3d &scale(jeFloat s);
	jeVec3d cross(const jeVec3d &v1) const;
	jeFloat dot(const jeVec3d &v1) const;

	jeVec3d &inverse();

	jeFloat magnitude() const;
	jeVec3d normalize() const;
	jeFloat distance(const jeVec3d &v1) const;

	bool operator ==(const jeVec3d &v1) const;
	bool operator !=(const jeVec3d &v1) const;

	jeVec3d &operator +=(const jeVec3d &v1);
	jeVec3d &operator -=(const jeVec3d &v1);
	jeVec3d &operator *=(const jeFloat scale);
	jeVec3d &operator /=(const jeFloat scale);
};

__inline jeVec3d operator +(jeVec3d &v1, const jeVec3d &v2)
{
	return v1.add(v2);
}

__inline jeVec3d operator -(jeVec3d &v1, const jeVec3d &v2)
{
	return v1.subtract(v2);
}

__inline jeVec3d operator *(jeVec3d &v1, jeFloat scale)
{
	return v1.scale(scale);
}

} // namespace jet3d

#endif
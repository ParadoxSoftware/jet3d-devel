/****************************************************************************************/
/*  VEC3D.H                                                                             */
/*                                                                                      */
/*  Author:                                                                             */
/*  Description: 3D Vector interface                                                    */
/*                                                                                      */
/*  The contents of this file are subject to the Jet3D Public License                   */
/*  Version 1.02 (the "License"); you may not use this file except in                   */
/*  compliance with the License. You may obtain a copy of the License at                */
/*  http://www.jet3d.com                                                                */
/*                                                                                      */
/*  Software distributed under the License is distributed on an "AS IS"                 */
/*  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See                */
/*  the License for the specific language governing rights and limitations              */
/*  under the License.                                                                  */
/*                                                                                      */
/*  The Original Code is Jet3D, released December 12, 1999.                             */
/*  Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved           */
/*                                                                                      */
/****************************************************************************************/
#ifndef JE_VEC3D_H
#define JE_VEC3D_H

#include "BaseType.h"

typedef struct jeVec3d
{
	float X, Y, Z, Pad;

	static jeVec3d UP;
	static jeVec3d DOWN;
	static jeVec3d LEFT;
	static jeVec3d RIGHT;
	static jeVec3d ZERO;

	static jeVec3d X_AXIS;
	static jeVec3d Y_AXIS;
	static jeVec3d Z_AXIS;

	jeVec3d() { X = Y = Z = Pad = 0.0f; }
	jeVec3d(float x, float y, float z) { X = x; Y = y; Z = z; Pad = 0.0f; }
	jeVec3d(float x, float y, float z, float pad) { X = x; Y = y; Z = z; Pad = pad; }	

	// inline jeVec3d operator =(const jeVec3d& other) {
	// 	return jeVec3d(other.X, other.Y, other.Z);
	// }

	inline bool operator ==(const jeVec3d& other) {
		return ((X == other.X) && (Y == other.Y) && (Z == other.Z));
	}

	inline bool operator !=(const jeVec3d& other) {
		return ((X != other.X) && (Y != other.Y) && (Z != other.Z));
	}

	inline jeVec3d operator +(const jeVec3d& other) { 
		return add(other);
	}

	inline jeVec3d operator -(const jeVec3d& other) {
		return subtract(other);
	}

	inline float operator *(const jeVec3d& other) {
		return dot(other);
	}

	inline jeVec3d operator *(float s) {
		return scale(s);
	}

	inline jeVec3d operator ^(const jeVec3d& other) {
		return cross(other);
	}

	inline jeVec3d operator ~() {
		return inverse();
	}

	jeVec3d add(const jeVec3d &other)
	{
		return jeVec3d(X + other.X, Y + other.Y, Z + other.Z);
	}

	jeVec3d subtract(const jeVec3d &other) {
		return jeVec3d(X - other.X, Y - other.Y, Z - other.Z);
	}

	jeVec3d scale(float scale) {
		return jeVec3d(X * scale, Y * scale, Z * scale);
	}

	jeVec3d addScaled(const jeVec3d& other, const float scale) {
		jeVec3d result = add(other);
		return result.scale(scale);
	}

	jeVec3d cross(const jeVec3d &other) {
		return jeVec3d(
			Y * other.Z - Z * other.Y,
			Z * other.X - X * other.Z,
			X * other.Y - Y * other.X
		);
	}
	
	float dot(const jeVec3d &other) {
		return ((X * other.X) + (Y * other.Y) + (Z * other.Z));
	}

	jeVec3d inverse() {
		return jeVec3d(-X, -Y, -Z);
	}

	jeFloat magnitude() {
		return sqrtf(dot(*this));
	}

	float normalize() {
		float dist = magnitude();
		if (dist == 0.0f)
			return 0.0f;

		float oneOverDist = 1.0f / dist;
		X *= oneOverDist;
		Y *= oneOverDist;
		Z *= oneOverDist;

		return dist;
	}

	float distance(const jeVec3d &other) {
		jeVec3d diff = subtract(other);
		return diff.dot(diff);
	}
} jeVec3d;

#ifndef NDEBUG
JETAPI	jeFloat JETCC   jeVec3d_GetElement(const jeVec3d *V, int32 Index);
JETAPI	void JETCC		jeVec3d_SetElement(jeVec3d *V, int32 Index, jeFloat Value);
#else
	#define jeVec3d_GetElement(Vector,Index)  (* ((&((Vector)->X)) +  (Index) ))
	#define jeVec3d_SetElement(Vector,Index, Value) ((* ((&((Vector)->X)) +  (Index) )) = Value)
#endif

JETAPI void JETCC		jeVec3d_Set(jeVec3d *V, jeFloat X, jeFloat Y, jeFloat Z);
JETAPI void JETCC		jeVec3d_Get(const jeVec3d *V, jeFloat *X, jeFloat *Y, jeFloat *Z);

JETAPI jeFloat JETCC	jeVec3d_DotProduct(const jeVec3d *V1, const jeVec3d *V2);
JETAPI void JETCC		jeVec3d_CrossProduct(const jeVec3d *V1, const jeVec3d *V2, jeVec3d *VResult);
JETAPI jeBoolean JETCC	jeVec3d_Compare(const jeVec3d *V1, const jeVec3d *V2,jeFloat tolarance);
JETAPI jeFloat JETCC	jeVec3d_Normalize(jeVec3d *V1);
JETAPI jeBoolean JETCC 	jeVec3d_IsNormalized(const jeVec3d *V);
JETAPI void JETCC		jeVec3d_Scale(const jeVec3d *VSrc, jeFloat Scale, jeVec3d *VDst);
JETAPI jeFloat JETCC	jeVec3d_Length(const jeVec3d *V1); 
JETAPI jeFloat JETCC	jeVec3d_LengthSquared(const jeVec3d *V1); 
JETAPI void JETCC		jeVec3d_Subtract(const jeVec3d *V1, const jeVec3d *V2, jeVec3d *V1MinusV2);
JETAPI void JETCC		jeVec3d_Add(const jeVec3d *V1, const jeVec3d *V2,  jeVec3d *VSum);
JETAPI void JETCC		jeVec3d_Copy(const jeVec3d *Vsrc, jeVec3d *Vdst);
JETAPI void JETCC		jeVec3d_Clear(jeVec3d *V);
JETAPI void JETCC		jeVec3d_Inverse(jeVec3d *V);
JETAPI void JETCC		jeVec3d_MA(jeVec3d *V1, jeFloat Scale, const jeVec3d *V2, jeVec3d *V1PlusV2Scaled);
JETAPI void JETCC		jeVec3d_AddScaled(const jeVec3d *V1, const jeVec3d *V2, jeFloat Scale, jeVec3d *V1PlusV2Scaled);

JETAPI jeFloat JETCC	jeVec3d_DistanceBetween(const jeVec3d *V1, const jeVec3d *V2);	// returns length of V1-V2	
JETAPI jeFloat JETCC	jeVec3d_DistanceBetweenSquared(const jeVec3d *V1, const jeVec3d *V2);

JETAPI jeBoolean JETCC jeVec3d_IsValid(const jeVec3d *V);

#endif

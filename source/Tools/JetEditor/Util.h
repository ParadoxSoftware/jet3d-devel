/*
	@file util.cpp
	@author paradoxnj
	@brief Editor utility functions

	@par license
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
#pragma once

#include <Math.h>

#include "ExtBox.h"
#include "Point.h"
#include "VFile.h"
#include "XForm3d.h"

#pragma warning( disable: 4514 )// Do NOT report "Inline Function Removed" as a warning

static const int32 UTIL_MAX_RESOURCE_LENGTH = 128;
static const jeFloat FLOAT_NEAR_ZERO = 0.00001f;

//#define UTIL_MAX_RESOURCE_LENGTH	(128)
//#define FLOAT_NEAR_ZERO				(0.00001f)

class Util
{
public:
	Util(){}
	virtual ~Util(){}

	static void			Init(unsigned long hStringResources);

	//static char *		GetRcString(char * psz, const unsigned int idResource);
	//static char *		LoadLocalRcString(unsigned int resid);  //allocates memory and fills string
	//static char *		LoadText(unsigned int resid);
	static jeBoolean	IsKeyDown(int vKey);
	static char *		StrDup(const char * const psz);
	static int			GetAppPath(wchar_t *Buf, int BufSize);
	static float		GetTime();

	// GDI THIN WRAPS
	static jeBoolean	Polyline(int32 hDC, Point * pPoints, int32 nPoints);

	// MATH AND SUCH
	static void			ExtBox_Union(const jeExtBox *B1, const jeExtBox *B2, jeExtBox *Result);
	static void			ExtBox_ExtendToEnclose(jeExtBox *B, const jeVec3d *Point);
	static jeFloat		ExtBox_GetExtent(const jeExtBox *B, int32 nElement);
	static void			ExtBox_InitFromTwoPoints(jeExtBox * B, const jeVec3d * p1, const jeVec3d * p2);
	static jeBoolean	ExtBox_Intersection(const jeExtBox *B1, const jeExtBox *B2, jeExtBox *Result);
	static void			ExtBox_SetInvalid(jeExtBox *B);
	static void			ExtBox_Transform(const jeExtBox *B, const jeXForm3d *XForm, jeExtBox *Result);
	static void			ExtBox_TransformJ(const jeExtBox *B, const jeXForm3d *XForm, jeExtBox *Result);
	static jeFloat		log2(jeFloat f);
	static jeFloat		NearestLowerPowerOf2(const jeFloat fVal);

	static int32		Time(void);

	static jeFloat		PointToLineDistanceSquared(const Point * pL1, const Point * pL2, const Point * pPoint);
	static jeFloat		PointDistanceSquared(const Point * pL1, const Point * pL2);

	// FILE STUFF
	static void			DriveAndPathOnly(char * pszPath);
	static void			NameOnly(char * pszPath);
	static void			NewExtension(char * pszString, const char * pszNewExt);
	static void			StripTrailingBackslash(char * pszString);
	static jeBoolean	VFile_ReadString(jeVFile * pFile, char * pszBuffer, const int32 nMaxChars);

	static __inline jeBoolean IsFloatZero(jeFloat f)
	{
		if (fabs(f) < FLOAT_NEAR_ZERO)
			return JE_TRUE;

		return JE_FALSE;
	}

	static __inline jeFloat Round(jeFloat f)
	{
		return (jeFloat)floor((f)+0.5f);
	}
};

/****************************************************************************************/
/*  RECT.H                                                                              */
/*                                                                                      */
/*  Author:                                                                             */
/*  Description:                                                                        */
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
#pragma once

// Disable inline functions removed warning
//#pragma warning( disable: 4514 4201 )

#include "Point.h"
#include "BaseType.h"

class Rect
{
public:
	Rect();
	Rect(int32 left, int32 top, int32 right, int32 bottom);
	Rect(Point &topLeft, Point &bottomRight);
	virtual ~Rect();

	Rect& operator =(const Rect &r1);

public:
	union
	{
		struct {
			int32 Left;
			int32 Top;
			int32 Right;
			int32 Bottom;
		};
		struct {
			Point TopLeft;
			Point BotRight;
		};
	};

public:
	__inline int32 Width() { return(Right - Left); }
	__inline int32 Height() { return(Bottom - Top); }

	jeBoolean	Compare(const Rect& Rect2);

	jeBoolean	IsContained(const Rect& pQueryRect);
	jeBoolean	IsEmpty();
	jeBoolean	IsIntersecting(const Rect& pRect2);
	jeBoolean	IsPointIn(const Point &pPoint);


	jeBoolean	Intersect(Rect &pIntersectedRect, const Rect &pR2);
	void		Normalize();
	jeBoolean	Union(Rect &pDest, const Rect &pR2);
	void		Offset(int32 dx, int32 dy);
	void		Inflate(int32 dx, int32 dy);
	void		ExtendToEnclose(const Point &pPoint);

	void		SetEmpty();
	void		GetTranslation(Point &pCenter);
};

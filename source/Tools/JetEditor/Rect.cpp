/****************************************************************************************/
/*  RECT.C                                                                              */
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
#include <Assert.h>
#pragma warning(disable : 4201 4214 4115)
#include <Windows.h>
#include <Windowsx.h>
#pragma warning(default : 4201 4214 4115; disable : 4514)

#include "Rect.h"

Rect::Rect()
{
}

Rect::Rect(int32 left, int32 top, int32 right, int32 bottom)
{
	Left = left;
	Top = top;
	Right = right;
	Bottom = bottom;
}

Rect::Rect(Point &topLeft, Point &bottomRight)
{
	TopLeft = topLeft;
	BotRight = bottomRight;
}

Rect::~Rect()
{}

Rect& Rect::operator =(const Rect &r1)
{
	Left = r1.Left;
	Right = r1.Right;
	Top = r1.Top;
	Bottom = r1.Bottom;

	return *this;
}

jeBoolean Rect::Compare(const Rect& pRect2)
{
	return(EqualRect((const RECT*)this, (const RECT*)&pRect2));
}

jeBoolean Rect::IsContained(const Rect& pQueryRect)
{
	if( (pQueryRect.Left >= Left) &&
		(pQueryRect.Top >= Top) &&
		(pQueryRect.Right <= Right) &&
		(pQueryRect.Bottom <= Bottom) )
	{
		return(JE_TRUE);
	}

	return(JE_FALSE);
}

jeBoolean Rect::IsIntersecting(const Rect& pRect2)
{
	RECT r;

	return(IntersectRect(&r, (const RECT*)this, (const RECT*)&pRect2));
}

// NZ == intersected, Z = no intersection
jeBoolean Rect::Intersect( Rect& pIntersectedRect, const Rect &pR2 )
{
	return IntersectRect( (RECT*)&pIntersectedRect, (const RECT*)this, (const RECT*)&pR2 ) ;
}


void Rect::Normalize()
{
    int nTemp;

    if ( Left > Right)
    {
        nTemp = Left ;
        Left = Right;
        Right = nTemp;
    }
    if (Top > Bottom)
    {
        nTemp = Top;
        Top = Bottom;
        Bottom = nTemp;
    }
}

jeBoolean Rect::Union( Rect &pDest, const Rect &pR2 )
{
	return UnionRect( (RECT*)&pDest, (const RECT*)this, (const RECT*)&pR2 ) ;
}

void Rect::Offset( int32 dx, int32 dy )
{
	OffsetRect( (RECT*)this, dx, dy ) ;
}

void Rect::Inflate( int32 dx, int32 dy )
{
	InflateRect( (RECT*)this, dx, dy ) ;
}

void Rect::ExtendToEnclose( const Point &pPoint )
{
	if( pPoint.X > Right ) Right = pPoint.X;
	if( pPoint.Y > Bottom ) Bottom = pPoint.Y;

	if( pPoint.X < Left ) Left = pPoint.X;
	if( pPoint.Y < Top ) Top = pPoint.Y;
}

void Rect::SetEmpty()
{
	SetRectEmpty( (RECT*)this ) ;
}

//
// IS Functions
//

jeBoolean Rect::IsEmpty()
{
	return IsRectEmpty( (RECT*)this ) ;
}

jeBoolean Rect::IsPointIn( const Point &pPoint )
{
	if( pPoint.X >= Left && pPoint.X <= Right )
		if( pPoint.Y >= Top && pPoint.Y <= Bottom )
			return JE_TRUE ;

	return JE_FALSE ;
}

void Rect::GetTranslation( Point &pCenter )
{
	pCenter.X = (Left + Right)/2 ;
	pCenter.Y = (Top + Bottom)/2 ;
}

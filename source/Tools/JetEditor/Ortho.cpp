/*
	@file Ortho.cpp
	@author paradoxnj
	@brief 2D editor view

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
#include <Assert.h>
#include <Float.h>

#pragma warning(disable : 4201 4214 4115)
#include <Windows.h>
#include <Windowsx.h>
#pragma warning(default : 4201 4214 4115; disable : 4514)

#include "ExtBox.h"
#include "jet.h"
#include "Ram.h"
#include "Units.h"
#include "Util.h"

#include "Ortho.h"

//#define SIGNATURE						(0xFDB97531)
//#define ORTHO_MAX_NAME_LENGTH			(32)
//#define ORTHO_MAXPOINTSPERFACE			(64)
//#define ORTHO_DEFAULTSELECTTOLERANCE	(64)

static const DWORD SIGNATURE = 0xFDB97531;
static const int ORTHO_MAX_NAME_LENGTH = 32;
static const int ORTHO_MAXPOINTSPERFACE = 64;
static const int ORTHO_DEFAULTSELECTTOLERANCE = 64;

Ortho::Ortho()
{
	nPixelSelectThreshold = ORTHO_DEFAULTSELECTTOLERANCE ;
	ZoomFactor = 1.0f;
	Height = 10;
	Width = 10;
}

Ortho::~Ortho()
{
	if( hDibSec != NULL )
	{
		DeleteObject( hDibSec ) ;
		hDibSec = NULL ;
	}
}

//
// ACCESSORS
//

jeFloat Ortho::GetGridDistance()
{
	jeExtBox	Distance ;
	ORTHO_AXIS	HAxis ;
	float		Extent ;

	// determine grid size for minimum 10 pixels between grid lines
	ViewToWorld( 0, 0, &Distance.Min );
	ViewToWorld( 10, 0, &Distance.Max );
	HAxis = GetHorizontalAxis();
	Extent = Util::ExtBox_GetExtent( &Distance, HAxis ) ;
	if( Extent < 1.0f )
		Extent = 1.0f ;

	return Util::NearestLowerPowerOf2( Extent ) ;

}

ORTHO_AXIS Ortho::GetHorizontalAxis()
{
	switch( ViewType )
	{
		case Ortho_ViewFront :	return Ortho_Axis_X ;
		case Ortho_ViewSide :	return Ortho_Axis_Z ;
		case Ortho_ViewTop :	return Ortho_Axis_X ;
	}

	return Ortho_Axis_Last ;
}

ORTHO_AXIS Ortho::GetOrthogonalAxis()
{
	switch(ViewType)
	{
		case Ortho_ViewFront :	return Ortho_Axis_Z ;
		case Ortho_ViewSide :	return Ortho_Axis_X ;
		case Ortho_ViewTop :	return Ortho_Axis_Y ;
	}
	
	return (ORTHO_AXIS)0 ;
}

jeFloat Ortho::GetRotationFromView(Point *pMousePt, Point *pAnchor, Point * pSelCenter)
{
	jeFloat	fRadians ;
	jeVec3d CtoM;
	jeVec3d CtoA;
	jeVec3d Cross;
	jeFloat	fCos;

	jeVec3d_Set( &CtoM,
		(float)(pSelCenter->X - pMousePt->X),
		(float)(pSelCenter->Y - pMousePt->Y),
		0.0f
	);
	jeVec3d_Normalize( &CtoM );

	jeVec3d_Set( &CtoA,
		(float)(pSelCenter->X - pAnchor->X),
		(float)(pSelCenter->Y - pAnchor->Y),
		0.0f
	);
	jeVec3d_Normalize( &CtoA );
	if( jeVec3d_Compare( &CtoA, &CtoM, 0.001f) )
		return( 0.0f );
	fCos = jeVec3d_DotProduct( &CtoA, &CtoM );
	if( fCos == -1.0f )
		fRadians =M_PI;	
	else
		fRadians = (float)acos( fCos );
	jeVec3d_CrossProduct( &CtoA, &CtoM, &Cross);
	if( ViewType != Ortho_ViewSide  )
	{
		if( Cross.Z > 0 )
			fRadians = (2.0f * M_PI) - fRadians;
	}
	else
	if( Cross.Z < 0 )
	{
		fRadians = (2.0f * M_PI) - fRadians;
	}
	assert( fRadians * fRadians >= 0.0f );

	return fRadians ;
}

ORTHO_AXIS Ortho::GetVerticalAxis()
{
	switch( ViewType )
	{
		case Ortho_ViewFront :	return Ortho_Axis_Y ;
		case Ortho_ViewSide :	return Ortho_Axis_Y ;
		case Ortho_ViewTop :	return Ortho_Axis_Z ;
	}
	
	return Ortho_Axis_Last ;
}

Ortho::Ortho_ViewType Ortho::GetViewType()
{
	return ViewType ;
}

long Ortho::GetWidth()
{
	return Width ;
}

long Ortho::GetHeight()
{
	return Height ;
}

const std::string &Ortho::GetName()
{
	return strName ;
}

int32 Ortho::GetViewSelectThreshold()
{
	return nPixelSelectThreshold ;
}

jeFloat Ortho::GetWorldSelectThreshold()
{
	return fWorldSelectThreshold ;
}

jeFloat Ortho::GetWorldHandleSelectThreshold()
{
	return fWorldHandleSelectThreshold ;
}

//
// IS
//
jeBoolean Ortho::IsViewPointInWorldBox(const int x, const int y, const jeExtBox * pWorldBox)
{
	jeExtBox	Box;
	jeVec3d		World;
	ORTHO_AXIS	OrthoAxis;

	assert( pWorldBox != NULL );

	Box = *pWorldBox;
	ViewToWorld( x, y, &World );
	
	// The remaining axis is set to extremes
	OrthoAxis = GetOrthogonalAxis();
	jeVec3d_SetElement( &Box.Min, OrthoAxis, -FLT_MAX ) ;
	jeVec3d_SetElement( &Box.Max, OrthoAxis, FLT_MAX ) ;

	return jeExtBox_ContainsPoint( &Box, &World ) ;

}

//
// MODIFIERS
//
void Ortho::MoveCamera(const jeVec3d * pDelta)
{
	jeVec3d_Add( &CamPos, pDelta, &CamPos ) ;
	UpdateWorldBounds();

}

void Ortho::ResetSettings(long vx, long vy)
{
	ResizeView(vx, vy);

	// Compute and set zoom factor
	SetZoom(Width / 640.0f);
	SetAnglesRPY(0.0f, M_PI, 0.0f);
	jeVec3d_Clear(&CamPos);

	UpdateWorldBounds();

}// Ortho_ResetSettings

void Ortho::ResizeView(long vx, long vy)
{
	HDC		ViewDC;

	vx=(vx+3)&~3;	//Align scan delta

	if( vx && vy )
	{
		if( hDibSec != NULL )
		{
			DeleteObject( hDibSec ) ;
			hDibSec = NULL ;
		}

		//Force top-down 8-bit bitmap of size WINDOW_WIDTH*WINDOW_HEIGHT.
		BMI.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
		BMI.bmiHeader.biPlanes       = 1;
		BMI.bmiHeader.biBitCount     = 16;
		BMI.bmiHeader.biCompression  = BI_RGB;
		BMI.bmiHeader.biSizeImage    = 0;
		BMI.bmiHeader.biClrUsed      = 0;
		BMI.bmiHeader.biClrImportant = 0;
		BMI.bmiHeader.biWidth        = vx;
		BMI.bmiHeader.biHeight       = -vy;    // Minus for top-down.

		ViewDC = CreateCompatibleDC( NULL ) ;
		assert(ViewDC);

		hDibSec = CreateDIBSection
		(
			ViewDC, 
			(BITMAPINFO *)&BMI, 
			DIB_RGB_COLORS, 
			(void **)&pBits, 
			NULL, 
			0
		);
		assert( hDibSec ) ;

		DeleteDC( ViewDC ) ;
	}

	FieldOfView		= 2.0f;	//fixed for now?
	XScreenScale	= ((jeFloat)vx) / FieldOfView ;
	YScreenScale	= ((jeFloat)vy) / FieldOfView ;
	MaxScale		= max( XScreenScale, YScreenScale ) ;
	MaxScaleInv		= 1.0f / MaxScale ;
	XCenter			= ((jeFloat)vx) / 2.0f - 0.5f ;
	YCenter			= ((jeFloat)vy) / 2.0f - 0.5f ;
	Width			= vx;
	Height			= vy;
}

void Ortho::SetAngles(const jeVec3d * pAngles)
{
	Angles = *pAngles ;
}

void Ortho::SetAnglesRPY(jeFloat roll, jeFloat pitch, jeFloat yaw)
{
	Angles.X = roll ;
	Angles.Y = pitch ;
	Angles.Z = yaw ;
}

void Ortho::SetBoxOrthogonalToMax(jeExtBox * pBox)
{
	ORTHO_AXIS	OrthoAxis ;
	assert( pBox != NULL ) ;
	
	OrthoAxis = GetOrthogonalAxis() ;
	jeVec3d_SetElement( &pBox->Min, OrthoAxis, -FLT_MAX ) ;
	jeVec3d_SetElement( &pBox->Max, OrthoAxis, FLT_MAX ) ;
}

void Ortho::SetCameraPos(const jeVec3d * pPos)
{
	CamPos = *pPos ;
	UpdateWorldBounds() ;
}

void Ortho::SetSelectThreshold(const int nPixels)
{
	nPixelSelectThreshold = nPixels ;
}

void Ortho::SetViewType(const Ortho_ViewType vt)
{
	/*int	nID ;
	
	ViewType = vt ;
	nID = IDS_FRONT ;
	switch( vt )
	{
	case Ortho_ViewFront :	nID = IDS_FRONT ;	break ;
	case Ortho_ViewSide :	nID = IDS_SIDE ;	break ;
	case Ortho_ViewTop :	nID = IDS_TOP ;		break ;
	}
	Util_GetRcString( pOrtho->szName, nID ) ;*/

}

void Ortho::SetZoom(const jeFloat zf)
{
	ZoomFactor = zf ;

	fWorldSelectThreshold = (float)nPixelSelectThreshold  ; 
	fWorldHandleSelectThreshold = HANDLESIZE  ; 
	UpdateWorldBounds();
}

void Ortho::UpdateWorldBounds()
{
	ORTHO_AXIS	OrthoAxis ;
	
	switch( ViewType )
	{
		case Ortho_ViewTop :
			ViewToWorld( 0, 0, &WorldBounds.Min ) ;
			ViewToWorld( Width-1, Height-1, &WorldBounds.Max ) ;
			break;

		case Ortho_ViewFront :
			ViewToWorld( 0, Height-1, &WorldBounds.Min ) ;
			ViewToWorld( Width-1, 0, &WorldBounds.Max ) ;
			break;

		case Ortho_ViewSide :
			ViewToWorld( 0, Height-1, &WorldBounds.Min ) ;
			ViewToWorld( Width-1, 0, &WorldBounds.Max ) ;
			break ;
	}

	OrthoAxis = GetOrthogonalAxis() ;		// Remaing access set to extremes
	jeVec3d_SetElement( &WorldBounds.Min, OrthoAxis, -FLT_MAX ) ;
	jeVec3d_SetElement( &WorldBounds.Max, OrthoAxis, FLT_MAX ) ;

}

void Ortho::ZoomChange(const jeFloat fFactor)
{
	jeFloat fNewZoom;
	jeFloat fDist;

	fNewZoom = ZoomFactor * (1.0f + fFactor ) ;

	fDist = fNewZoom * (jeFloat)Width ;
	if( ((fDist < 1.0f) && (fNewZoom < ZoomFactor )) ||
		((fDist > 100000.0f) && (fNewZoom > ZoomFactor )) )
	{
		// either way too small or way too big,
		// and trying to make it worse
		MessageBeep( (UINT)-1 ) ;
	}
	else
	{
		SetZoom( fNewZoom ) ;
	}
}

//
// COORDINATES AND TRANSLATION
//

// Return world position at center of view
void Ortho::GetViewCenter(jeVec3d * pCenter)
{
	jeVec3d TopLeft;
	jeVec3d BottomRight;

	ViewToWorld( 0, 0, &TopLeft ) ;
	ViewToWorld( Width-1, Height-1, &BottomRight ) ;
	jeVec3d_Add( &TopLeft, &BottomRight, pCenter );
	jeVec3d_Scale( pCenter, 0.5f, pCenter ) ;
}

void Ortho::ViewToWorld( const int x, const int y, jeVec3d *pW )
/*
  XY view coordinate transformed to world coordinate, depending on view.

    Mouse Coordinates
    -----------------

		   |
		   |
 		   |
	-------+-------> +X
		   |
		   |
		   |
		   +Y


        Top View				  Front View				   Side View
  	    --------				  ----------				   ---------
									   +Y						   +Y
		   |						   |						   |
		   |						   |						   |
		   |						   |						   |
	-------+-------> +X			-------+-------> +X			-------+-------> +Z
		   |						   |						   |
		   |						   |						   |
		   |						   |						   |
  		   +Z
*/
{
	jeFloat	ZoomInv= 1.0f / ZoomFactor ;

	switch( ViewType )
	{
		case Ortho_ViewTop :
			jeVec3d_Set( pW, (x - XCenter), 0.0f, (y - YCenter)) ;
			jeVec3d_Scale( pW, ZoomInv, pW ) ;
			jeVec3d_Add( pW, &CamPos, pW ) ;
			break;

		case Ortho_ViewFront :
			jeVec3d_Set( pW, (x - XCenter), -(y - YCenter), 0.0f ) ;
			jeVec3d_Scale( pW, ZoomInv, pW ) ;
			jeVec3d_Add( pW, &CamPos, pW ) ;
			break;

		case Ortho_ViewSide :
			jeVec3d_Set( pW, 0.0f, -(y - YCenter), (x - XCenter) ) ;
			jeVec3d_Scale( pW, ZoomInv, pW ) ;
			jeVec3d_Add( pW, &CamPos, pW ) ;
			break;
	}
}

void Ortho::ViewToWorldRect( const Point * pV1, const Point * pV2, jeExtBox * pWorldBox )
{
	jeVec3d	Vec1 ;
	jeVec3d Vec2 ;
	
	assert( pV1 != NULL ) ;
	assert( pV2 != NULL ) ;
	assert( pWorldBox != NULL ) ;
	
	ViewToWorld( pV1->X, pV1->Y, &Vec1 ) ;
	ViewToWorld( pV2->X, pV2->Y, &Vec2 ) ;
	Util::ExtBox_InitFromTwoPoints( pWorldBox, &Vec1, &Vec2 ) ;
	SetBoxOrthogonalToMax( pWorldBox ) ;
}

void Ortho::ViewToWorldDistance( const int x, const int y, jeVec3d *pW )
{
	jeFloat	ZoomInv= 1.0f / ZoomFactor ;
	
	switch( ViewType )
	{
		case Ortho_ViewTop :
			jeVec3d_Set( pW, (jeFloat)x, 0.0f, (jeFloat)y ) ;
			jeVec3d_Scale( pW, ZoomInv, pW ) ;
			break;

		case Ortho_ViewFront :
			jeVec3d_Set( pW, (jeFloat)x, (jeFloat)-y, 0.0f ) ;
			jeVec3d_Scale( pW, ZoomInv, pW ) ;
			break;

		case Ortho_ViewSide :
			jeVec3d_Set( pW, 0.0f, (jeFloat)-y, (jeFloat)x ) ;
			jeVec3d_Scale( pW, ZoomInv, pW ) ;
			break;
	}
}

void Ortho::WorldToView( const jeVec3d * pW, Point * pPt )
{
	jeVec3d ptView;
	
	assert( pW != NULL ) ;
	assert( pPt != NULL ) ;

	switch( ViewType )
	{
		case Ortho_ViewTop :
			jeVec3d_Subtract( pW, &CamPos, &ptView ) ;
			jeVec3d_Scale( &ptView, ZoomFactor, &ptView ) ;
			pPt->X = (int32)(XCenter + ptView.X ) ;
			pPt->Y = (int32)(YCenter + ptView.Z ) ;
			break;

		case Ortho_ViewFront :
			jeVec3d_Subtract( pW, &CamPos, &ptView ) ;
			jeVec3d_Scale( &ptView, ZoomFactor, &ptView ) ;
			pPt->X = (int32)(XCenter + ptView.X ) ;
			pPt->Y = (int32)(YCenter - ptView.Y ) ;
			break;

		case Ortho_ViewSide :
			jeVec3d_Subtract( pW, &CamPos, &ptView ) ;
			jeVec3d_Scale( &ptView, ZoomFactor, &ptView ) ;
			pPt->X = (int32)(XCenter + ptView.Z ) ;
			pPt->Y = (int32)(YCenter - ptView.Y ) ;
			break;
	}
}


void Ortho::WorldToViewRect( const jeExtBox * pWorldBox, Rect * pViewRect )
{
	assert( pWorldBox != NULL ) ;
	assert( pViewRect != NULL ) ;

	WorldToView( &pWorldBox->Min, &pViewRect->TopLeft ) ;
	WorldToView( &pWorldBox->Max, &pViewRect->BotRight ) ;
	
	//Rect_Normalize( pViewRect ) ;
	pViewRect->Normalize();

	if( pViewRect->Bottom == pViewRect->Top )
		pViewRect->Bottom++;
	if( pViewRect->Left == pViewRect->Right )
		pViewRect->Right++;
}

jeBoolean Ortho::TestWorldToViewRect( const jeExtBox * pWorldBox, Rect * pViewRect )
{
	jeBoolean	bIntersects ;
	jeExtBox	IntersectBounds ;
	
	assert( pWorldBox != NULL ) ;
	assert( pViewRect != NULL ) ;
	
	bIntersects = Util::ExtBox_Intersection( &WorldBounds, pWorldBox, &IntersectBounds ) ;
	if( bIntersects )
	{
		WorldToViewRect( &IntersectBounds, pViewRect ) ;
	}

	return bIntersects ;
}

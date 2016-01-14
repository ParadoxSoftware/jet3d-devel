/*
	@file Ortho.h
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
#pragma once

#include <string>

#include "ExtBox.h"
#include "Point.h"
#include "Rect.h"
#include "defs.h"

class Ortho
{
public:
	Ortho();
	virtual ~Ortho();
	
	typedef enum Ortho_ViewType
	{
		Ortho_ViewFront,
		Ortho_ViewSide,
		Ortho_ViewTop,
	} Ortho_ViewType;
		
protected:
	jeVec3d				Angles;
	jeVec3d				CamPos;
	std::string			strName;

	struct
	{
		BITMAPINFOHEADER	bmiHeader;
		RGBQUAD				bmiColors[256];
	} BMI;

	HBITMAP			hDibSec;
	uint32			Flags;
	uint8		*	pBits;
	Ortho_ViewType	ViewType;
	jeFloat			ZoomFactor;
	int32			nPixelSelectThreshold;
	jeFloat			fWorldSelectThreshold;
	jeFloat			fWorldHandleSelectThreshold;
	//	jeVec3d			Vpn, Vright, Vup ;
	//	jeFloat			roll, pitch, yaw;
	jePlane			FrustPlanes[4];
	jeFloat			FieldOfView;
	jeFloat			XCenter, YCenter;
	jeFloat			YScreenScale, XScreenScale;
	jeFloat			MaxScale;
	jeFloat			MaxScaleInv;
	jeFloat			SpeedScale;
	jeExtBox		WorldBounds;
	long			Width;
	long			Height;

public:
	// ACCESSORS
	jeFloat			GetGridDistance();
	long			GetHeight();
	ORTHO_AXIS		GetHorizontalAxis();
	const std::string &GetName();
	ORTHO_AXIS		GetOrthogonalAxis();
	jeFloat			GetRotationFromView(Point *pMousePt, Point *pAnchor, Point * pSelCenter);
	ORTHO_AXIS		GetVerticalAxis();
	Ortho_ViewType	GetViewType();
	int32			GetViewSelectThreshold();
	long			GetWidth();
	jeFloat			GetWorldHandleSelectThreshold();
	jeFloat			GetWorldSelectThreshold();

	// IS
	jeBoolean		IsViewPointInWorldBox(const int x, const int y, const jeExtBox * pWorldBox);

	// MODIFIERS
	void			MoveCamera(const jeVec3d * pDelta);
	void			ResetSettings(long vx, long vy);
	void			ResizeView(long vx, long vy);
	void			SetAngles(const jeVec3d * pAngles);
	void			SetAnglesRPY(jeFloat roll, jeFloat pitch, jeFloat yaw);
	void			SetBoxOrthogonalToMax(jeExtBox * pBox);
	void			SetCameraPos(const jeVec3d * pPos);
	void			SetSelectThreshold(const int nPixels);
	void			SetViewType(const Ortho_ViewType vt);
	void			SetZoom(const jeFloat zf);
	void			UpdateWorldBounds();
	void			ZoomChange(const jeFloat fFactor);


	// COORDINATES AND TRANSLATION
	void			GetViewCenter(jeVec3d * pCenter);
	void			ViewToWorld(const int x, const int y, jeVec3d *pW);
	void			ViewToWorldDistance(const int x, const int y, jeVec3d *pW);
	void			ViewToWorldRect(const Point * pV1, const Point * pV2, jeExtBox * pWorldBox);
	void			WorldToView(const jeVec3d * pW, Point * pPt);
	void            WorldToViewRect(const jeExtBox * pWorldBox, Rect * pViewRect);
	jeBoolean		TestWorldToViewRect(const jeExtBox * pWorldBox, Rect * pViewRect);
};

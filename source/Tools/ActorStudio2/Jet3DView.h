/*
	@file Jet3DView.h
	@author paradoxnj
	@brief Jet3D Engine view

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

#include "wx/wx.h"
#include "jet.h"

class Jet3DView : public wxControl
{
	DECLARE_CLASS(Jet3DView)

public:
	Jet3DView(wxFrame *pParent);
	virtual ~Jet3DView();

	void frame();

	__inline jeActor *GetActor()		{ return m_pActor;  }
	__inline jeActor_Def *GetActorDef()	{ return m_pActorDef; }

protected:
	bool m_bInitialized;

	jeEngine *m_pEngine;
	jeCamera *m_pCamera;

	jeObject *m_pActorObj;
	jeActor_Def *m_pActorDef;
	jeActor *m_pActor;
	jeXForm3d m_ActorXForm;

	jeRect m_CameraRect;
	jeFloat m_FOV;
	jeXForm3d m_CameraXForm;

	jeWorld *m_pWorld;
	jet3d::jeResourceMgr *m_pResMgr;

	wxTimer m_Timer;
	wxPoint m_PrevPos;

	DECLARE_EVENT_TABLE()
};
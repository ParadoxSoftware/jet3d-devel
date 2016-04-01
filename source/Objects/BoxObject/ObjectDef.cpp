/****************************************************************************************/
/*  OBJECTDEF.C                                                                         */
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

#pragma warning ( disable : 4115 )
#include <windows.h>
#pragma warning ( default : 4115 )

#include "jeTypes.h"
#include "BoxObject.h"
#include "Object.h"
#include "ObjectDef.h"
#include "jeVersion.h"

jeObjectDef ObjectDef = {
	JE_OBJECT_TYPE_UNKNOWN,
	"BoxObject",
	0,
	CreateInstance,
	CreateRef,
	Destroy,
	AttachWorld,
	DettachWorld,
	AttachEngine,
	DettachEngine,
	AttachSoundSystem,
	DettachSoundSystem,
	Render,
	Collision,
	GetExtBox,
	CreateFromFile,
	WriteToFile,
	GetPropertyList,
	SetProperty,
	NULL,				// Get Property
	SetXForm,
	GetXForm,
	GetXFormModFlags,
	GetChildren,
	AddChild,
	RemoveChild,
	EditDialog,
	SendMsg,    
	NULL,	  //UpdateTimeDelta
	DuplicateInstance,	  //DuplicateInstance
	ChangeBoxCollision, //Icestorm
	NULL,	// GetGlobalPropertyList
	NULL,	// SetGlobalProperty
	NULL,	//SetRenderNextTime,
};


int WINAPI DllMain(
	HINSTANCE	hInstance,
	DWORD		fdwReason,
	PVOID		pvReserved )
{
	switch( fdwReason )
	{
	case DLL_PROCESS_ATTACH:
		Init_Class( hInstance );
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return( TRUE );
}

DLLExport jeBoolean Object_RegisterDef(float Major, float Minor)
{
	if( JET_MAJOR_VERSION != Major )
		return( JE_FALSE );
	if( JET_MINOR_VERSION != Minor )
		return( JE_FALSE );

	return( jeObject_RegisterGlobalObjectDef(&ObjectDef) );
}


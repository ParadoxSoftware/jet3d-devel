/****************************************************************************************/
/*  TERRAINOBJECTDEF.C                                                                  */
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
#include "Object.h"
#include "jeVersion.h"

#include "Terrain.h"

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////////////
//
//	DLL Entry point
//
////////////////////////////////////////////////////////////////////////////////////////
int WINAPI DllMain(
	HINSTANCE	hInstance,
	DWORD		fdwReason,
	PVOID		pvReserved )
{

	// process launch reason
	switch( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
		{
		//	Init_Class( hInstance );
			break;
		}

		case DLL_THREAD_ATTACH:
		{
			break;
		}

		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		{
		//	DeInit_Class();
			break;
		}
	}

	// all done
	return TRUE;

	// eliminate warnings
	pvReserved;

} // DllMain()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Object_RegisterDef()
//
////////////////////////////////////////////////////////////////////////////////////////
__declspec(dllexport) jeBoolean Object_RegisterDef(
	float Major,	// major version number
	float Minor )	// minor version number
{

	// fail if versions don't match
	if ( ( Major != JET_MAJOR_VERSION ) || ( Minor != JET_MINOR_VERSION ) )
	{
		return JE_FALSE;
	}

	// register def
	return jeTerrain_RegisterObjectDef();

} // Object_RegisterDef()

#ifdef __cplusplus
}
#endif
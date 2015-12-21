/****************************************************************************************/
/*  MAKEHELP.H																			*/
/*                                                                                      */
/*  Author: Jim Mischel		                                                            */
/*  Description: Helper functions for interface between GUI and actor make subsystem.	*/
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
#ifndef MAKEHELP_H
#define MAKEHELP_H

#include "MkUtil.h"
#include "AProject.h"
#include "AOptions.h"

#define MAKEHELP_PROCESSID 0x12345678
// User messages seem to not work unless they're even numbers.
// Is this an MFC thing?
#define WM_USER_COMPILE_DONE (WM_USER + 100)
#define WM_USER_COMPILE_MESSAGE (WM_USER + 104)

void MakeHelp_SetMessagesWindow (CWnd *MsgWindow);
bool MakeHelp_GetRelativePath (const AProject *Project, const char *NewPath, char *Dest);

bool MakeHelp_StartCompile (AProject *Project, AOptions *Options, CWnd *Parent);

void MakeHelp_CancelCompile (void);

extern "C" void MakeHelp_Printf (const char *Fmt, ...);



#endif

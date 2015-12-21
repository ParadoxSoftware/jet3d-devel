/****************************************************************************************/
/*  STDAFX.H                                                                            */
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
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__37F45631_C0E1_11D2_8B41_00104B70D76D__INCLUDED_)
#define AFX_STDAFX_H__37F45631_C0E1_11D2_8B41_00104B70D76D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
//	by trilobite	Jan. 2011
//#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows XP and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
//	by trilobite	Jan. 2011
//#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#define _WIN32_WINNT 0x0501		// Change this to the appropriate value to target Windows XP and Windows 2000 or later.
#endif						

// by trilobite jan, 2011
//uncommenting / revising
#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
//#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#define _WIN32_WINDOWS 0x0510 // Change this to the appropriate value to target Windows xp or later.
#endif
//

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
//	by trilobite	Jan. 2011
//#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#define _WIN32_IE 0x0700	// Change this to the appropriate value to target IE 7.0 or later.
//
#endif



#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "Basetype.h"		// Jet3D base types
#include "Resource.h"


//	tommorris feb 2005
#define _SCB_REPLACE_MINIFRAME
#include "sizecbar.h"
#include "scbarg.h"
#include "scbarcf.h"
#define baseCMyBar CSizingControlBarCF
//	end tommorris feb 2005


//	trilobite jan. 2011
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
//

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__37F45631_C0E1_11D2_8B41_00104B70D76D__INCLUDED_)


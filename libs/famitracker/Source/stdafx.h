/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2012  Jonathan Liss
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

//#define _CRTDBG_MAPALLOC

//// Get rid of warnings in VS 2005
//#define _CRT_SECURE_NO_DEPRECATE
//#define _CRT_SECURE_NO_WARNINGS

//#ifndef VC_EXTRALEAN
//#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
//#endif

//// Modify the following defines if you have to target a platform prior to the ones specified below.
//// Refer to MSDN for the latest info on corresponding values for different platforms.
//#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
//#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
//#endif

//#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
//#define _WIN32_WINNT 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
//#endif						

//#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
//#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
//#endif

//#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
//#define _WIN32_IE 0x0600	// Change this to the appropriate value to target IE 5.0 or later.
//#endif

//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

//// turns off MFC's hiding of some common and often safely ignored warning messages
//#define _AFX_ALL_WARNINGS

//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions

//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <afx.h>
//#include <afxdhtml.h>
//#include <afxdlgs.h>

// Releasing pointers
#define SAFE_RELEASE(p) \
	if (p != NULL) { \
		delete p;	\
		p = NULL;	\
	}	\

#define SAFE_RELEASE_ARRAY(p) \
	if (p != NULL) { \
		delete [] p;	\
		p = NULL;	\
	}	\

//#define new DEBUG_NEW

#include "cqtmfc.h"
#include "resource.h"
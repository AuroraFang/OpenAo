// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__BBFB079B_D1AB_478B_8959_F1395EC891EA__INCLUDED_)
#define AFX_STDAFX_H__BBFB079B_D1AB_478B_8959_F1395EC891EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x0501

#define _SAVE_LOGININFO_ADMINTOOL

#ifdef _SAVE_LOGININFO_ADMINTOOL
#undef _ATUM_ADMIN_RELEASE
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning(disable:4786)


#include "DbgOut_C.h"
#include "DefineGlobal.h"
#include "SystemLogManager.h"
#include "AtumParam.h"
#include "AtumProtocol.h"
#include "StringDefineQuery.h"		// 2008-07-02 by cmkwon, MySQL ���� ���� - 
#include "StringDefineProcedure.h"	// 2008-08-22 by cmkwon, MySQL ����(Procedure �̾Ƴ���) - 

#define SIZE_MAX_SQL_PATTERN_BUFFER			64
extern char* GetSqlPattern(const char* str, char* buf);

#define STR_ALL_SERVER_GROUP_STRING			"All ServerGroup"	// 2008-04-16 by cmkwon, ���� ��� �� ���� ��ȯ �̺�Ʈ �ý��� ���� - SetHappyEv.h �� �ִ����� ����� �̵�, // 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� -	

// 2009-02-04 by cmkwon, AdminTool�� GM ���� ���� - ADSTRMSG ==>AdminToolStringMessage
#define ADSTRMSG_090204_0001				"You don't have permission for this action !!"	// 2009-02-04 by cmkwon, AdminTool�� GM ���� ���� - 

#include "GlobalGameServer.h" // with "GlobalGameServerForAdminTool.cpp

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__BBFB079B_D1AB_478B_8959_F1395EC891EA__INCLUDED_)

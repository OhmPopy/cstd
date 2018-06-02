//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2009 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPControls.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <locale.h>
#include "BCGPControls.h"
#include "BCGControlsPropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPControlsApp

BEGIN_MESSAGE_MAP(CBCGPControlsApp, CWinApp)
	//{{AFX_MSG_MAP(CBCGPControlsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPControlsApp construction

CBCGPControlsApp::CBCGPControlsApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBCGPControlsApp object

CBCGPControlsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBCGPControlsApp initialization

BOOL CBCGPControlsApp::InitInstance()
{
	AfxEnableControlContainer();

	globalData.SetDPIAware ();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER < 1400
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	SetRegistryKey(_T("BCGSoft\\BCGControlBarPro\\Examples"));
	SetRegistryBase (_T("Settings"));

	InitCommonControls();
	InitContextMenuManager ();
	InitShellManager ();

	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
	CBCGPButton::EnableWinXPTheme ();

	if (globalData.m_nBitsPerPixel < 16)
	{
		AfxMessageBox (_T("The current display settings will not allow this application to display itself properly.\r\nPlease change the display color setting to 'Highest (32 bit)' and run this application again"));
	}

	BCGControlsPropSheet propSheet;
	m_pMainWnd = &propSheet;
	propSheet.DoModal();

	BCGCBProCleanUp ();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

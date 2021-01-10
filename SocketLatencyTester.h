
// SocketLatencyTester.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSocketLatencyTesterApp:
// See SocketLatencyTester.cpp for the implementation of this class
//

class CSocketLatencyTesterApp : public CWinApp
{
public:
	CSocketLatencyTesterApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSocketLatencyTesterApp theApp;
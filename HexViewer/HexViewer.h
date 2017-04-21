
// HexViewer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHexViewerApp:
// See HexViewer.cpp for the implementation of this class
//

class CHexViewerApp : public CWinApp
{
public:
	CHexViewerApp();
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

extern CHexViewerApp theApp;
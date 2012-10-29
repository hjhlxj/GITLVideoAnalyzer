
// GITLVideoAnalyser.h : main header file for the GITLVideoAnalyser application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGITLVideoAnalyserApp:
// See GITLVideoAnalyser.cpp for the implementation of this class
//

class CGITLVideoAnalyserApp : public CWinApp
{
public:
	CGITLVideoAnalyserApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGITLVideoAnalyserApp theApp;

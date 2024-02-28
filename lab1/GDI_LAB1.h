
// GDI_LAB1.h : main header file for the GDI_LAB1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGDILAB1App:
// See GDI_LAB1.cpp for the implementation of this class
//

class CGDILAB1App : public CWinApp
{
public:
	CGDILAB1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGDILAB1App theApp;

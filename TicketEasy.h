
// TicketEasy.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTicketEasyApp:
// See TicketEasy.cpp for the implementation of this class
//

class CTicketEasyApp : public CWinApp
{
public:
	CTicketEasyApp();

// Overrides
public:
	virtual int		ExitInstance();
	virtual BOOL	InitInstance();

	BOOL			IsTwiceInstance();

	void			CreateCaptureDirectory(char * usrPath);

	CString			m_strAppDir;
	CString			m_strUsrDir;

protected:
	HANDLE			m_hMutex;

	DECLARE_MESSAGE_MAP()
};

extern CTicketEasyApp theApp;


// TicketEasy.cpp : Defines the class behaviors for the application.
// 02/02/2020

#include "pch.h"
#include "framework.h"
#include "TicketEasy.h"
#include "TicketEasyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTicketEasyApp

BEGIN_MESSAGE_MAP(CTicketEasyApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTicketEasyApp construction

CTicketEasyApp::CTicketEasyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hMutex = NULL;
}


// The one and only CTicketEasyApp object

CTicketEasyApp theApp;


// CTicketEasyApp initialization

BOOL CTicketEasyApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (IsTwiceInstance())
		return FALSE;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("TicketEasy"));

	TCHAR szMod[MAX_PATH];
	memset(szMod, 0, MAX_PATH);
	GetModuleFileName(NULL, szMod, sizeof(szMod));

	TCHAR szDrv[_MAX_DRIVE], szDir[_MAX_DIR];
	memset(szDrv, 0, _MAX_DRIVE);
	memset(szDir, 0, _MAX_DIR);

	_tsplitpath_s(szMod, szDrv, _MAX_DRIVE, szDir, _MAX_DIR, NULL, 0, NULL, 0);
	m_strAppDir.Format(_T("%s%s"), szDrv, szDir);

	char *usrPath;
	size_t len;
	_dupenv_s(&usrPath, &len, "APPDATA");

	CreateCaptureDirectory(usrPath);

	CFrameWnd * pFrame = new CFrameWnd();
	pFrame->Create(0, 0, WS_OVERLAPPEDWINDOW);
#ifdef _DEBUGX
	CTicketEasyDlg dlg;
#else
	CTicketEasyDlg dlg(pFrame);
#endif
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{

	}

	delete pFrame;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CTicketEasyApp::ExitInstance()
{
	if (m_hMutex)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
	}

	return CWinApp::ExitInstance();
}

BOOL CTicketEasyApp::IsTwiceInstance()
{
	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "{6AF8C10C-B131-435C-839A-9865AA901E08}");
	if (m_hMutex != NULL)
		return TRUE;

	m_hMutex = CreateMutex(NULL, FALSE, "{6AF8C10C-B131-435C-839A-9865AA901E08}");

	return FALSE;
}

void CTicketEasyApp::CreateCaptureDirectory(char * usrPath)
{
	m_strUsrDir.Format("%s\\TicketEasy\\*.*", usrPath);

	CFileFind finder;
	BOOL bFound = finder.FindFile((LPCTSTR)m_strUsrDir);
	m_strUsrDir.Format("%s\\TicketEasy", usrPath);

	if (bFound)
		return;

	CreateDirectory((LPCTSTR)m_strUsrDir, NULL);
}
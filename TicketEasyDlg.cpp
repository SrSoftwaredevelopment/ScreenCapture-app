// TicketEasyDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TicketEasy.h"
#include "TicketEasyDlg.h"
#include "DlgMsg.h"
#include <time.h>
#include <locale.h>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTicketEasyDlg dialog
CTicketEasyDlg::CTicketEasyDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TICKETEASY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTicketEasyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_CLOSE,		m_btnClose);
	DDX_Control(pDX, IDC_BUTTON_SUBMIT,		m_btnSubmit);
	DDX_Control(pDX, IDC_EDIT_CONTENT,		m_edtCont);
	DDX_Control(pDX, IDC_EDIT_TITLE,		m_edtTitle);
}	

BEGIN_MESSAGE_MAP(CTicketEasyDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE,			OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SUBMIT,		OnBnClickedButtonSubmit)
	ON_COMMAND(ID_POPUP_EXIT,				OnPopupExit)
	ON_COMMAND(ID_POPUP_SHOWWINDOW,			OnPopupShowwindow)
END_MESSAGE_MAP()

// ------------------------------------------------
// The following code declares our AnchorMap
// and defines how each control should be handled
// ------------------------------------------------
BEGIN_ANCHOR_MAP(CTicketEasyDlg)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_CLOSE,		ANF_TOPRIGHT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_RIGHT,		ANF_TOPRIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_CONTENT,		ANF_TOPLEFT | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_TITLE,		ANF_TOPLEFT | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_OLD,		ANF_BOTTOMLEFT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_STARTTIME,	ANF_BOTTOMLEFT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_NEW,		ANF_BOTTOMRIGHT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_ENDTIME,	ANF_BOTTOMRIGHT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_BOTTOM,		ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_SUBMIT,		ANF_BOTTOMRIGHT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_IMGPANEL,	ANF_BOTTOMLEFT | ANF_RIGHT)
END_ANCHOR_MAP()

// CTicketEasyDlg message handlers
BOOL CTicketEasyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

#ifdef MODAL
	InitAnchors(ANIF_CALCSIZE);
	Initialize();
	ShowWindow(SW_MAXIMIZE);
#else
	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	InitAnchors(ANIF_CALCSIZE);
	Initialize();
	ShowWindow(SW_MAXIMIZE);

	m_hook.InstallHook(m_hWnd);

	//SetParent(GetDesktopWindow());
	//CRect rect;
	//GetDesktopWindow()->GetWindowRect(&rect);
	//SetWindowPos(&wndTopMost, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

	PostMessage(WM_HIDEWINDOW);
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTicketEasyDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if (dc.GetSafeHdc() == NULL)
			return;

		CRect rt;
		GetClientRect(rt);

		CBitmap bmp;
		if (!bmp.CreateCompatibleBitmap(&dc, rt.Width(), rt.Height()))
			return;

		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);

		CDC dcMemory;
		dcMemory.CreateCompatibleDC(&dc);
		CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

		HDC hDC = dcMemory.GetSafeHdc();
		m_imgBk.Draw(hDC, rt);

		rt.left += 15;
		rt.top += 15;
		rt.right = rt.left + m_imgLogo.GetWidth();
		rt.bottom = rt.top + m_imgLogo.GetHeight();
		m_imgLogo.Draw(hDC, rt);

		CBrush brush, *pOldBrush;
		CPen pen, *pOldPen;

		brush.CreateSolidBrush(RGB(248, 248, 248));
		pOldBrush = dcMemory.SelectObject(&brush);

		pen.CreatePen(PS_SOLID, 2, RGB(248, 248, 248));
		pOldPen = dcMemory.SelectObject(&pen);

		// setup arrows
		ARROWSTRUCT a;
		a.nWidth = 6;
		a.fTheta = 0.4f;
		a.bFill = true;

		GetClientRect(rt);

		dcMemory.MoveTo(150, rt.bottom - 80);
		ArrowTo(&dcMemory, rt.right - 150, rt.bottom - 80, &a);

		dcMemory.SelectObject(pOldBrush);
		brush.DeleteObject();

		dcMemory.SelectObject(pOldPen);
		pen.DeleteObject();

		dc.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);
		dcMemory.SelectObject(pOldBitmap);

		bmp.DeleteObject();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTicketEasyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CTicketEasyDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd * pWnd = GetDlgItem(IDC_EDIT_CONTENT);
			if (pMsg->hwnd != pWnd->GetSafeHwnd())
				return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CTicketEasyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_hWnd != NULL)
	{
		CRect rt;
		GetWindowRect(&rt);
		HandleAnchors(&rt);      // you can alternatively pass NULL for &rcWnd

		RelayoutCapturePanels();
	}
}

void CTicketEasyDlg::RelayoutCapturePanels()
{
	CWnd * pWnd = GetDlgItem(IDC_STATIC_IMGPANEL);
	if (pWnd == NULL)
		return;

	CRect rt;
	pWnd->GetClientRect(&rt);

	int w = (rt.Width() - 21) / CAPTURECOUNT;
	int h = rt.Height() - 17;

	for (int i = 0; i < CAPTURECOUNT; i++)
	{
		pWnd->GetClientRect(&rt);

		rt.left = rt.left + m_nX * (i + 1) + (i * w) + 2;
		rt.right = rt.left + w;
		rt.top = rt.top + m_nY;
		rt.bottom = rt.top + h;

		if (m_imgPanel[i].m_hWnd != NULL)
			m_imgPanel[i].MoveWindow(&rt);
	}

	Invalidate();
}

void CTicketEasyDlg::Initialize()
{
	m_nX = 3;
	m_nY = 12;

	CFont * pFont = GetFont();
	if (pFont)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		pFont->GetLogFont(&lf);
		m_bigFont.CreateFont(24, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, lf.lfFaceName);
		m_midFont.CreateFont(24, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, lf.lfFaceName);
		m_smallFont.CreateFont(14, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, lf.lfFaceName);
	}
	else
	{
		m_bigFont.CreateFont(24, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Tahoma");
		m_midFont.CreateFont(14, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Tahoma");
		m_smallFont.CreateFont(14, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Tahoma");
	}
	
	CString strImgFile;
	strImgFile.Format(_T("%sResources\\background2.png"), theApp.m_strAppDir);
	m_imgBk.Load((LPCTSTR)strImgFile, CXIMAGE_FORMAT_PNG);

	strImgFile.Format(_T("%sResources\\Logo.png"), theApp.m_strAppDir);
	m_imgLogo.Load((LPCTSTR)strImgFile, CXIMAGE_FORMAT_PNG);

	strImgFile.Format(_T("%sResources\\Logo.png"), theApp.m_strAppDir);
	m_imgLogo.Load((LPCTSTR)strImgFile, CXIMAGE_FORMAT_PNG);

	strImgFile.Format(_T("%sResources\\exit_hover.png"), theApp.m_strAppDir);
	m_imgButtons[0].Load((LPCTSTR)strImgFile, CXIMAGE_FORMAT_PNG);

	strImgFile.Format(_T("%sResources\\exit.png"), theApp.m_strAppDir);
	m_imgButtons[1].Load((LPCTSTR)strImgFile, CXIMAGE_FORMAT_PNG);

	// Create the tray icon
	CString strTitle;
	strTitle.LoadString(IDS_STRING_TITLE);
	m_TrayIcon.Create(NULL, WM_ICON_NOTIFY, (LPCTSTR)strTitle, m_hIcon, IDR_MENU_TRAY, FALSE, NULL, NULL, NIIF_WARNING, 20);

	GetDlgItem(IDC_STATIC_OLD)->SetFont(&m_midFont);
	GetDlgItem(IDC_STATIC_NEW)->SetFont(&m_midFont);
	GetDlgItem(IDC_STATIC_STARTTIME)->SetFont(&m_smallFont);
	GetDlgItem(IDC_STATIC_ENDTIME)->SetFont(&m_smallFont);

	m_btnSubmit.SetSkin(IDB_BITMAP_NORMAL, IDB_BITMAP_HOVER, IDB_BITMAP_HOVER, 0, 0, 0, 1, 0, 0);
	m_btnSubmit.SetToolTipText("Submit"); 
	m_btnSubmit.SetFont(&m_bigFont);

	HBITMAP hBmpN = m_imgButtons[0].MakeBitmap(NULL, true);
	HBITMAP hBmpH = m_imgButtons[1].MakeBitmap(NULL, true);
	m_btnClose.SetSkin(hBmpH, hBmpN, hBmpN, 0, 0, 0, 1, 0, 0);
	m_btnClose.SetToolTipText("Close");
	m_btnClose.SetFont(&m_bigFont); 

	m_edtTitle.SetPromptShowMode(pmGray);
	m_edtTitle.SetPromptText("Please give your ticket a title");
	m_edtTitle.SetFont(&m_midFont);

	m_edtCont.SetPromptShowMode(pmGray);
	m_edtCont.SetPromptText("Let us know what the issue is");
	m_edtCont.SetFont(&m_midFont);

	CWnd * pChildView = GetDlgItem(IDC_STATIC_IMGPANEL);

	strImgFile.Format(_T("%sResources\\Logo.png"), theApp.m_strAppDir);
	for (int i = 0; i < CAPTURECOUNT; i++)
	{
		m_imgPanel[i].Create(IDD_DIALOG_THUMB, pChildView);
		m_imgPanel[i].ShowWindow(SW_SHOW);
		m_imgPanel[i].SetImage((LPCTSTR)strImgFile, bltCenter);
	}

	m_dwProcId = 0xFFFFFFFF;
	GetWindowThreadProcessId(m_hWnd, &m_dwProcId);

	ArrangeScreenshots();
}

void CTicketEasyDlg::OnPopupShowwindow()
{
	ShowWindow(SW_SHOW);
}

void CTicketEasyDlg::OnPopupExit()
{
	OnCancel();
}

void CTicketEasyDlg::OnBnClickedButtonClose()
{
#ifdef MODAL
	OnCancel();
#else
	ShowWindow(SW_HIDE);
#endif
}

void CTicketEasyDlg::OnDestroy()
{
	CDialog::OnDestroy();

#ifndef _DEBUG
	m_hook.UninstallHook();
#endif
}

void CTicketEasyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
#ifndef _DEBUG
	if (nID == SC_CLOSE || nID == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
		return;
	}
#endif

	CDialog::OnSysCommand(nID, lParam);
}

LRESULT CTicketEasyDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_HIDEWINDOW)
	{
		ShowWindow(SW_HIDE);
		return TRUE;
	}

	if (message == WM_USER_KEYBDHOOK)
	{
		ShowWindow(SW_SHOW);
		SetForegroundWindow();
		SetActiveWindow();
		BringWindowToTop();
		return TRUE;
	}

	if (message == WM_USER_MOUSEHOOK)
	{
		DoScreenCapture();
		return TRUE;
	}

	if (message == WM_USER_SENDCAPTURE)
	{
		ArrangeScreenshots();
		return TRUE;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


HBRUSH CTicketEasyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int nId = pWnd->GetDlgCtrlID();
	if (nId == IDC_STATIC_OLD || nId == IDC_STATIC_NEW || nId == IDC_STATIC_IMGPANEL || nId == IDC_STATIC_ARROW || nId == IDC_STATIC_STARTTIME || nId == IDC_STATIC_ENDTIME)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(224, 224, 224));
	}

	return hbr;
}

// ArrowTo()
//
void CTicketEasyDlg::ArrowTo(CDC * pDC, int x, int y, ARROWSTRUCT *pA)
{
	POINT ptTo = { x, y };
	ArrowTo(pDC, &ptTo, pA);
}

// ArrowTo()
//
void CTicketEasyDlg::ArrowTo(CDC * pDC, const POINT *lpTo, ARROWSTRUCT *pA) 
{
	POINT pFrom;
	POINT pBase;
	POINT aptPoly[3];
	float vecLine[2];
	float vecLeft[2];
	float fLength;
	float th;
	float ta;

	// get from point
	::MoveToEx(pDC->GetSafeHdc(), 0, 0, &pFrom);

	// set to point
	aptPoly[0].x = lpTo->x;
	aptPoly[0].y = lpTo->y;

	// build the line vector
	vecLine[0] = (float)aptPoly[0].x - pFrom.x;
	vecLine[1] = (float)aptPoly[0].y - pFrom.y;

	// build the arrow base vector - normal to the line
	vecLeft[0] = -vecLine[1];
	vecLeft[1] = vecLine[0];

	// setup length parameters
	fLength = (float)sqrt(vecLine[0] * vecLine[0] + vecLine[1] * vecLine[1]);
	th = pA->nWidth / (2.0f * fLength);
	ta = pA->nWidth / (2.0f * (tanf(pA->fTheta) / 2.0f) * fLength);

	// find the base of the arrow
	pBase.x = (int)(aptPoly[0].x + -ta * vecLine[0]);
	pBase.y = (int)(aptPoly[0].y + -ta * vecLine[1]);

	// build the points on the sides of the arrow
	aptPoly[1].x = (int)(pBase.x + th * vecLeft[0]);
	aptPoly[1].y = (int)(pBase.y + th * vecLeft[1]);
	aptPoly[2].x = (int)(pBase.x + -th * vecLeft[0]);
	aptPoly[2].y = (int)(pBase.y + -th * vecLeft[1]);

	::MoveToEx(pDC->GetSafeHdc(), pFrom.x, pFrom.y, NULL);

	// draw we're fillin'...
	if (pA->bFill) 
	{
		pDC->LineTo(aptPoly[0].x, aptPoly[0].y);
		pDC->Polygon(aptPoly, 3);
	}
	else {
		pDC->LineTo(pBase.x, pBase.y);
		pDC->LineTo(aptPoly[1].x, aptPoly[1].y);
		pDC->LineTo(aptPoly[0].x, aptPoly[0].y);
		pDC->LineTo(aptPoly[2].x, aptPoly[2].y);
		pDC->LineTo(pBase.x, pBase.y);
		::MoveToEx(pDC->GetSafeHdc(), aptPoly[0].x, aptPoly[0].y, NULL);
	}
}

void CTicketEasyDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 1150;
	lpMMI->ptMinTrackSize.y = 780;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CTicketEasyDlg::UnixTimeToFileTime(time_t t, LPFILETIME pft)
{
	// Note that LONGLONG is a 64-bit value
	LONGLONG ll;

	ll = Int32x32To64(t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = ll >> 32;
}

void CTicketEasyDlg::UnixTimeToSystemTime(time_t t, LPSYSTEMTIME pst)
{
	FILETIME ft;

	UnixTimeToFileTime(t, &ft);
	FileTimeToSystemTime(&ft, pst);
}

void CTicketEasyDlg::DoScreenCapture()
{
	HWND hWnd = ::GetDesktopWindow();
	if (hWnd == m_hWnd)
		return;

	COleDateTime tmNow = COleDateTime::GetCurrentTime();
	COleDateTime tmBase(1970, 1, 1, 0, 0, 0);
	COleDateTimeSpan tmSpan = tmNow - tmBase;

	LONG lTotalSec = tmSpan.GetTotalSeconds();

	CString strFile, strThum;
	strFile.Format("%s\\Capture_%u.jpg", theApp.m_strUsrDir, lTotalSec);
	strThum.Format("%s\\Thum_%u.jpg", theApp.m_strUsrDir, lTotalSec);

 	HDC hdc = ::GetWindowDC(hWnd);
 
 	CDC dc;
	dc.Attach(hdc);
 
 	CDC memDC;
 	memDC.CreateCompatibleDC(&dc);
 
 	CRect r;
 	::GetWindowRect(hWnd, &r);

 	CBitmap bm;
	bm.CreateCompatibleBitmap(&dc, r.Width(), r.Height());

	CBitmap * oldbm = memDC.SelectObject(&bm);
	memDC.BitBlt(0, 0, r.Width(), r.Height(), &dc, 0, 0, SRCCOPY);

	m_imgCapt.CreateFromHBITMAP((HBITMAP)bm.m_hObject);
	m_imgCapt.SetJpegQuality(50);
	m_imgCapt.Save(strFile, CXIMAGE_FORMAT_JPG);

	m_imgCapt.Resample2(r.Width() / 5, r.Height() / 5);
	m_imgCapt.Save(strThum, CXIMAGE_FORMAT_JPG);
	m_imgCapt.Destroy();

	memDC.SelectObject(oldbm);
	bm.DeleteObject();
	memDC.DeleteDC();

	dc.Detach();
	dc.DeleteDC();

	::ReleaseDC(hWnd, hdc); 

	PostMessage(WM_USER_SENDCAPTURE);
}

BOOL CTicketEasyDlg::IsExistScreenshot(LONG lCaptTime)
{
	BOOL bExist = FALSE;

	int nCount = m_vCaptures.size();
	for (int i = 0; i < nCount; i++)
	{
		if (lCaptTime == m_vCaptures.at(i))
		{
			bExist = TRUE;
			break;
		}
	}

	return bExist;
}

void CTicketEasyDlg::ArrangeScreenshots()
{
	CString strFile, strThum;
	strFile.Format("%s\\*.jpg", theApp.m_strUsrDir);

	CFileFind finder;
	BOOL bFound = finder.FindFile((LPCTSTR)strFile);

	m_vCaptures.clear();

	while (bFound)
	{
		bFound = finder.FindNextFileA();

		if (finder.IsDots() || finder.IsDirectory())
			continue;

		CString strName = finder.GetFileTitle();
		if (strName.Find("Capture_") == -1)
			continue;

		CString strTime = strName.Mid(8);
		LONG lTotalSec = atol((LPCTSTR)strTime);

		if (!IsExistScreenshot(lTotalSec))
			m_vCaptures.push_back(lTotalSec);
	}

	int nCount = m_vCaptures.size();
	if (nCount == 0)
	{
		GetDlgItem(IDC_STATIC_STARTTIME)->SetWindowText("00:00:00");
		GetDlgItem(IDC_STATIC_ENDTIME)->SetWindowText("00:00:00");
		return;
	}

	//sort(m_vCaptures.begin(), m_vCaptures.end(), greater<>());
	sort(m_vCaptures.begin(), m_vCaptures.end());

	if (nCount > CAPTURECOUNT)
	{
		int nDiff = nCount - CAPTURECOUNT;
		for (int i = 0; i < nDiff; i ++)
		{
			LONG lTime = m_vCaptures.at(i);
			strFile.Format("%s\\Capture_%u.jpg", theApp.m_strUsrDir, lTime);
			strThum.Format("%s\\Thum_%u.jpg", theApp.m_strUsrDir, lTime);

			DeleteFile((LPCTSTR)strFile);
			DeleteFile((LPCTSTR)strThum);
		}

		m_vCaptures.erase(m_vCaptures.begin(), m_vCaptures.begin() + nDiff);
		nCount = m_vCaptures.size();
	}

	for (int i = 0; i < CAPTURECOUNT; i++)
	{
		if (i < nCount)
		{
			LONG lTime = m_vCaptures.at(i);
			//strFile.Format("%s\\Capture_%u.jpg", theApp.m_strUsrDir, lTime);
			strFile.Format("%s\\Thum_%u.jpg", theApp.m_strUsrDir, lTime);
			m_imgPanel[i].SetImage((LPCTSTR)strFile, bltStretch);
		}
		else
		{
			strFile.Format(_T("%sResources\\Logo.png"), theApp.m_strAppDir);
			m_imgPanel[i].SetImage((LPCTSTR)strFile, bltCenter);
		}
	}

	LONG lStart = m_vCaptures.at(0);
	LONG lEnd = m_vCaptures.at(nCount - 1);

	COleDateTime tmBase(1970, 1, 1, 0, 0, 0);
	COleDateTimeSpan tmSpan1(0, 0, 0, lStart);
	COleDateTimeSpan tmSpan2(0, 0, 0, lEnd);

	COleDateTime tmStart = tmBase + tmSpan1;
	COleDateTime tmEnd = tmBase + tmSpan2;

	CString strTime;
	strTime.Format("%d:%02d:%02d", tmStart.GetHour(), tmStart.GetMinute(), tmStart.GetSecond());
	GetDlgItem(IDC_STATIC_STARTTIME)->SetWindowText((LPCTSTR)strTime);

	strTime.Format("%d:%02d:%02d", tmEnd.GetHour(), tmEnd.GetMinute(), tmEnd.GetSecond());
	GetDlgItem(IDC_STATIC_ENDTIME)->SetWindowText((LPCTSTR)strTime);

	Invalidate();
}

BOOL CTicketEasyDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


BOOL CTicketEasyDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_BUTTON));
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return TRUE;
}

void CTicketEasyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	POINT p2 = point;
	::ClientToScreen(m_hWnd, &p2);
	HWND mouse_wnd = ::WindowFromPoint(p2);
	if (mouse_wnd == m_hWnd)
	{
		for (int i = 0; i < CAPTURECOUNT; i++)
			m_imgPanel[i].ShowButtons(FALSE);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CTicketEasyDlg::OnBnClickedButtonSubmit()
{

}

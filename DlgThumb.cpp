// DlgThumb.cpp : implementation file
//
#include "pch.h"
#include "TicketEasy.h"
#include "DlgThumb.h"
#include "DlgImgEdit.h"
#include "TicketEasyDlg.h"

// CDlgThumb dialog
CDlgThumb::CDlgThumb(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_THUMB, pParent)
{
	m_nBltMode = bltCenter;
}

CDlgThumb::~CDlgThumb()
{
	m_img.Destroy();
}

void CDlgThumb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_DELETE,		m_btnDel);
	DDX_Control(pDX, IDC_BUTTON_EDIT,		m_btnEdt);
}


BEGIN_MESSAGE_MAP(CDlgThumb, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_DELETE,		OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_EDIT,			OnBnClickedButtonEdit)
END_MESSAGE_MAP()

// ------------------------------------------------
// The following code declares our AnchorMap
// and defines how each control should be handled
// ------------------------------------------------
BEGIN_ANCHOR_MAP(CDlgThumb)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_EDIT,		ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_DELETE,		ANF_TOP)
END_ANCHOR_MAP()

// CDlgThumb message handlers
BOOL CDlgThumb::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgThumb::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitAnchors(ANIF_CALCSIZE);

	CFont * pFont = GetFont();
	if (pFont)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		pFont->GetLogFont(&lf);
		m_bigFont.CreateFont(18, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, lf.lfFaceName);
	}
	else
	{
		m_bigFont.CreateFont(18, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Tahoma");
	}

	m_btnDel.SetSkin(IDB_BITMAP_HOVER2, IDB_BITMAP_NORMAL2, IDB_BITMAP_NORMAL2, 0, 0, 0, 1, 0, 0, true);
	m_btnDel.SetToolTipText("Delete");
	m_btnDel.SetFont(&m_bigFont);

	m_btnEdt.SetSkin(IDB_BITMAP_HOVER2, IDB_BITMAP_NORMAL2, IDB_BITMAP_NORMAL2, 0, 0, 0, 1, 0, 0, true);
	m_btnEdt.SetToolTipText("Edit");
	m_btnEdt.SetFont(&m_bigFont);

	if (m_nBltMode != bltCenter)
		ShowButtons(TRUE);
	else
		ShowButtons(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgThumb::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_hWnd != NULL)
	{
		CRect rt;
		GetWindowRect(&rt);
		HandleAnchors(&rt);      // you can alternatively pass NULL for &rcWnd
	}
}

void CDlgThumb::SetImage(LPCTSTR lpszFile, int nBlt)
{
	m_nBltMode = nBlt;
	m_strImgFile = lpszFile;
	m_img.Load((LPCTSTR)m_strImgFile, CXIMAGE_FORMAT_JPG);
	Invalidate();
}

void CDlgThumb::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages

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

	if (m_nBltMode == bltStretch)
	{
		rt.InflateRect(-1, -1, -1, -1);
		m_img.Draw(hDC, rt);
	}
	else if (m_nBltMode == bltCenter)
	{
		rt.InflateRect(-1, -1, -1, -1);
		dcMemory.FillSolidRect(&rt, RGB(0, 0, 0));
		int w = m_img.GetWidth() - 30;
		int h = m_img.GetHeight() - 10;
		int x = rt.left + (rt.Width() - w) / 2 + 3;
		int y = rt.top + (rt.Height() - h) / 2 + 10;
		m_img.Stretch(dcMemory.GetSafeHdc(), x, y, w, h);
	}
	else
	{
		rt.InflateRect(-1, -1, -1, -1);
		dcMemory.FillSolidRect(&rt, RGB(0, 0, 0));
		int x = rt.left;
		int y = rt.top;
		m_img.Blt(dcMemory.GetSafeHdc(), x, y);
	}

	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, 1, RGB(224, 224, 224));

	pOldPen = dcMemory.SelectObject(&pen);

	dcMemory.MoveTo(rt.left, rt.top);
	dcMemory.LineTo(rt.left, rt.bottom);
	dcMemory.LineTo(rt.right, rt.bottom);
	dcMemory.LineTo(rt.right, rt.top);
	dcMemory.LineTo(rt.left, rt.top);

	dcMemory.SelectObject(pOldPen);
	pen.DeleteObject();

	dc.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);

	bmp.DeleteObject();
}

void CDlgThumb::OnBnClickedButtonEdit()
{
	// TODO: Add your control notification handler code here
	CString strFile = m_strImgFile;
	char szDrv[_MAX_DRIVE], szDir[_MAX_DIR], szName[_MAX_FNAME], szExt[_MAX_EXT];
	memset(szDrv, 0, _MAX_DRIVE);
	memset(szDir, 0, _MAX_DIR);
	memset(szName, 0, _MAX_FNAME);
	memset(szExt, 0, _MAX_EXT);
	_tsplitpath_s((LPCTSTR)strFile, szDrv, _MAX_DRIVE, szDir, _MAX_DIR, szName, _MAX_FNAME, szExt, _MAX_EXT);

	CString strName = szName;
	strName.Replace("Thum_", "Capture_");
	strFile.Format("%s%s%s%s", szDrv, szDir, strName, szExt);

	CDlgImgEdit dlg;
	dlg.SetCaptureImage((LPCTSTR)strFile);
	int nRet = dlg.DoModal();
	if (nRet != IDOK)
		return;

	CTicketEasyDlg * pDlg = (CTicketEasyDlg *)AfxGetMainWnd();
	if (pDlg)
		pDlg->ArrangeScreenshots();
}


void CDlgThumb::OnBnClickedButtonDelete()
{
	int nRet = MyMessageBox("Do you want to delete this image?", MBYESNO);
	if (nRet != IDOK)
		return;

	CString strFile = m_strImgFile;
	
	DeleteFile((LPCTSTR)strFile);

	char szDrv[_MAX_DRIVE], szDir[_MAX_DIR], szName[_MAX_FNAME], szExt[_MAX_EXT];
	memset(szDrv, 0, _MAX_DRIVE);
	memset(szDir, 0, _MAX_DIR);
	memset(szName, 0, _MAX_FNAME);
	memset(szExt, 0, _MAX_EXT);
	_tsplitpath_s((LPCTSTR)strFile, szDrv, _MAX_DRIVE, szDir, _MAX_DIR, szName, _MAX_FNAME, szExt, _MAX_EXT);

	CString strName = szName;
	strName.Replace("Thum_", "Capture_");
	strFile.Format("%s%s%s%s", szDrv, szDir, strName, szExt);
	DeleteFile((LPCTSTR)strFile);

	CTicketEasyDlg * pDlg = (CTicketEasyDlg *)AfxGetMainWnd();
	if (pDlg)
		pDlg->ArrangeScreenshots();
}

void CDlgThumb::OnMouseMove(UINT nFlags, CPoint point)
{
	POINT p2 = point;
	::ClientToScreen(m_hWnd, &p2);
	HWND mouse_wnd = ::WindowFromPoint(p2); 
	if (mouse_wnd == m_hWnd && m_nBltMode == bltStretch) // if (m_nBltMode == bltStretch)
		ShowButtons(TRUE);
	else
		ShowButtons(FALSE);

	CDialog::OnMouseMove(nFlags, point);
}

void CDlgThumb::ShowButtons(BOOL bShow)
{
	if (bShow)
	{
		m_btnEdt.ShowWindow(SW_SHOW);
		m_btnDel.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnEdt.ShowWindow(SW_HIDE);
		m_btnDel.ShowWindow(SW_HIDE);
	}
}

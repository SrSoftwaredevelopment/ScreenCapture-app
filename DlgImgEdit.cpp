// DlgImgEdit.cpp : implementation file
//

#include "pch.h"
#include "TicketEasy.h"
#include "DlgImgEdit.h"

// CDlgImgEdit dialog
CDlgImgEdit::CDlgImgEdit(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_EDITOR, pParent)
{
	m_nDraw = DRAWUNDO;
}

CDlgImgEdit::~CDlgImgEdit()
{
}

void CDlgImgEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_BLUR,		m_btnBlur);
	DDX_Control(pDX, IDC_CHECK_CROP,		m_btnCrop);
	DDX_Control(pDX, IDC_BUTTON_CLOSE,		m_btnClose);
	DDX_Control(pDX, IDC_BUTTON_EXIT,		m_btnExit);
	DDX_Control(pDX, IDC_BUTTON_SAVE,		m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_UNDO,		m_btnUndo);
	DDX_Control(pDX, IDC_STATIC_IMG,		m_imgPanel);
}


BEGIN_MESSAGE_MAP(CDlgImgEdit, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE,			OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_EXIT,			OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_SAVE,			OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_CHECK_BLUR,			OnBnClickedCheckBlur)
	ON_BN_CLICKED(IDC_CHECK_CROP,			OnBnClickedCheckCrop)
	ON_BN_CLICKED(IDC_BUTTON_UNDO,			OnBnClickedButtonUndo)
END_MESSAGE_MAP()

// ------------------------------------------------
// The following code declares our AnchorMap
// and defines how each control should be handled
// ------------------------------------------------
BEGIN_ANCHOR_MAP(CDlgImgEdit)
	ANCHOR_MAP_ENTRY(IDC_STATIC_RIGHT,		ANF_TOPRIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_CLOSE,		ANF_TOPRIGHT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_IMG,		ANF_TOPLEFT | ANF_BOTTOMRIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_EXIT,		ANF_BOTTOMRIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_SAVE,		ANF_BOTTOMRIGHT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_BOTTOM,		ANF_BOTTOMRIGHT)
END_ANCHOR_MAP()


// CDlgImgEdit message handlers
BOOL CDlgImgEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
#ifdef MODAL
	InitAnchors(ANIF_CALCSIZE);
	Initialize();
	ShowWindow(SW_MAXIMIZE);
#else
	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	InitAnchors(ANIF_CALCSIZE);
	Initialize();
	ShowWindow(SW_MAXIMIZE);

	//SetParent(GetDesktopWindow());
	//CRect rect;
	//GetDesktopWindow()->GetWindowRect(&rect);
	//SetWindowPos(&wndTopMost, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImgEdit::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_hWnd != NULL)
	{
		CRect rt;
		GetWindowRect(&rt);
		HandleAnchors(&rt);      // you can alternatively pass NULL for &rcWnd
		Invalidate();
	}
}

void CDlgImgEdit::Initialize()
{
	CFont * pFont = GetFont();
	if (pFont)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		pFont->GetLogFont(&lf);
		m_bigFont.CreateFont(24, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, lf.lfFaceName);
	}
	else
	{
		m_bigFont.CreateFont(24, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Tahoma");
	}

	CString strImgFile;
	strImgFile.Format(_T("%sResources\\Logo.png"), theApp.m_strAppDir);
	m_imgLogo.Load((LPCTSTR)strImgFile, CXIMAGE_FORMAT_PNG);
	   
	m_btnExit.SetSkin(IDB_BITMAP_NORMAL, IDB_BITMAP_HOVER, IDB_BITMAP_HOVER, 0, 0, 0, 1, 0, 0);
	m_btnExit.SetToolTipText("Exit");
	m_btnExit.SetFont(&m_bigFont);

	m_btnSave.SetSkin(IDB_BITMAP_NORMAL, IDB_BITMAP_HOVER, IDB_BITMAP_HOVER, 0, 0, 0, 1, 0, 0);
	m_btnSave.SetToolTipText("Save");
	m_btnSave.SetFont(&m_bigFont);

	SkinButton(m_btnBlur,  "blur", "Blur");
	SkinButton(m_btnCrop,  "crop", "Crop");
	SkinButton(m_btnClose, "exit", "Close");
	SkinButton(m_btnUndo,  "undo", "Undo");

	m_imgPanel.SetImgFile(m_strImgPath);
	m_imgPanel.SetBltMode(bltStretch);

	OnBnClickedCheckCrop();
}

void CDlgImgEdit::SkinButton(CxSkinButton& button, LPCTSTR lpszTok, LPCTSTR lpszTooltip)
{
	CxImage	imgBtns[2];

	CString strImgFile;
	strImgFile.Format(_T("%sResources\\%s_hover.png"), theApp.m_strAppDir, lpszTok);
	imgBtns[0].Load((LPCTSTR)strImgFile, CXIMAGE_FORMAT_PNG);

	strImgFile.Format(_T("%sResources\\%s.png"), theApp.m_strAppDir, lpszTok);
	imgBtns[1].Load((LPCTSTR)strImgFile, CXIMAGE_FORMAT_PNG);

	HBITMAP hBmpN = imgBtns[0].MakeBitmap(NULL, true);
	HBITMAP hBmpH = imgBtns[1].MakeBitmap(NULL, true);
	button.SetSkin(hBmpH, hBmpN, hBmpN, 0, 0, 0, 1, 0, 0);
	button.SetToolTipText(lpszTooltip);
}

void CDlgImgEdit::OnPaint()
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

	CBrush brush, *pOldBrush;
	brush.CreateSolidBrush(RGB(0, 0, 0));
	pOldBrush = dcMemory.SelectObject(&brush);

	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, 2, RGB(248, 248, 248));
	pOldPen = dcMemory.SelectObject(&pen);

	HDC hDC = dcMemory.GetSafeHdc();
	
	rt.left += 15;
	rt.top += 15;
	rt.right = rt.left + m_imgLogo.GetWidth();
	rt.bottom = rt.top + m_imgLogo.GetHeight();
	m_imgLogo.Draw(hDC, rt);

	m_imgPanel.GetWindowRect(&rt);
	ScreenToClient(&rt);

	dcMemory.MoveTo(rt.left, rt.top);
	dcMemory.LineTo(rt.left, rt.bottom);
	dcMemory.LineTo(rt.right, rt.bottom);
	dcMemory.LineTo(rt.right, rt.top);
	dcMemory.LineTo(rt.left, rt.top);

	dcMemory.SelectObject(pOldBrush);
	brush.DeleteObject();

	dcMemory.SelectObject(pOldPen);
	pen.DeleteObject();

	dc.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);

	bmp.DeleteObject();
}

void CDlgImgEdit::OnBnClickedButtonClose()
{
	OnBnClickedButtonExit();
}

void CDlgImgEdit::OnBnClickedButtonSave()
{
	m_imgPanel.GetImage()->Save(m_strImgPath, CXIMAGE_FORMAT_JPG);

	CString strThumbPath = m_strImgPath;
	strThumbPath.Replace("\\Capture_", "\\Thum_");
	m_imgPanel.GetImage()->Save(strThumbPath, CXIMAGE_FORMAT_JPG);

	CDialog::OnOK();
}

void CDlgImgEdit::OnBnClickedButtonExit()
{
	int nRet = MyMessageBox("Do you want to save the image?", MBYESNO);
	if (nRet == IDOK)
	{
		OnBnClickedButtonSave();
	}
	else
		CDialog::OnCancel();
}


BOOL CDlgImgEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgImgEdit::OnBnClickedCheckCrop()
{
	m_nDraw = DRAWCROP;
	((CButton *)GetDlgItem(IDC_CHECK_CROP))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK_BLUR))->SetCheck(FALSE);
	m_imgPanel.SetDrawMode(m_nDraw);
}

void CDlgImgEdit::OnBnClickedCheckBlur()
{
	m_nDraw = DRAWBLUR;
	((CButton *)GetDlgItem(IDC_CHECK_CROP))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_CHECK_BLUR))->SetCheck(TRUE);
	m_imgPanel.SetDrawMode(m_nDraw);
}

void CDlgImgEdit::OnBnClickedButtonUndo()
{

}

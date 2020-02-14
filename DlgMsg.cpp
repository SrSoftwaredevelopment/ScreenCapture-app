// DlgMsg.cpp : implementation file
//
#include "pch.h"
#include "TicketEasy.h"
#include "DlgMsg.h"


// CDlgMsg dialog
IMPLEMENT_DYNAMIC(CDlgMsg, CDialogSK)

CDlgMsg::CDlgMsg(CWnd* pParent /*=nullptr*/)
	: CDialogSK(IDD_DIALOG_MSG, pParent)
{

}

CDlgMsg::~CDlgMsg()
{
}

void CDlgMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_OK,		m_btnOk);
	DDX_Control(pDX, IDC_BUTTON_YES,	m_btnYes);
	DDX_Control(pDX, IDC_BUTTON_NO,		m_btnNo);
	DDX_Control(pDX, IDC_STATIC_MSG,	m_msg);
}


BEGIN_MESSAGE_MAP(CDlgMsg, CDialogSK)
	ON_BN_CLICKED(IDC_BUTTON_YES,		OnBnClickedButtonYes)
	ON_BN_CLICKED(IDC_BUTTON_NO,		OnBnClickedButtonNo)
	ON_BN_CLICKED(IDC_BUTTON_OK,		OnBnClickedButtonOk)
END_MESSAGE_MAP()


// CDlgMsg message handlers
BOOL CDlgMsg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

	SetBitmap(IDB_BITMAP_MSGBK);           // set background bitmap
	SetStyle(LO_STRETCH);                 // resize dialog to the size of the bitmap
	SetTransparentColor(RGB(0, 255, 0)); // set green as the transparent color

	CFont * pFont = GetFont();
	if (pFont)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		pFont->GetLogFont(&lf);
		m_bigFont.CreateFont(24, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, lf.lfFaceName);
		m_midFont.CreateFont(18, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, lf.lfFaceName);
	}
	else
	{
		m_bigFont.CreateFont(24, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Tahoma");
		m_midFont.CreateFont(18, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Tahoma");
	}

	m_btnOk.SetSkin(IDB_BITMAP_NORMAL, IDB_BITMAP_HOVER, IDB_BITMAP_HOVER, 0, 0, 0, 1, 0, 0);
	m_btnOk.SetToolTipText("OK");
	m_btnOk.SetFont(&m_bigFont);

	m_btnYes.SetSkin(IDB_BITMAP_NORMAL, IDB_BITMAP_HOVER, IDB_BITMAP_HOVER, 0, 0, 0, 1, 0, 0);
	m_btnYes.SetToolTipText("Yes");
	m_btnYes.SetFont(&m_bigFont);

	m_btnNo.SetSkin(IDB_BITMAP_NORMAL, IDB_BITMAP_HOVER, IDB_BITMAP_HOVER, 0, 0, 0, 1, 0, 0);
	m_btnNo.SetToolTipText("No");
	m_btnNo.SetFont(&m_bigFont);

	if (m_nButtons == MBYESNO)
	{
		m_btnOk.ShowWindow(SW_HIDE);
		m_btnYes.ShowWindow(SW_SHOW);
		m_btnNo.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnOk.ShowWindow(SW_SHOW);
		m_btnYes.ShowWindow(SW_HIDE);
		m_btnNo.ShowWindow(SW_HIDE);
	}

	m_msg.SetFont(&m_midFont);
	m_msg.SetWindowTextA((LPCTSTR)m_strBody);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMsg::OnBnClickedButtonYes()
{
	OnOK();
}

void CDlgMsg::OnBnClickedButtonNo()
{
	OnCancel();
}

void CDlgMsg::OnBnClickedButtonOk()
{
	OnOK();
}

int MyMessageBox(LPCTSTR lpszTxt, int nMsgButtons)
{
	CDlgMsg dlg;
	dlg.SetButtonCount(nMsgButtons);
	dlg.SetMessageText(lpszTxt);
	return dlg.DoModal();
}
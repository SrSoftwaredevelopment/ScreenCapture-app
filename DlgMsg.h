#pragma once

#include "CDialogSK.h"
#include "xSkinButton.h"
#include "TransparentStatic.h"

enum MessageButton
{
	MBNONE,
	MBOK,
	MBYESNO, 
};

// CDlgMsg dialog
class CDlgMsg : public CDialogSK
{
	DECLARE_DYNAMIC(CDlgMsg)

public:
	CDlgMsg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgMsg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MSG };

	CxSkinButton		m_btnOk;
	CxSkinButton		m_btnYes;
	CxSkinButton		m_btnNo;

	CTransparentStatic	m_msg;

	CString				m_strBody;
	int					m_nButtons;

protected:
	CFont				m_bigFont;
	CFont				m_midFont;

public:
	void				SetButtonCount(int nCount) { m_nButtons = nCount; }
	void				SetMessageText(LPCTSTR lpszTxt) { m_strBody = lpszTxt; }

protected:
	virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL		OnInitDialog();

	afx_msg void		OnBnClickedButtonYes();
	afx_msg void		OnBnClickedButtonNo();
	afx_msg void		OnBnClickedButtonOk();

	DECLARE_MESSAGE_MAP()
};

int MyMessageBox(LPCTSTR lpszTxt, int nMsgButtons);
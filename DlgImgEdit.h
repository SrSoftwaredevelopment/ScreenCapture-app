#pragma once

#include "BPCtrlAnchorMap.h"
#include "xSkinButton.h"
#include "WndImage.h"

// CDlgImgEdit dialog
class CDlgImgEdit : public CDialog
{
	// -------------------------------------
	// the following code declares the 
	// required elements for our AnchorMap
	// -------------------------------------
	DECLARE_ANCHOR_MAP();

public:
	CDlgImgEdit(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgImgEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDITOR };

	CxSkinButton	m_btnBlur;
	CxSkinButton	m_btnCrop;
	CxSkinButton	m_btnClose;
	CxSkinButton	m_btnExit;
	CxSkinButton	m_btnSave;
	CxSkinButton	m_btnUndo;

	CWndImage		m_imgPanel;

protected:
	CFont			m_bigFont;
	CxImage			m_imgLogo;
	CString			m_strImgPath;

	int				m_nDraw;

public:
	void			SetCaptureImage(LPCTSTR lpszFile) { m_strImgPath = lpszFile;  }

protected:
	void			Initialize();
	void			SkinButton(CxSkinButton& button, LPCTSTR lpszTok, LPCTSTR lpszTooltip);

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	afx_msg void	OnBnClickedButtonClose();
	afx_msg void	OnBnClickedButtonExit();
	afx_msg void	OnBnClickedButtonSave();
	afx_msg void	OnBnClickedCheckBlur();
	afx_msg void	OnBnClickedCheckCrop();
	afx_msg void	OnBnClickedButtonUndo();
	afx_msg void	OnPaint();
	afx_msg void	OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

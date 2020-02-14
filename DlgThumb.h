#pragma once

#include "BPCtrlAnchorMap.h"
#include "xSkinButton.h"
#include "WndImage.h"
#include <ximage.h>

// CDlgThumb dialog
class CDlgThumb : public CDialog
{
	// -------------------------------------
	// the following code declares the 
	// required elements for our AnchorMap
	// -------------------------------------
	DECLARE_ANCHOR_MAP();

public:
	CDlgThumb(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgThumb();

// Dialog Data
	enum { IDD = IDD_DIALOG_THUMB };

	CxSkinButton	m_btnEdt;
	CxSkinButton	m_btnDel;

	CString			m_strImgFile;
	int				m_nBltMode;

	CxImage			m_img;
	CFont			m_bigFont;

public:
	void			SetImage(LPCTSTR lpszFile, int nBlt);
	void			ShowButtons(BOOL bShow);

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	
	afx_msg void	OnBnClickedButtonDelete();
	afx_msg void	OnBnClickedButtonEdit();
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnPaint();
	afx_msg void	OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

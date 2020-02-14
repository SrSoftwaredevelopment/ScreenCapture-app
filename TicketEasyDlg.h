
// TicketEasyDlg.h : header file
//

#pragma once

#include "BPCtrlAnchorMap.h"
#include "SystemTray.h"
#include "PromptEdit.h"
#include "xSkinButton.h"
#include "TicketHook.h"
#include "TransparentStatic.h"
#include "DlgThumb.h"
#include <vector>

using namespace std;

#define	 WM_ICON_NOTIFY			WM_USER + 0x6000
#define	 WM_HIDEWINDOW			WM_USER + 0x5000
#define	 WM_USER_SENDCAPTURE	WM_USER + 0x5001
#define  CAPTURECOUNT			5


typedef struct tagARROWSTRUCT {
	int nWidth;		// width (in pixels) of the full base of the arrowhead
	float fTheta;	// angle (in radians) at the arrow tip between the two
					//  sides of the arrowhead
	bool bFill;		// flag indicating whether or not the arrowhead should be
					//  filled
} ARROWSTRUCT;

// CTicketEasyDlg dialog
class CTicketEasyDlg : public CDialog
{
	// -------------------------------------
	// the following code declares the 
	// required elements for our AnchorMap
	// -------------------------------------
	DECLARE_ANCHOR_MAP();

// Construction
public:
	CTicketEasyDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TICKETEASY_DIALOG };

	CxSkinButton		m_btnClose;
	CxSkinButton		m_btnSubmit;

	CPromptEdit			m_edtTitle;
	CPromptEdit			m_edtCont;

	CDlgThumb 	 		m_imgPanel[CAPTURECOUNT];

	CxImage				m_imgCapt;
	CxImage				m_imgBk;
	CxImage				m_imgLogo;
	CxImage				m_imgButtons[2];

// Implementation
protected:
	HICON				m_hIcon;
	CSystemTray			m_TrayIcon;
	CTicketHook			m_hook;

	CFont				m_bigFont;
	CFont				m_midFont;
	CFont				m_smallFont;

	int					m_nX;
	int					m_nY;

	DWORD				m_dwProcId;

	vector<LONG>		m_vCaptures;

public:
	void				ArrangeScreenshots();

protected:
	void				ArrowTo(CDC * pDC, int x, int y, ARROWSTRUCT *pArrow);
	void				ArrowTo(CDC * pDC, const POINT *lpTo, ARROWSTRUCT *pArrow);
	void				DoScreenCapture();
	void				Initialize();
	void				RelayoutCapturePanels();
	void				UnixTimeToFileTime(time_t t, LPFILETIME pft);
	void				UnixTimeToSystemTime(time_t t, LPSYSTEMTIME pst);

	BOOL				IsExistScreenshot(LONG lCaptTime);

	// Generated message map functions
	virtual void		DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL		OnInitDialog();
	virtual BOOL		PreTranslateMessage(MSG* pMsg);
	virtual LRESULT		WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void		OnBnClickedButtonClose();
	afx_msg void		OnBnClickedButtonSubmit();
	afx_msg void		OnDestroy();
	afx_msg void		OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void		OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void		OnPopupExit();
	afx_msg void		OnPopupShowwindow();
	afx_msg void		OnPaint();
	afx_msg void		OnSize(UINT nType, int cx, int cy);
	afx_msg void		OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg BOOL		OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL		OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HCURSOR		OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
};

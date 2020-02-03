#if !defined(AFX_DIMEDITCTRL_H__CF8D88FB_6945_11D4_8AC4_00C04F6092F9__INCLUDED_)
#define AFX_DIMEDITCTRL_H__CF8D88FB_6945_11D4_8AC4_00C04F6092F9__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DimEditCtrl.h : header file
//


//
//	This Specifies The Length Of The Dim Text Buffer...
//
static	const	int		DIM_TEXT_LEN =	128;				// Dim Text Buffer Length


/////////////////////////////////////////////////////////////////////////////
// CDimEditCtrl window

class CDimEditCtrl : public CEdit
{
// Construction
public:
	/**/	CDimEditCtrl();									// Constructor

// Attributes
public:

// Operations
public:

	void	SetShowDimControl( bool bShow );				// Show Or Hide The Dim Control
	void	SetDimText( LPCTSTR cpText );					// Set The Dim Text
	void	SetDimColor( COLORREF crDColor );				// Set The Dim Color
	void	SetDimOffset( char cRedOS, char cGreenOS, 
					char cBlueOS );							// Set The Dim Color Offset

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDimEditCtrl)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDimEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDimEditCtrl)
	afx_msg void OnChange();
	afx_msg void OnSetfocus();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//}}AFX_MSG
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

	void		DrawDimText( void );						// Draw The Dim Text

	COLORREF	m_crDimTextColor;							// "Hard" Dim Text Color
	TCHAR		m_caDimText[ DIM_TEXT_LEN + 1 ];			// Dim Text Buffer
	bool		m_bShowDimText;								// Are We Showing The Dim Text?
	bool		m_bUseDimOffset;							// Are We Using The Offset Colors (Not Hard Color)?
	char		m_cRedOS;									// Red Color Dim Offset
	char		m_cGreenOS;									// Green Color Dim Offset
	char		m_cBlueOS;									// Blue Color Dim Offset
	int			m_iDimTextLen;								// Length Of Dim Text
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIMEDITCTRL_H__CF8D88FB_6945_11D4_8AC4_00C04F6092F9__INCLUDED_)

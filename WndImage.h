// =======================================================
// 
//                      CWndImage.h
//              WndImage class declarations
// 
//           Copyright (C) 2000 Peter Hauptmann
//
//  Can be used and distributed freely 
//          as long as the source copyright stays intact
// 
//  updated version can be found under 
//          http://buerger.metropolis.de/bitbucket/
//
// -------------------------------------------------------
//
//  2000-05-16: Initial Revision
//



#if !defined(AFX_WNDIMAGE_H__18E060C2_DE37_4419_B87A_09ACF6963828__INCLUDED_)
#define AFX_WNDIMAGE_H__18E060C2_DE37_4419_B87A_09ACF6963828__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ximage.h"
#include "MyTracker.h"

enum bltmodes
{
	bltBlt,
	bltStretch,      // blit stretched to fit entire frame
	bltCenter,       // align to center
};

enum DrawMode
{
	DRAWUNDO,
	DRAWCROP,
	DRAWBLUR,
};

#define WM_USER_LBUTTONTRACK	WM_USER + 0x2100
#define WM_USER_IMAGECROP		WM_USER + 0x2101
#define WM_USER_IMAGEBLUR		WM_USER + 0x2102

// ==================================================================
//  CWndImage
// ------------------------------------------------------------------

class CWndImage : public CWnd
{
public:
	CWndImage();
	virtual    ~CWndImage();

	BOOL			Create(RECT const & r, CWnd * parent, UINT id, DWORD dwStyle = WS_CHILD | WS_VISIBLE);
	BOOL			SetImgFile(LPCTSTR fileName);

	void			SetBltMode(int nMode) { m_bltMode = nMode; }
	void			SetDrawMode(int nDraw) { m_nDrawMode = nDraw; }

	CxImage *		GetImage() { return &m_imgBk; }

protected:
	void			GetImageXY(CxImage * ima, long * x, long * y);

	BOOL			IsStretcDrawRect(CxImage& img);
	BOOL			SetImageRectSelection(CRect *rect);

protected:
	CxImage			m_imgBk;
	CMyTracker		m_tracker;

	int				m_bltMode;
	int				m_nDrawMode;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//{{AFX_MSG(CWndImage)
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnPaint();
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDIMAGE_H__18E060C2_DE37_4419_B87A_09ACF6963828__INCLUDED_)

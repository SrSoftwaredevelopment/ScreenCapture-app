// =======================================================
// 
//                    CWndImage.cpp
//             WndImage class implementation
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

#include "pch.h"
#include "WndImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==================================================================
//  CTor / DTor
// ------------------------------------------------------------------

CWndImage::CWndImage()
{
	m_bltMode = bltBlt;
	m_nDrawMode = DRAWUNDO;
	m_tracker.m_rect = CRect(0, 0, 0, 0);
	m_tracker.m_nStyle = CRectTracker::resizeOutside | CRectTracker::dottedLine;
	m_tracker.m_rect.SetRect(-1, -2, -3, -4);
	m_tracker.SetRectColor(RGB(255, 0, 0));
}

CWndImage::~CWndImage()
{

}


BEGIN_MESSAGE_MAP(CWndImage, CWnd)
	//{{AFX_MSG_MAP(CWndImage)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// ==================================================================
//  OnPaint
// ------------------------------------------------------------------

void CWndImage::OnPaint() 
{
	DWORD style = GetStyle();
	if (!(style & WS_VISIBLE))
		return;

	CPaintDC dc(this);

	if (m_bltMode == bltStretch)
	{
		CRect r;
		GetClientRect(&r);
		dc.FillSolidRect(&r, RGB(46, 46, 46));
		m_imgBk.Draw(dc.GetSafeHdc(), r);
	}
	else  if (m_bltMode == bltCenter)
	{
		CRect r;
		GetClientRect(&r);
		dc.FillSolidRect(&r, RGB(46, 46, 46));

		int w = m_imgBk.GetWidth();
		int h = m_imgBk.GetHeight();
		int x = r.left + (r.Width() - w) / 2;
		int y = r.top + (r.Height() - h) / 2;

		m_imgBk.Stretch(dc.GetSafeHdc(), x, y, w, h);
	}
	else
	{
		CRect r;
		GetClientRect(&r);
		dc.FillSolidRect(&r, RGB(46, 46, 46));
		r.right = r.left + m_imgBk.GetWidth();
		r.bottom = r.top + m_imgBk.GetHeight();
		m_imgBk.Draw(dc.GetSafeHdc(), r);
	}
}

// ==================================================================
//  Create
// ------------------------------------------------------------------
BOOL CWndImage::Create(RECT const & r, CWnd * parent, UINT id, DWORD dwStyle)
{
  BOOL ok = CWnd::Create(NULL, NULL, dwStyle, r, parent, id, NULL);
  return ok;
}

BOOL CWndImage::SetImgFile(LPCTSTR fileName)
{
	m_imgBk.Load(fileName, CXIMAGE_FORMAT_JPG);
	Invalidate();
	return TRUE;
}

void CWndImage::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	Invalidate();
}

void CWndImage::GetImageXY(CxImage * ima, long * x, long * y)
{
	if (ima == NULL)
		return;

	CPoint point = CPoint(0, 0);
	float fx = (float)(*x + point.x);
	float fy = (float)(*y + point.y);

	if (m_bltMode == bltStretch)
	{
		CRect rect;
		GetClientRect(&rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		fx *= ima->GetWidth() / (float)width;
		fy *= ima->GetHeight() / (float)height;
	}

	*x = (long)fx;
	*y = (long)fy;
}

BOOL CWndImage::SetImageRectSelection(CRect *rect)
{
	CxImage* ima = GetImage();
	if (ima == NULL)
		return FALSE;

	long x, y;
	RECT rect_img;
	x = rect_img.left = rect->left;
	y = rect_img.top = rect->top;
	GetImageXY(ima, &x, &y);
	rect_img.top = ima->GetHeight() - 1 - y;
	rect_img.left = x;

	x = rect_img.right = rect->right;
	y = rect_img.bottom = rect->bottom;
	GetImageXY(ima, &x, &y);
	rect_img.bottom = ima->GetHeight() - 1 - y;
	rect_img.right = x;

	ima->SelectionClear();
	ima->SelectionAddRect(rect_img);

	return TRUE;
}

void CWndImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_tracker.HitTest(point) < 0)
	{
		CRectTracker track;
		if (track.TrackRubberBand(this, point, true)) {
			track.m_rect.NormalizeRect();
			m_tracker.m_rect = track.m_rect;
			SetImageRectSelection(&m_tracker.m_rect);
			SendMessage(WM_USER_LBUTTONTRACK);
		}
		else {
			m_tracker.m_rect = CRect(0, 0, 0, 0);
		}
	}
	else {
		if (m_tracker.Track(this, point, true)) {
			m_tracker.m_rect.NormalizeRect();
			SetImageRectSelection(&m_tracker.m_rect);
			SendMessage(WM_USER_LBUTTONTRACK);
		}
		else {
			m_tracker.m_rect = CRect(0, 0, 0, 0);
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CWndImage::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CWndImage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_tracker.SetCursor(this, nHitTest))
		return TRUE;

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CWndImage::IsStretcDrawRect(CxImage& img)
{
	CRect rtImg;
	GetClientRect(&rtImg);

	int w1 = img.GetWidth();
	int h1 = img.GetHeight();
	int w2 = rtImg.Width();
	int h2 = rtImg.Height();

	if (w1 > w2 || h1 > h2)
		return TRUE;

	return FALSE;
}

LRESULT CWndImage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCHITTEST || message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK)
		return ::DefWindowProc(m_hWnd, message, wParam, lParam);

	if (message == WM_USER_LBUTTONTRACK)
	{
		if (m_nDrawMode == DRAWCROP)
		{
			RECT rt;
			m_imgBk.SelectionGetBox(rt);
			rt.bottom = m_imgBk.GetHeight() - 1 - rt.bottom;
			rt.top = m_imgBk.GetHeight() - 1 - rt.top;
			m_imgBk.Crop(rt);

			m_tracker.m_rect = CRect(0, 0, 0, 0);

			if (IsStretcDrawRect(m_imgBk))
				SetBltMode(bltStretch);
			else
				SetBltMode(bltBlt);
		}
		else if (m_nDrawMode == DRAWBLUR)
		{
			RECT rt;
			m_imgBk.SelectionGetBox(rt);
			rt.bottom = m_imgBk.GetHeight() - rt.bottom;
			rt.top = m_imgBk.GetHeight() - rt.top;

			m_tracker.m_rect = CRect(0, 0, 0, 0);

			int32_t kernel[] = { 1,1,1,1,1,1,1,1,1 };
			for (int i = 0; i < 10; i++)
				m_imgBk.Filter(kernel, 3, 9, 0);
			
			if (IsStretcDrawRect(m_imgBk))
				SetBltMode(bltStretch);
			else
				SetBltMode(bltBlt);
		}

		Invalidate();
		return TRUE;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

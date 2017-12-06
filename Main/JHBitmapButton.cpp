// JHBitmapButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Main.h"
#include "JHBitmapButton.h"


// JHBitmapButton

IMPLEMENT_DYNAMIC(JHBitmapButton, CBitmapButton)

JHBitmapButton::JHBitmapButton()
{
	m_clrTrans				= RGB(255,255,255);
	m_bMouseHover			= FALSE;
	m_nIDBitmapResourceHover= 0;
	ZeroMemory(&m_stTrackMouse, sizeof(m_stTrackMouse));
}

JHBitmapButton::~JHBitmapButton()
{
}


BEGIN_MESSAGE_MAP(JHBitmapButton, CBitmapButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// JHBitmapButton 메시지 처리기입니다.


void JHBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS != NULL);
	// must have at least the first bitmap loaded before calling DrawItem
	ASSERT(m_bitmap.m_hObject != NULL);     // required

	// use the main bitmap for up, the selected bitmap for down
	CBitmap* pBitmap = &m_bitmap;
	UINT state = lpDIS->itemState;
	if ((state & ODS_SELECTED) && m_bitmapSel.m_hObject != NULL)
		pBitmap = &m_bitmapSel;
	else if ((state & ODS_FOCUS) && m_bitmapFocus.m_hObject != NULL)
		pBitmap = &m_bitmapFocus;   // third image for focused
	else if ((state & ODS_DISABLED) && m_bitmapDisabled.m_hObject != NULL)
		pBitmap = &m_bitmapDisabled;   // last image for disabled
	if ((TRUE == m_bMouseHover) && NULL != m_cBitmapHover.GetSafeHandle())
	{
		pBitmap = &m_cBitmapHover;
	}

	// draw the whole button
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	CRect rect;
	rect.CopyRect(&lpDIS->rcItem);

	CLayoutMgr::DrawBitmapTrans(pDC, pBitmap, rect.left, rect.top, m_clrTrans);
}

BOOL JHBitmapButton::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void JHBitmapButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (FALSE == m_bMouseHover)
	{
		::_TrackMouseEvent(&m_stTrackMouse);
		m_stTrackMouse.cbSize = sizeof(m_stTrackMouse);
		m_stTrackMouse.hwndTrack = m_hWnd;
		m_stTrackMouse.dwFlags = TME_LEAVE|TME_HOVER;
		m_stTrackMouse.dwHoverTime = 1;
		if(TrackMouseEvent(&m_stTrackMouse))
		{
			m_bMouseHover = TRUE;
			Invalidate();
		}
		//m_bMouseHover = TRUE;
		Invalidate(FALSE);
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}

void JHBitmapButton::OnMouseLeave()
{
	m_bMouseHover = FALSE;

	Invalidate(FALSE);
	CBitmapButton::OnMouseLeave();
}

VOID JHBitmapButton::SetHoverBitmapID(IN UINT nIDBitmapResourceHover)
{
	m_nIDBitmapResourceHover = nIDBitmapResourceHover;
	if (0 != m_nIDBitmapResourceHover)
	{
		m_cBitmapHover.DeleteObject();
		m_cBitmapHover.LoadBitmap(m_nIDBitmapResourceHover);
	}
}
int JHBitmapButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBitmapButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_stTrackMouse.cbSize		= sizeof(TRACKMOUSEEVENT);
	m_stTrackMouse.dwFlags		= TME_LEAVE;
	m_stTrackMouse.dwHoverTime	= HOVER_DEFAULT;
	m_stTrackMouse.hwndTrack	= GetSafeHwnd();

	return 0;
}


VOID CLayoutMgr::DrawBitmapTrans(IN OUT CDC* pDC, IN CBitmap* pcBitmap, IN int x, IN int y, IN COLORREF crMask)
{
	CBitmap		*pOldBitmapImage	= NULL;
	CBitmap		*pOldBitmapTrans	= NULL;
	COLORREF	clrOldBack			= 0;
	COLORREF	clrOldText			= 0;
	BITMAP		bm					= {0,};
	CDC			dcImage;
	CDC			dcTrans;
	CBitmap		bitmapTrans;

	if ((NULL == pcBitmap) || (NULL == pDC))
	{
		return;
	}

	pcBitmap->GetBitmap(&bm);
	dcImage.CreateCompatibleDC( pDC );
	pOldBitmapImage = dcImage.SelectObject( pcBitmap );
	clrOldBack = pDC->SetBkColor( RGB( 255, 255, 255 ) );
	clrOldText = pDC->SetTextColor( RGB( 0, 0, 0 ) );
	dcTrans.CreateCompatibleDC( pDC );
	bitmapTrans.CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL );
	pOldBitmapTrans = dcTrans.SelectObject( &bitmapTrans );
	dcImage.SetBkColor( crMask );
	dcTrans.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY );
	pDC->BitBlt( x, y, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCINVERT );
	pDC->BitBlt( x, y, bm.bmWidth, bm.bmHeight, &dcTrans, 0, 0, SRCAND );
	pDC->BitBlt( x, y, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCINVERT );
	dcTrans.SelectObject( pOldBitmapTrans );
	pDC->SetBkColor( clrOldBack );
	pDC->SetTextColor( clrOldText );
	bitmapTrans.DeleteObject();
	dcTrans.DeleteDC();
	dcImage.SelectObject( pOldBitmapImage );
	dcImage.DeleteDC();    
}
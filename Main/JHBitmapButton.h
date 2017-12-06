#pragma once


// JHBitmapButton

class JHBitmapButton : public CBitmapButton
{
	DECLARE_DYNAMIC(JHBitmapButton)

public:
	JHBitmapButton();
	virtual ~JHBitmapButton();
	virtual VOID SetTransBitmap(IN COLORREF clrTrans) {m_clrTrans = clrTrans;}
	virtual VOID SetHoverBitmapID(IN UINT nIDBitmapResourceHover);

protected:
	DECLARE_MESSAGE_MAP()
	COLORREF			m_clrTrans;
	BOOL				m_bMouseHover;
	TRACKMOUSEEVENT		m_stTrackMouse;
	UINT				m_nIDBitmapResourceHover;
	CBitmap				m_cBitmapHover;

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


class CLayoutMgr
{
public:
	static VOID DrawBitmapTrans(IN OUT CDC* pDC, IN CBitmap* pcBitmap, IN int x, IN int y, IN COLORREF crMask);
};


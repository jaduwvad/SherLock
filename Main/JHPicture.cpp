// JHPicture.cpp : ���� �����Դϴ�.
//
#include "stdafx.h"
#include "Main.h"
#include "JHPicture.h"


// JHPicture

IMPLEMENT_DYNAMIC(JHPicture, CStatic)

JHPicture::JHPicture()
{
	transparent = RGB(0,0,0);
}

JHPicture::~JHPicture()
{
}


BEGIN_MESSAGE_MAP(JHPicture, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void JHPicture::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	HBITMAP old, bmp = GetBitmap();
	if( bmp != NULL )
	{
		BITMAP bmInfo;
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		old = (HBITMAP)::SelectObject(memDC.m_hDC, bmp);
		::GetObject(bmp, sizeof(BITMAP), &bmInfo);
		::TransparentBlt(dc.m_hDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, memDC.m_hDC,
			0, 0, bmInfo.bmWidth, bmInfo.bmHeight, transparent);
		::SelectObject(memDC.m_hDC, old);
		memDC.DeleteDC();
	}
	else
		CStatic::OnPaint();
	// �׸��� �޽����� ���ؼ��� CStatic::OnPaint()��(��) ȣ������ ���ʽÿ�.
}

// JHPicture �޽��� ó�����Դϴ�.



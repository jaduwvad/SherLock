// JHPicture.cpp : 구현 파일입니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.
}

// JHPicture 메시지 처리기입니다.



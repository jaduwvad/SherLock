// DIALOG1.cpp : 구현 파일입니다.
//
#pragma once

#include "stdafx.h"
#include "password.h"
#include "DIALOG1.h"
#include "afxdialogex.h"
#include "SAVE.h"

// DIALOG1 대화 상자입니다.

IMPLEMENT_DYNAMIC(DIALOG1, CDialog)

	DIALOG1::DIALOG1(CWnd* pParent /*=NULL*/)
	: CDialog(DIALOG1::IDD, pParent)
{

}

DIALOG1::~DIALOG1()
{
}

void DIALOG1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DIALOG1, CDialog)
	ON_BN_CLICKED(ID_VIEWALL, &DIALOG1::OnBnClickedViewall)
	ON_BN_CLICKED(IDC_VIEW1, &DIALOG1::OnBnClickedView1)
	ON_BN_CLICKED(ID_VIEW2, &DIALOG1::OnBnClickedView2)
END_MESSAGE_MAP()


// DIALOG1 메시지 처리기입니다.




//////////////////////////////////////////////전체보기///////////////////////////////////////////
void DIALOG1::OnBnClickedViewall()
{
	UpdateData(TRUE);
	buttonNum = 1;
	UpdateData(FALSE);
	EndDialog(1);
}	




///////////////////////////////////////////암호화할 파일////////////////////////////////////////
void DIALOG1::OnBnClickedView1()
{
	UpdateData(TRUE);
	buttonNum = 2;
	UpdateData(FALSE);
	EndDialog(1);
}




///////////////////////////////////////////복호화할 파일//////////////////////////////////////////
void DIALOG1::OnBnClickedView2()
{
	UpdateData(TRUE);
	buttonNum = 3;
	UpdateData(FALSE);
	EndDialog(1);
}

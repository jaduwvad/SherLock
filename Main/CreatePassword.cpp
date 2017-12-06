// CreatePassword.cpp : 구현 파일입니다.
//
#pragma once

#include "stdafx.h"
#include "Main.h"
#include "CreatePassword.h"
#include "afxdialogex.h"


// CCreatePassword 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCreatePassword, CDialogEx)

	CCreatePassword::CCreatePassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreatePassword::IDD, pParent)
	, password_New(_T(""))
	, password_Confirm(_T(""))
{

}

CCreatePassword::~CCreatePassword()
{
}

void CCreatePassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PASSWORD_NEW, password_New);
	DDX_Text(pDX, IDC_PASSWORD_CONFIRM, password_Confirm);
}


BEGIN_MESSAGE_MAP(CCreatePassword, CDialogEx)
	ON_BN_CLICKED(IDC_PASSWORD_OK, &CCreatePassword::OnBnClickedPasswordOk)
END_MESSAGE_MAP()


// CCreatePassword 메시지 처리기입니다.



////////////////////////////////////////////////////비밀번호 생성///////////////////////////////////////////
void CCreatePassword::OnBnClickedPasswordOk()
{
	UpdateData(TRUE);

	CString f;

	if(password_New.GetLength()>=4)
	{
		if(password_New==password_Confirm)
		{
			CStdioFile passwordFile;
						
			passwordFile.Open("D:\\listmemory\\password.dat",CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			passwordFile.WriteString(password_New);
			passwordFile.Close();
			OnOK();
		}

		else
		{
			MessageBox(_T("비밀번호 재확인!"));
		}
	}
	else
	{
		MessageBox(_T("최소 4자 이상 입력!"));
	}

	UpdateData(FALSE);
}


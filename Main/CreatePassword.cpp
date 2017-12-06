// CreatePassword.cpp : ���� �����Դϴ�.
//
#pragma once

#include "stdafx.h"
#include "Main.h"
#include "CreatePassword.h"
#include "afxdialogex.h"


// CCreatePassword ��ȭ �����Դϴ�.

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


// CCreatePassword �޽��� ó�����Դϴ�.



////////////////////////////////////////////////////��й�ȣ ����///////////////////////////////////////////
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
			MessageBox(_T("��й�ȣ ��Ȯ��!"));
		}
	}
	else
	{
		MessageBox(_T("�ּ� 4�� �̻� �Է�!"));
	}

	UpdateData(FALSE);
}


// ModifyPassword.cpp : ���� �����Դϴ�.
//
#pragma once
#include "stdafx.h"
#include "Main.h"
#include "ModifyPassword.h"
#include "afxdialogex.h"


// CModifyPassword ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CModifyPassword, CDialogEx)

	CModifyPassword::CModifyPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyPassword::IDD, pParent)
	, password_New(_T(""))
	, password_Confirm(_T(""))
	, password_Old(_T(""))
{

}

CModifyPassword::~CModifyPassword()
{
}

void CModifyPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_PASSWORD_NEW, password_New);
	DDX_Text(pDX, IDC_PASSWORD_CONFIRM, password_Confirm);
	DDX_Text(pDX, IDC_PASSWORD_OLD, password_Old);
	DDX_Control(pDX, IDC_MODPWBG, modPwBg);
	DDX_Control(pDX, IDC_PASSWORD_OK, pwOkBtn);
	DDX_Control(pDX, IDC_CANCEL, cancelBtn);
}


BEGIN_MESSAGE_MAP(CModifyPassword, CDialogEx)
	ON_BN_CLICKED(IDC_PASSWORD_OK, &CModifyPassword::OnBnClickedPasswordOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CANCEL, &CModifyPassword::OnBnClickedCancel)
END_MESSAGE_MAP()


// CModifyPassword �޽��� ó�����Դϴ�.


BOOL CModifyPassword::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP hBit=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_SMALLDLG));

	modPwBg.SetBitmap(hBit);
	CRect rt;
	GetClientRect(&rt);
	modPwBg.SetWindowPos(NULL,0,0,rt.Width(), rt.Height(),SWP_SHOWWINDOW);

	pwOkBtn.LoadBitmaps(IDB_OKBTN_N, IDB_OKBTN_C, IDB_OKBTN_N, IDB_OKBTN_N);
	pwOkBtn.SizeToContent();
	pwOkBtn.SetHoverBitmapID(IDB_OKBTN_C);

	cancelBtn.LoadBitmaps(IDB_BACK, IDB_BACK_C, IDB_BACK, IDB_BACK);
	cancelBtn.SizeToContent();
	cancelBtn.SetHoverBitmapID(IDB_BACK_C);
	//
	return TRUE;
}

/////////////////////////////////////////////////////��й�ȣ ����///////////////////////////////////////////
void CModifyPassword::OnBnClickedPasswordOk()
{
	UpdateData(TRUE);

	CStdioFile passwordFile;
	CString savedPassword;

	passwordFile.Open("D:\\listmemory\\password.dat", CFile::modeRead);
	passwordFile.ReadString(savedPassword);
	passwordFile.Close();

	if(password_Old==savedPassword)
	{
		if(password_New.GetLength()>=4)
		{
			if(password_New==password_Confirm)
			{
				///////////���� �ʱ�ȭ/////////////
				DeleteFile("D:\\listmemory\\password.dat");
			
				//////////��й�ȣ ���ξ���/////////
				passwordFile.Open("D:\\listmemory\\password.dat",CFile::modeCreate | CFile::modeReadWrite | CFile::typeText);
				passwordFile.WriteString(password_New);
				passwordFile.Close();
				OnOK();
			}
			else
			{
				(MessageBox(_T("�� ��й�ȣ ��Ȯ��!")));
			}
		}
		else
		{
			MessageBox(_T("�ּ� 4�� �̻� �Է�!"));
		}

	}
	else
	{
		MessageBox(_T("���� ��й�ȣ ��Ȯ��!"));
	}
	UpdateData(FALSE);
}


HBRUSH CModifyPassword::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_STATIC1 || nID == IDC_STATIC2 || nID == IDC_STATIC3 || nID == IDC_STATIC4)
		{
			pDC->SetBkMode(TRANSPARENT);      // �������
			//pDC->SetTextColor(RGB(0, 0, 0));  // ���� ��
			//pDC->SetBkColor(RGB(0, 128, 0));  // ��� ��
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	}
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


void CModifyPassword::OnBnClickedCancel()
{
	OnCancel();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

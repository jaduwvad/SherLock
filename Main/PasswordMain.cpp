// Password.cpp : ���� �����Դϴ�.
//
#pragma once
#include "stdafx.h"
#include "Main.h"
#include "PasswordMain.h"
#include "afxdialogex.h"
#include "ModifyPassword.h"


// CPassword ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPasswordMain, CDialogEx)

	CPasswordMain::CPasswordMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPasswordMain::IDD, pParent)
	, userInputPassword(_T(""))
{

}

CPasswordMain::~CPasswordMain()
{
}




void CPasswordMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_PASSWORD, userInputPassword);
	DDX_Control(pDX, IDC_PWMAINBG, pwBg);
	DDX_Control(pDX, IDC_PASSWORD_OK, pwOkBtn);
	DDX_Control(pDX, IDC_PWCANCEL, pwCancel);
}

BOOL CPasswordMain::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();
	
	HBITMAP hBit=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_SMALLDLG));

	pwBg.SetBitmap(hBit);
	CRect rt;
	GetClientRect(&rt);
	pwBg.SetWindowPos(NULL,0,0,rt.Width(), rt.Height(),SWP_SHOWWINDOW);

	pwOkBtn.LoadBitmaps(IDB_OKBTN_N, IDB_OKBTN_C, IDB_OKBTN_N, IDB_OKBTN_N);
	pwOkBtn.SizeToContent();
	pwOkBtn.SetHoverBitmapID(IDB_OKBTN_C);

	pwCancel.LoadBitmaps(IDB_BACK, IDB_BACK_C, IDB_BACK, IDB_BACK);
	pwCancel.SizeToContent();
	pwCancel.SetHoverBitmapID(IDB_BACK_C);


	//
	return TRUE;
}

BEGIN_MESSAGE_MAP(CPasswordMain, CDialogEx)
	ON_BN_CLICKED(IDC_PASSWORD_OK, &CPasswordMain::OnBnClickedPasswordOk)
	ON_BN_CLICKED(IDC_MOD_BUTTON, &CPasswordMain::OnBnClickedModButton)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_PWCANCEL, &CPasswordMain::OnBnClickedPwcancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPassword �޽��� ó�����Դϴ�.


void CPasswordMain::OnBnClickedPasswordOk()
{
	UpdateData(TRUE);

	CStdioFile passwordFile;
	CString savedPassword="";

	passwordFile.Open("D:\\listmemory\\password.dat", CFile::modeReadWrite | CFile::typeText);
	passwordFile.ReadString(savedPassword);
	passwordFile.Close();

	if(savedPassword==userInputPassword)
	{
		OnOK();
		isPw=TRUE;
	}

	else
	{
		SetTimer(101, 1000, NULL);
		if(AfxMessageBox("�ʴ� Ʋ�ȴ�!") == IDOK)
		isPw=FALSE;
	}
		

	UpdateData(FALSE);// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



void CPasswordMain::OnBnClickedModButton()
{
	CModifyPassword dlg;
	dlg.DoModal();
}


void CPasswordMain::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	isPw = FALSE;

	CDialogEx::OnClose();
}


void CPasswordMain::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


LRESULT CPasswordMain::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;
	return CDialogEx::OnNcHitTest(point);
}


void CPasswordMain::OnBnClickedPwcancel()
{
	OnCancel();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


HBRUSH CPasswordMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_PLEASEINPUT)
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

BOOL CPasswordMain::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->wParam == VK_RETURN)
	{
		OnBnClickedPasswordOk();
		return TRUE;
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}



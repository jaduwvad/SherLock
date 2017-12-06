// Password.cpp : 구현 파일입니다.
//
#pragma once
#include "stdafx.h"
#include "Main.h"
#include "PasswordMain.h"
#include "afxdialogex.h"
#include "ModifyPassword.h"


// CPassword 대화 상자입니다.

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


// CPassword 메시지 처리기입니다.


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
		if(AfxMessageBox("너는 틀렸다!") == IDOK)
		isPw=FALSE;
	}
		

	UpdateData(FALSE);// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CPasswordMain::OnBnClickedModButton()
{
	CModifyPassword dlg;
	dlg.DoModal();
}


void CPasswordMain::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	isPw = FALSE;

	CDialogEx::OnClose();
}


void CPasswordMain::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


LRESULT CPasswordMain::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;
	return CDialogEx::OnNcHitTest(point);
}


void CPasswordMain::OnBnClickedPwcancel()
{
	OnCancel();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


HBRUSH CPasswordMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_PLEASEINPUT)
		{
			pDC->SetBkMode(TRANSPARENT);      // 배경투명
			//pDC->SetTextColor(RGB(0, 0, 0));  // 글자 색
			//pDC->SetBkColor(RGB(0, 128, 0));  // 배경 색
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

BOOL CPasswordMain::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_RETURN)
	{
		OnBnClickedPasswordOk();
		return TRUE;
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}



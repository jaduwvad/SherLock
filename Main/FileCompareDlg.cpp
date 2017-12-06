
// FileCompareDlg.cpp : 구현 파일
//

#pragma once
#include "stdafx.h"
#include "FileCompare.h"
#include "FileCompareDlg.h"
#include "afxdialogex.h"
#include "SearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileCompareDlg 대화 상자


CFileCompareDlg::CFileCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileCompareDlg::IDD, pParent)
	, findPath(_T(""))
	, searchType(0)
	, isGui(FALSE)
	, infoEdit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, findPath);
	DDX_Control(pDX, IDC_MFCSHELLTREE, fileTree);
	DDX_Radio(pDX, IDC_RADIO_SEARCH_BASIC, searchType);
	DDX_Control(pDX, IDC_SLIDER_DETAIL, sliderDetail);
	DDX_Control(pDX, IDC_FCBG, fcBg);
	DDX_Control(pDX, IDC_FINDFILE, s_LoadBtn);
	DDX_Control(pDX, IDCANCEL, s_Cancel);
	DDX_Control(pDX, IDC_FINDSTART, s_Start);
}

BEGIN_MESSAGE_MAP(CFileCompareDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FINDFILE, &CFileCompareDlg::OnBnClickedFindfile)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE, &CFileCompareDlg::OnTvnSelchangedMfcshelltree)
	ON_BN_CLICKED(IDC_FINDSTART, &CFileCompareDlg::OnBnClickedFindstart)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO_SEARCH_BASIC, IDC_RADIO_SEARCH_DETAIL, OnBnClickedRadio)
	ON_BN_CLICKED(IDC_GUI_CHECK, &CFileCompareDlg::OnBnClickedGuiCheck)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DETAIL, &CFileCompareDlg::OnNMReleasedcaptureSliderDetail)
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFileCompareDlg 메시지 처리기

BOOL CFileCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTimer(0,100,NULL);
	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//SetWindowTheme(searchType.m_hWnd, L"", L"");

	HBITMAP hBit=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_SERODLG));
	fcBg.SetBitmap(hBit);
	::SetWindowTheme(GetDlgItem(IDC_RADIO_SEARCH_DETAIL)->GetSafeHwnd(), L"", L"");
	::SetWindowTheme(GetDlgItem(IDC_RADIO_SEARCH_BASIC)->GetSafeHwnd(), L"", L"");
	CRect rt;
	GetClientRect(&rt);
	fcBg.SetWindowPos(NULL,0,0,rt.Width(), rt.Height(),SWP_SHOWWINDOW);

	HTREEITEM ht;
	ht = fileTree.GetFirstVisibleItem();

	cDriveCnt = 0;
	sliderDetail.SetBitmapChannel(IDB_SLIDE3, IDB_SLIDE4, TRUE );
	null.SetBitmapChannel( IDB_SLIDEOFF, IDB_SLIDEOFF, TRUE );
	sliderDetail.SetRange(1, 100);
	sliderDetail.SetPos(50);
	detail = sliderDetail.GetPos();
	numOfFile = 0;
	fault = FALSE;

	//SetWindowTheme(this->m_hWnd, NULL,NULL);

	s_Cancel.LoadBitmaps(IDB_BACK, IDB_BACK_C, IDB_BACK, IDB_BACK);
	s_Cancel.SizeToContent();
	s_Cancel.SetHoverBitmapID(IDB_BACK_C);

	s_LoadBtn.LoadBitmaps(IDB_E_LOADBTN_N, IDB_E_LOADBTN_C, IDB_E_LOADBTN_N, IDB_E_LOADBTN_N);
	s_LoadBtn.SizeToContent();
	s_LoadBtn.SetHoverBitmapID(IDB_E_LOADBTN_C);

	s_Start.LoadBitmaps(IDB_SEARCHSTARTC, IDB_SEARCHSTARTN, IDB_SEARCHSTARTC, IDB_SEARCHSTARTC);
	s_Start.SizeToContent();
	s_Start.SetHoverBitmapID(IDB_SEARCHSTARTN);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFileCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFileCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileCompareDlg::OnBnClickedFindfile()
{
	CFileDialog fileDlg(TRUE, _T("*,*"), _T("*.*"),OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, _T("*.*"), this);
	if (IDOK != fileDlg.DoModal())
		return;
	findPath = fileDlg.GetPathName();
	//findPath.Replace(_T("\\"), _T("\\\\"));
	UpdateData(FALSE);

	CFile targetFile;

	targetFilePath = findPath;
	targetFilePath.Replace(_T("\\"), _T("\\\\"));
	targetFile.Open((LPCTSTR)targetFilePath, CFile::modeRead);

	targetFileName = targetFile.GetFileName();
	targetFileSize = targetFile.GetLength();
	targetFilePath = targetFile.GetFilePath();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CFileCompareDlg::OnTvnSelchangedMfcshelltree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	fileTree.GetItemPath(searchScope);
	fault = TRUE;
	if(searchScope == "")
	{
		MessageBox(_T("잘못된 경로 선택입니다."));
	}
	else
	{
		dir = searchScope;
	}
	Invalidate();
	
	//SetTimer(0, 500, NULL);
}

void CFileCompareDlg::OnBnClickedFindstart()
{
	if(searchScope == "")
	{
		MessageBox(_T("검색범위가 설정되지 않았습니다. 검색범위를 지정해 주세요."));
		return;
	}
	if(findPath == "")
	{
		MessageBox(_T("검색 대상 파일이 선택되지 않았습니다. 검색할 파일을 지정해 주세요."));
		return;
	}
	if(searchType == 0)
	{
	//	MessageBox(_T("찾을 File : ")+targetFilePath+_T("\n\n")+_T("찾을 경로 : ")+searchScope+_T("\n\n")+_T("검색 Type : 간단히 검색\n"));

		SearchDlg searchDlg;
		searchDlg.searchScope = searchScope;
		searchDlg.searchType = searchType;
		searchDlg.targetFileName = targetFileName;
		searchDlg.targetFilePath = targetFilePath;
		searchDlg.targetFileSize = targetFileSize;
		searchDlg.isGui = isGui;
		searchDlg.detail = detail;
		searchDlg.DoModal();
		numOfFile=0;

		//MessageBox(_T("검색 완료!!"));
	}
	else if(searchType == 1)
	{
//		MessageBox(_T("찾을 File : ")+targetFilePath+_T("\n\n")+_T("찾을 경로 : ")+searchScope+_T("\n\n")+_T("검색 Type : 자세히 검색\n"));

		SearchDlg searchDlg;
		searchDlg.searchScope = searchScope;
		searchDlg.searchType = searchType;
		searchDlg.targetFileName = targetFileName;
		searchDlg.targetFilePath = targetFilePath;
		searchDlg.targetFileSize = targetFileSize;
		searchDlg.isGui = isGui;
		searchDlg.detail = sliderDetail.GetPos();

		searchDlg.DoModal();
		numOfFile=0;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFileCompareDlg::OnBnClickedRadio(UINT msg)
{
	UpdateData(TRUE);
	if(searchType == 0)
	{
		GetDlgItem(IDC_SLIDER_DETAIL)->ShowWindow(FALSE);
		Invalidate();
	}
	else if(searchType == 1)
	{		
		GetDlgItem(IDC_SLIDER_DETAIL)->ShowWindow(TRUE);
		Invalidate();
	}
	UpdateData(FALSE);
}


void CFileCompareDlg::OnBnClickedGuiCheck()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CFileCompareDlg::OnNMReleasedcaptureSliderDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	detail = sliderDetail.GetPos();
	UpdateData(TRUE);
	*pResult = 0;
}

HBRUSH CFileCompareDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_RADIO_SEARCH_BASIC)
		{
			pDC->SetBkMode(TRANSPARENT);      // 배경투명
			//pDC->SetTextColor(RGB(0, 0, 0));  // 글자 색
			pDC->SetBkColor(RGB(255, 255, 255));  // 배경 색
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
		if(nID == IDC_RADIO_SEARCH_DETAIL)
		{
			pDC->SetBkMode(TRANSPARENT);      // 배경투명
			//pDC->SetTextColor(RGB(0, 0, 0));  // 글자 색
			pDC->SetBkColor(RGB(255, 255, 255));  // 배경 색
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


LRESULT CFileCompareDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;

	return nHit;

	return CDialogEx::OnNcHitTest(point);
}


void CFileCompareDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	GetDlgItem(IDC_RADIO_SEARCH_BASIC)->wndTopMost;
	CDialogEx::OnTimer(nIDEvent);
}

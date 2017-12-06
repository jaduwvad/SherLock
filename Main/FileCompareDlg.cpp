
// FileCompareDlg.cpp : ���� ����
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


// CFileCompareDlg ��ȭ ����


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


// CFileCompareDlg �޽��� ó����

BOOL CFileCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTimer(0,100,NULL);
	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CFileCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CFileCompareDlg::OnTvnSelchangedMfcshelltree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	fileTree.GetItemPath(searchScope);
	fault = TRUE;
	if(searchScope == "")
	{
		MessageBox(_T("�߸��� ��� �����Դϴ�."));
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
		MessageBox(_T("�˻������� �������� �ʾҽ��ϴ�. �˻������� ������ �ּ���."));
		return;
	}
	if(findPath == "")
	{
		MessageBox(_T("�˻� ��� ������ ���õ��� �ʾҽ��ϴ�. �˻��� ������ ������ �ּ���."));
		return;
	}
	if(searchType == 0)
	{
	//	MessageBox(_T("ã�� File : ")+targetFilePath+_T("\n\n")+_T("ã�� ��� : ")+searchScope+_T("\n\n")+_T("�˻� Type : ������ �˻�\n"));

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

		//MessageBox(_T("�˻� �Ϸ�!!"));
	}
	else if(searchType == 1)
	{
//		MessageBox(_T("ã�� File : ")+targetFilePath+_T("\n\n")+_T("ã�� ��� : ")+searchScope+_T("\n\n")+_T("�˻� Type : �ڼ��� �˻�\n"));

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CFileCompareDlg::OnNMReleasedcaptureSliderDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	detail = sliderDetail.GetPos();
	UpdateData(TRUE);
	*pResult = 0;
}

HBRUSH CFileCompareDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_RADIO_SEARCH_BASIC)
		{
			pDC->SetBkMode(TRANSPARENT);      // �������
			//pDC->SetTextColor(RGB(0, 0, 0));  // ���� ��
			pDC->SetBkColor(RGB(255, 255, 255));  // ��� ��
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
		if(nID == IDC_RADIO_SEARCH_DETAIL)
		{
			pDC->SetBkMode(TRANSPARENT);      // �������
			//pDC->SetTextColor(RGB(0, 0, 0));  // ���� ��
			pDC->SetBkColor(RGB(255, 255, 255));  // ��� ��
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


LRESULT CFileCompareDlg::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;

	return nHit;

	return CDialogEx::OnNcHitTest(point);
}


void CFileCompareDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	GetDlgItem(IDC_RADIO_SEARCH_BASIC)->wndTopMost;
	CDialogEx::OnTimer(nIDEvent);
}

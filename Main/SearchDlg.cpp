// SearchDlg.cpp : 구현 파일입니다.
//
#pragma once
#include "stdafx.h"
#include "FileCompare.h"
#include "SearchDlg.h"
#include "afxdialogex.h"
#include <math.h>



// SearchDlg 대화 상자입니다.
pSearchBasic pp=&SearchDlg::searchFileBasic;



IMPLEMENT_DYNAMIC(SearchDlg, CDialogEx)

	SearchDlg::SearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SearchDlg::IDD, pParent)
{

}

SearchDlg::~SearchDlg()
{
}

void SearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULTLIST, resultList);
	DDX_Control(pDX, IDC_PROGRESS, progressCtrl);
	DDX_Control(pDX, IDC_SEARCHBG, searchBg);
	DDX_Control(pDX, IDC_SEARCHCANCEL, stopBtn);
	DDX_Control(pDX, IDC_CLOSE, closeBtn);
}


BEGIN_MESSAGE_MAP(SearchDlg, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_RESULTLIST, &SearchDlg::OnLvnColumnclickResultlist)
	ON_NOTIFY(NM_DBLCLK, IDC_RESULTLIST, &SearchDlg::OnNMDblclkResultlist)
	ON_BN_CLICKED(IDC_CLOSE, &SearchDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_SEARCHCANCEL, &SearchDlg::OnBnClickedSearchcancel)
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// SearchDlg 메시지 처리기입니다.


void SearchDlg::searchFileBasic(CString dir)
{
	dir +=_T("\\*.*");
	CFileFind find;
	BOOL is = find.FindFile(dir);
//	SetDlgItemText(IDC_STATUS, _T("검색중 입니다."));
//	Invalidate();
	while(is)
	{
		if(fault == FALSE)
		{
			break;
		}
		is = find.FindNextFile();

		if(find.IsDots())
		{
			continue;
		}

		if(find.IsDirectory())
		{
			CString newDir=find.GetFilePath();
			(this->*pp)(newDir);
			//searchFileBasic(newDir);
		}
		else
		{
			///////////////////////////////////검색 방법
			if(searchType == 0)  ////////////// 기본 검색
			{
				if(progress == TRUE)
				{
					MSG msg;
					while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
					{
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
					numOfFile++;
				}
				else
				{
					numOfFile++;
					if(numOfFileTemp > 3000)
					{
						if(numOfFile > numOfFileTemp/100)
						{
							progressPos++;
							progressCtrl.SetPos(progressPos);
							numOfFile = 0;
							MSG msg;
							while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
							{
								::TranslateMessage(&msg);
								::DispatchMessage(&msg);
							}
					//		SetDlgItemText(IDC_STATUS, find.GetFilePath());
						}
					}
					else
					{
						progressCtrl.SetPos(numOfFile);
						MSG msg;
						while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
						{
							::TranslateMessage(&msg);
							::DispatchMessage(&msg);
						}
					//	SetDlgItemText(IDC_STATUS, find.GetFilePath());
					}


					CString newFileName = find.GetFileName();

					if(targetFileName==newFileName) // 이름 같음
					{
						double newFileSize = find.GetLength();
						//MessageBox(_T("이름!"));
						if(targetFileSize <= newFileSize+20 && targetFileSize >= newFileSize-20)
						{						
							CString newFilePath = find.GetFilePath();

							resultList.InsertItem(fileCnt,newFileName);
							resultList.SetItemText(fileCnt, 1, newFilePath);
							fileCnt++;
						}
					}
				}

			}
			else if(searchType == 1)// 자세히 검색
			{
				if(progress == TRUE)
				{
					MSG msg;
					while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
					{
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
					numOfFile++;
				}
				else
				{
					numOfFile++;
					if(numOfFileTemp > 1000)
					{
						if(numOfFile > numOfFileTemp/100)
						{
							progressPos++;
							progressCtrl.SetPos(progressPos);
							numOfFile = 0;
							MSG msg;
							while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
							{
								::TranslateMessage(&msg);
								::DispatchMessage(&msg);
							}
						//	SetDlgItemText(IDC_STATUS, find.GetFilePath());
						}
					}
					else
					{
						progressCtrl.SetPos(numOfFile);
						MSG msg;
						while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
						{
							::TranslateMessage(&msg);
							::DispatchMessage(&msg);
						}
				//		SetDlgItemText(IDC_STATUS, find.GetFilePath());
					}

					double newFileSize = find.GetLength();
					//MessageBox(_T("이름!"));

					if(targetFileSize <= (newFileSize+(targetFileSize*((100-detail)/1000))) && targetFileSize >= (newFileSize-(targetFileSize*((100-detail)/1000))))
					{
						CString newFileName = find.GetFileName();
						CString newFilePath = find.GetFilePath();
						CString tempFileSize;
						CString extendNew = newFilePath.Right(newFilePath.GetLength() - newFilePath.ReverseFind('.') - 1);
						CString extendOri = targetFileName.Right(targetFileName.GetLength() - targetFileName.ReverseFind('.') - 1);
						int crazyCount = 0;

						if(extendOri == extendNew)
						{
							resultList.InsertItem(fileCnt,newFileName);
							resultList.SetItemText(fileCnt, 1, newFilePath);
							tempFileSize.Format(_T("%.2f%%"), 100.0-abs(((targetFileSize-newFileSize)*100)/targetFileSize));
							resultList.SetItemText(fileCnt, 2, tempFileSize);
							fileCnt++;
						}
					}
				}

			}
		}
	}
}

BOOL SearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

/////////////////////////////////////////
	HBITMAP hBit=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_COMMONDLG));

	searchBg.SetBitmap(hBit);
	CRect rt;
	GetClientRect(&rt);
	searchBg.SetWindowPos(NULL,0,0,rt.Width(), rt.Height(),SWP_SHOWWINDOW);

	stopBtn.LoadBitmaps(IDB_STOPN, IDB_StopC, IDB_STOPN, IDB_STOPN);
	stopBtn.SizeToContent();
	stopBtn.SetHoverBitmapID(IDB_StopC);
	closeBtn.LoadBitmaps(IDB_BACK, IDB_BACK_C, IDB_BACK, IDB_BACK);
	closeBtn.SizeToContent();
	closeBtn.SetHoverBitmapID(IDB_BACK_C);

	//////////////////////////////////////////



	if(searchType == 0) // 기본검색
	{
		resultList.DeleteAllItems();   //리스트 초기화

		resultList.SetExtendedStyle(LVS_EX_FULLROWSELECT);   //리스트 스타일 초기화??

		resultList.InsertColumn(0, _T("파일명"), LVCFMT_LEFT, 200);   //항목추가1
		resultList.InsertColumn(1, _T("위치"), LVCFMT_LEFT, 400);   //항목추가2
		//resultList.InsertColumn(2, _T("/"), LVCFMT_LEFT, 100);   //항목추가3
	}
	else if(searchType == 1) // 자세히 검색
	{
		resultList.DeleteAllItems();   //리스트 초기화

		resultList.SetExtendedStyle(LVS_EX_FULLROWSELECT);   //리스트 스타일 초기화??

		resultList.InsertColumn(0, _T("파일명"), LVCFMT_LEFT, 100);   //항목추가1
		resultList.InsertColumn(1, _T("위치"), LVCFMT_LEFT, 330);   //항목추가2
		resultList.InsertColumn(2, _T("일치율"), LVCFMT_LEFT, 75);   //항목추가3
	}
	else // 초정밀 검색
	{
		resultList.DeleteAllItems();   //리스트 초기화

		resultList.SetExtendedStyle(LVS_EX_FULLROWSELECT);   //리스트 스타일 초기화??

		resultList.InsertColumn(0, _T("파일명"), LVCFMT_LEFT, 100);   //항목추가1
		resultList.InsertColumn(1, _T("위치"), LVCFMT_LEFT, 330);   //항목추가2
		resultList.InsertColumn(2, _T("일치율"), LVCFMT_LEFT, 70);   //항목추가3
	}

	fileCnt = 0;
	numOfFile = 0;
	if(isGui == 0)
	{
		progress = TRUE;
	}
	else
	{
		progress = FALSE;
		numOfFile = 4000;
	}

	timerCnt = 0;
	fault = TRUE;
	progressPos = 0;
	progressCtrl.SetRange(0,100);
	SetTimer(0, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void SearchDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(searchType==0)
	{
		for(int i = 0 ; i<2 ; i++)
		{
			progress = TRUE;
			if(searchScope == _T("C:\\") && cDriveCnt == 1)
			{
				numOfFile = cDriveFileCnt;
				if(i == 0)
				{
					continue;
				}
			}
			if(i==1)
			{
				if(searchScope == _T("C:\\") && cDriveCnt == 0)
				{
					cDriveFileCnt = numOfFile;
					cDriveCnt = 1;
				}
				progress = FALSE;
				numOfFileTemp=numOfFile;
				if(numOfFileTemp > 1000)
				{
					progressCtrl.SetRange(0, 100);
				}
				else
				{
					progressCtrl.SetRange(0, numOfFile);
				}
				numOfFile = 0;
				SetDlgItemText(IDC_STATUS, _T("검색중 입니다..."));
				Invalidate();
			}
			(this->*pp)(searchScope);

			if(fault == TRUE)
			{
				SetDlgItemText(IDC_STATUS, _T("검색이 완료되었습니다!!"));
				int x, y;
				progressCtrl.GetRange(x,y);
				progressCtrl.SetPos(y);
				Invalidate();
			}
			else
			{
				SetDlgItemText(IDC_STATUS, _T("검색이 중지되었습니다!!"));
				Invalidate();
			}
		}
	}
	else if(searchType == 1)
	{
		for(int i=0 ; i<2 ; i++)
		{
			progress = TRUE;
			if(searchScope == _T("C:\\") && cDriveCnt == 1)
			{
				numOfFile = cDriveFileCnt;
				if(i == 0)
				{
					continue;
				}
			}
			if(i==1)
			{
				if(searchScope == _T("C:\\") && cDriveCnt == 0)
				{
					cDriveFileCnt = numOfFile;
					cDriveCnt = 1;
				}
				progress = FALSE;
				numOfFileTemp = numOfFile;
				if(numOfFileTemp > 1000)
				{
					progressCtrl.SetRange(0, 100);
				}
				else
				{
					progressCtrl.SetRange(0, numOfFile);
				}
				numOfFile = 0;
				SetDlgItemText(IDC_STATUS, _T("검색중 입니다..."));
				Invalidate();
			}

			(this->*pp)(searchScope);

			if(fault == TRUE)
			{
				SetDlgItemText(IDC_STATUS, _T("검색이 완료되었습니다!!"));
				int x, y;
				progressCtrl.GetRange(x,y);
				progressCtrl.SetPos(y);
				Invalidate();
			}
			else
			{
				SetDlgItemText(IDC_STATUS, _T("검색이 중지되었습니다!!"));
				Invalidate();
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void SearchDlg::OnLvnColumnclickResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void SearchDlg::OnNMDblclkResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int itemindex = pNMListView->iItem;
	CString path;
	path = resultList.GetItemText(itemindex,1);
	int i = path.ReverseFind('\\');   //  이거슨 맨뒤에 있는 \\의 뒤에 부분 즉 Temp.dat 부분을 자른 나머지 부분의 길이를 리턴.
	path = path.Left(i)+_T("\\");
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOW);
	Invalidate();

	*pResult = 0;
}


void SearchDlg::OnBnClickedClose()
{
	OnOK();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void SearchDlg::OnBnClickedSearchcancel()
{
	fault = FALSE;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

HBRUSH SearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_STATUS)
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


LRESULT SearchDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;

	return nHit;

	return CDialogEx::OnNcHitTest(point);
}

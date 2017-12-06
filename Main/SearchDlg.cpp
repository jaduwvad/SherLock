// SearchDlg.cpp : ���� �����Դϴ�.
//
#pragma once
#include "stdafx.h"
#include "FileCompare.h"
#include "SearchDlg.h"
#include "afxdialogex.h"
#include <math.h>



// SearchDlg ��ȭ �����Դϴ�.
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


// SearchDlg �޽��� ó�����Դϴ�.


void SearchDlg::searchFileBasic(CString dir)
{
	dir +=_T("\\*.*");
	CFileFind find;
	BOOL is = find.FindFile(dir);
//	SetDlgItemText(IDC_STATUS, _T("�˻��� �Դϴ�."));
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
			///////////////////////////////////�˻� ���
			if(searchType == 0)  ////////////// �⺻ �˻�
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

					if(targetFileName==newFileName) // �̸� ����
					{
						double newFileSize = find.GetLength();
						//MessageBox(_T("�̸�!"));
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
			else if(searchType == 1)// �ڼ��� �˻�
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
					//MessageBox(_T("�̸�!"));

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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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



	if(searchType == 0) // �⺻�˻�
	{
		resultList.DeleteAllItems();   //����Ʈ �ʱ�ȭ

		resultList.SetExtendedStyle(LVS_EX_FULLROWSELECT);   //����Ʈ ��Ÿ�� �ʱ�ȭ??

		resultList.InsertColumn(0, _T("���ϸ�"), LVCFMT_LEFT, 200);   //�׸��߰�1
		resultList.InsertColumn(1, _T("��ġ"), LVCFMT_LEFT, 400);   //�׸��߰�2
		//resultList.InsertColumn(2, _T("/"), LVCFMT_LEFT, 100);   //�׸��߰�3
	}
	else if(searchType == 1) // �ڼ��� �˻�
	{
		resultList.DeleteAllItems();   //����Ʈ �ʱ�ȭ

		resultList.SetExtendedStyle(LVS_EX_FULLROWSELECT);   //����Ʈ ��Ÿ�� �ʱ�ȭ??

		resultList.InsertColumn(0, _T("���ϸ�"), LVCFMT_LEFT, 100);   //�׸��߰�1
		resultList.InsertColumn(1, _T("��ġ"), LVCFMT_LEFT, 330);   //�׸��߰�2
		resultList.InsertColumn(2, _T("��ġ��"), LVCFMT_LEFT, 75);   //�׸��߰�3
	}
	else // ������ �˻�
	{
		resultList.DeleteAllItems();   //����Ʈ �ʱ�ȭ

		resultList.SetExtendedStyle(LVS_EX_FULLROWSELECT);   //����Ʈ ��Ÿ�� �ʱ�ȭ??

		resultList.InsertColumn(0, _T("���ϸ�"), LVCFMT_LEFT, 100);   //�׸��߰�1
		resultList.InsertColumn(1, _T("��ġ"), LVCFMT_LEFT, 330);   //�׸��߰�2
		resultList.InsertColumn(2, _T("��ġ��"), LVCFMT_LEFT, 70);   //�׸��߰�3
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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void SearchDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
				SetDlgItemText(IDC_STATUS, _T("�˻��� �Դϴ�..."));
				Invalidate();
			}
			(this->*pp)(searchScope);

			if(fault == TRUE)
			{
				SetDlgItemText(IDC_STATUS, _T("�˻��� �Ϸ�Ǿ����ϴ�!!"));
				int x, y;
				progressCtrl.GetRange(x,y);
				progressCtrl.SetPos(y);
				Invalidate();
			}
			else
			{
				SetDlgItemText(IDC_STATUS, _T("�˻��� �����Ǿ����ϴ�!!"));
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
				SetDlgItemText(IDC_STATUS, _T("�˻��� �Դϴ�..."));
				Invalidate();
			}

			(this->*pp)(searchScope);

			if(fault == TRUE)
			{
				SetDlgItemText(IDC_STATUS, _T("�˻��� �Ϸ�Ǿ����ϴ�!!"));
				int x, y;
				progressCtrl.GetRange(x,y);
				progressCtrl.SetPos(y);
				Invalidate();
			}
			else
			{
				SetDlgItemText(IDC_STATUS, _T("�˻��� �����Ǿ����ϴ�!!"));
				Invalidate();
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void SearchDlg::OnLvnColumnclickResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void SearchDlg::OnNMDblclkResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int itemindex = pNMListView->iItem;
	CString path;
	path = resultList.GetItemText(itemindex,1);
	int i = path.ReverseFind('\\');   //  �̰Ž� �ǵڿ� �ִ� \\�� �ڿ� �κ� �� Temp.dat �κ��� �ڸ� ������ �κ��� ���̸� ����.
	path = path.Left(i)+_T("\\");
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOW);
	Invalidate();

	*pResult = 0;
}


void SearchDlg::OnBnClickedClose()
{
	OnOK();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void SearchDlg::OnBnClickedSearchcancel()
{
	fault = FALSE;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

HBRUSH SearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_STATUS)
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


LRESULT SearchDlg::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;

	return nHit;

	return CDialogEx::OnNcHitTest(point);
}

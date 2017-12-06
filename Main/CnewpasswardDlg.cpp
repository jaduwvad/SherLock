// CnewpasswardDlg.cpp : ���� �����Դϴ�.
//
#pragma once

#include "stdafx.h"
#include "Main.h"
#include "CnewpasswardDlg.h"
#include "afxdialogex.h"
#include "fileTreat.h"


// CnewpasswardDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CnewpasswardDlg, CDialogEx)

CnewpasswardDlg::CnewpasswardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CnewpasswardDlg::IDD, pParent)
{

}

CnewpasswardDlg::~CnewpasswardDlg()
{
}

void CnewpasswardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, f_list);
	DDX_Control(pDX, IDC_PREBG, preBg);
	DDX_Control(pDX, IDC_FILE_CHOICE, loadFileBtn);
	DDX_Control(pDX, IDC_FILE_DELETE, delFileBtn);
	DDX_Control(pDX, IDC_ADD_LOCK, closeBtn);
	DDX_Control(pDX, IDC_LIST_RESET, clearBtn);
}


BEGIN_MESSAGE_MAP(CnewpasswardDlg, CDialogEx)
//	ON_WM_INITMENUPOPUP()
	ON_BN_CLICKED(IDC_FILE_CHOICE, &CnewpasswardDlg::OnBnClickedFileChoice)
	ON_BN_CLICKED(IDC_FILE_DELETE, &CnewpasswardDlg::OnBnClickedFileDelete)
	ON_BN_CLICKED(IDC_LIST_RESET, &CnewpasswardDlg::OnBnClickedListReset)
	ON_BN_CLICKED(IDC_ADD_LOCK, &CnewpasswardDlg::OnBnClickedAddLock)
	ON_NOTIFY(NM_DBLCLK, IDC_VIEWLIST, &CnewpasswardDlg::OnNMDblclkResultlist)
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CnewpasswardDlg �޽��� ó�����Դϴ�.


BOOL CnewpasswardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	fileTreat filelist;

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.  
	filelist.OnFileload();
	//filelist.initlist(existFileList);

	////////////////////////////////////////////////////////////
	HBITMAP hBit=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_COMMONDLG2));
	preBg.SetBitmap(hBit);

	CRect rt;
	GetClientRect(&rt);
	preBg.SetWindowPos(NULL,0,0,rt.Width(), rt.Height(),SWP_SHOWWINDOW);

	closeBtn.LoadBitmaps(IDB_BACK, IDB_BACK_C, IDB_BACK, IDB_BACK);
	closeBtn.SizeToContent();
	closeBtn.SetHoverBitmapID(IDB_BACK_C);

	loadFileBtn.LoadBitmaps(IDB_E_LOADBTN_N, IDB_E_LOADBTN_C, IDB_E_LOADBTN_N, IDB_E_LOADBTN_N);
	loadFileBtn.SizeToContent();
	loadFileBtn.SetHoverBitmapID(IDB_E_LOADBTN_C);

	delFileBtn.LoadBitmaps(IDB_E_DELBTN_N,IDB_E_DELBTN_C,IDB_E_DELBTN_N,IDB_E_DELBTN_N);
	delFileBtn.SizeToContent();
	delFileBtn.SetHoverBitmapID(IDB_E_DELBTN_C);

	clearBtn.LoadBitmaps(IDB_ClearN,IDB_ClearC,IDB_ClearN,IDB_ClearN);
	clearBtn.SizeToContent();
	clearBtn.SetHoverBitmapID(IDB_ClearC);
	/////////////////////////////////////////////////////////////
	f_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	f_list.InsertColumn(0,_T("�����̸�"),LVCFMT_LEFT,100);
	f_list.InsertColumn(1,_T("���� ������"),LVCFMT_LEFT,300);//listcontrol column���� 

	SetTimer(0, 100, NULL); // ���ϵ巡�� ���� ������ üũ

	for(int i=0; i<512; i++){
		if(existFileList[i].conFileName=="")
			break;

		f_list.InsertItem(0,(existFileList[i].conFileName));
		f_list.SetItemText(0,1,(existFileList[i].conFilePath));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CnewpasswardDlg::OnBnClickedFileChoice()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog fileDlg(TRUE, _T("*.*"), _T("*.*"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("*.*"), this);
	fileTreat existlist;

	if(fileDlg.DoModal() == IDOK){
		fileName = fileDlg.GetFileName();
		filePath = fileDlg.GetPathName();
	}

	if(existlist.DataCheck(existFileList, fileName, filePath))
	{
		existlist.InsertData(existFileList, fileName, filePath);
		existlist.addFileMemory(filePath);

		f_list.InsertItem(0,(fileName));
		f_list.SetItemText(0,1,(filePath));


	}

	else if(fileName!="")
		MessageBox(_T("�̸��� ��ΰ� �ߺ��Դϴ�."));

	fileName="";
	filePath="";
}


void CnewpasswardDlg::OnBnClickedFileDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int buf;
	CString listpath;
	fileTreat existlist;

	buf=f_list.GetSelectionMark();
	listpath = f_list.GetItemText(buf,1);
	f_list.DeleteItem(buf);

	existlist.DeleteData(existFileList, listpath);
	existlist.AlignIndex(existFileList);
	existlist.listAditing(existFileList);
}

void CnewpasswardDlg::OnBnClickedListReset()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	fileTreat existfile;

	f_list.DeleteAllItems();
	existfile.DeleteAllData(existFileList);
}


void CnewpasswardDlg::OnBnClickedAddLock()
{
	OnCancel();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


LRESULT CnewpasswardDlg::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;

	return nHit;

	return CDialogEx::OnNcHitTest(point);
}


void CnewpasswardDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CString tempNamePath, selectedName, selectedPath;
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if(f_list.ifDrag == 1)
	{
		KillTimer(0);
		fileTreat existlist;
		CString loadFail[10];
		int failCnt = 0;
		for(int i = 0 ; i<10 ; i++)
		{
			loadFail[i] = _T("");
		}
		for(int i = 0;i < f_list.uCount ; i++)
		{		
			if(i > 9)
			{
				MessageBox(_T("������ �ִ� 10�������� �ѹ��� ��� �����մϴ�."));
				f_list.ifDrag = 0;
				SetTimer(0, 100, NULL);
				return;
			}
			tempNamePath = f_list.buffer[i];
			selectedPath = tempNamePath.Left(tempNamePath.ReverseFind('\\'));
			selectedName = tempNamePath.Right(tempNamePath.GetLength() - tempNamePath.ReverseFind('\\') - 1);
			if( selectedName.Left(selectedName.ReverseFind('.')) ==_T("") )
			{
				f_list.ifDrag = 0;
				goto label;
			}
				if(existlist.DataCheck(existFileList, selectedName, selectedPath))
				{
					selectedPath=selectedPath+"\\"+selectedName;
					existlist.InsertData(existFileList, selectedName, selectedPath);
					existlist.addFileMemory(selectedPath);

					f_list.InsertItem(0,(selectedName));
					f_list.SetItemText(0,1,(selectedPath));
				}
				else
				{
				//	MessageBox(_T("�̸��� ��ΰ� �ߺ��Դϴ�."));
					loadFail[failCnt] = selectedName;
					failCnt++;
					//MessageBox(selectedName+_T("\n\n���� ������ �̹� �����մϴ�."));
					f_list.ifDrag = 0;
					goto label;
				}

				fileName="";
				filePath="";
			for(int ii=0;ii<100;ii++)
			{
label:break;
			}
		}// while
		if(failCnt != 0)
		{
			CString failFiles=_T("");
			for(int i = 0 ; i<failCnt ; i++)
			{
				failFiles = failFiles + loadFail[i] + _T("\n");
			}
			MessageBox(_T("���� ���ϵ��� �ߺ��� ���ϵ��Դϴ�.\n\n") + failFiles);
		}
		f_list.ifDrag = 0;
		SetTimer(0, 100, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CnewpasswardDlg::OnNMDblclkResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int itemindex = pNMListView->iItem;
	CString path;
	path = f_list.GetItemText(itemindex,1);
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOW);
	*pResult = 0;
}

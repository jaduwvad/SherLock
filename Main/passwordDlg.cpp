
// passwordDlg.cpp : ���� ����
//
#pragma once
#include "stdafx.h"
#include "password.h"
#include "passwordDlg.h"
#include "afxdialogex.h"

#include "Crypt.h"
#include "FWAES.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPasswordDlg::IDD, pParent)
	, VIEW(_T(""))
	, m_strKeyValue(_T(""))
	, m_strIV(_T(""))
	, m_strFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDD_MAIN_DIALOG);
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEWLIST, VIEWLIST);
	DDX_Text(pDX, IDC_VIEW, VIEW);
	DDX_Control(pDX, IDC_PROGRESS1, cProgress);
	DDX_Control(pDX, IDC_CBG, encryptBG);
	DDX_Control(pDX, IDC_VIEWOPT, changeBtn);
	DDX_Control(pDX, IDCANCEL, back);
	DDX_Control(pDX, IDC_ADD, e_LoadBtn);
	DDX_Control(pDX, IDOK, e_EnCBtn);
	DDX_Control(pDX, IDC_DELETE, e_DelBtn);
}

BEGIN_MESSAGE_MAP(CPasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPasswordDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_VIEWLIST, &CPasswordDlg::OnLvnItemchangedViewlist)
	ON_BN_CLICKED(IDC_ADD, &CPasswordDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &CPasswordDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_VIEWOPT, &CPasswordDlg::OnBnClickedViewopt)
	ON_NOTIFY(NM_DBLCLK, IDC_VIEWLIST, &CPasswordDlg::OnNMDblclkResultlist)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CpasswordDlg �޽��� ó����

BOOL CPasswordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	buttonCount = 1;
	VIEW = "VIEW : ALL";
	UpdateData(FALSE);

	//////////////////////////////////////////////////////////////////////////////
	
	HBITMAP hBit=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_COMMONDLG));

	encryptBG.SetBitmap(hBit);
	CRect rt;
	GetClientRect(&rt);
	encryptBG.SetWindowPos(NULL,0,0,rt.Width(), rt.Height(),SWP_SHOWWINDOW);

	/*
	HRGN h = BitmapToRegion(hBit,RGB(0,0,0), RGB(0,0,0));
	if (h)
		SetWindowRgn(h, TRUE);
	*/
	changeBtn.LoadBitmaps(IDB_CHANGE, IDB_CHANGE_C, IDB_CHANGE, IDB_CHANGE);
	changeBtn.SizeToContent();
	changeBtn.SetHoverBitmapID(IDB_CHANGE_C);
	back.LoadBitmaps(IDB_BACK, IDB_BACK_C, IDB_BACK, IDB_BACK);
	back.SizeToContent();
	back.SetHoverBitmapID(IDB_BACK_C);

	e_LoadBtn.LoadBitmaps(IDB_E_LOADBTN_N, IDB_E_LOADBTN_C, IDB_E_LOADBTN_N, IDB_E_LOADBTN_N);
	e_LoadBtn.SizeToContent();
	e_LoadBtn.SetHoverBitmapID(IDB_E_LOADBTN_C);
	e_EnCBtn.LoadBitmaps(IDB_E_ENDBTN_N,IDB_E_ENDBTN_C,IDB_E_ENDBTN_N,IDB_E_ENDBTN_N);
	e_EnCBtn.SizeToContent();
	e_EnCBtn.SetHoverBitmapID(IDB_E_ENDBTN_C);
	e_DelBtn.LoadBitmaps(IDB_E_DELBTN_N,IDB_E_DELBTN_C,IDB_E_DELBTN_N,IDB_E_DELBTN_N);
	e_DelBtn.SizeToContent();
	e_DelBtn.SetHoverBitmapID(IDB_E_DELBTN_C);

	//////////////////////////////////////////////////////////////////////////////

	VIEWLIST.DeleteAllItems();   //����Ʈ �ʱ�ȭ

	VIEWLIST.SetExtendedStyle(LVS_EX_FULLROWSELECT);   //����Ʈ ��Ÿ�� �ʱ�ȭ??

	VIEWLIST.InsertColumn(0, _T("���ϸ�"), LVCFMT_LEFT/*�׸��̸� ����: CENTER, RIGHT, LEFT etc...*/, 100/*�׸� ũ��*/);   //�׸��߰�1
	VIEWLIST.InsertColumn(1, _T("��ġ"), LVCFMT_LEFT, 300);   //�׸��߰�2
	VIEWLIST.InsertColumn(2, _T("��ȣȭ"), LVCFMT_LEFT, 60);   //�׸��߰�3

	//-----------------------------------��ȣȭ ��� ����-----------------------------------//
	m_nRound = 5;																			//
	m_strKeyValue = _T("12345678");															//
	m_strIV = _T("All Zero 128 bits");														//
	//
	UpdateData(FALSE);																		//		
	//--------------------------------------------------------------------------------------//

	SetTimer(0, 100, NULL); // ���ϵ巡�� ���� ������ üũ

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


//////////////////////////////////��ȣȭ,��ȣȭ/////////////////////////////////////
void CPasswordDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	for(int i=0 ; i<VIEWLIST.GetItemCount() ; i++)
	{
		VIEWLIST.SetSelectionMark(i);
		VIEWLIST.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		//	VIEWLIST.SetFocus();
	}

	POSITION pos = VIEWLIST.GetFirstSelectedItemPosition();

	//-----------------���α׷��� ��Ʈ�� ����-------------------//
	int itemCount;												//
	itemCount = VIEWLIST.GetSelectedCount();					//
	cProgress.SetRange(0, itemCount);							//
	cProgress.SetStep(1);										//
	cProgress.SetPos(0);
	//----------------------------------------------------------//

	if ( VIEWLIST.GetSelectedCount() <= 0 )
	{
		AfxMessageBox(_T("������ �����ϼ���."));
		return;
	}

	while(pos)
	{
		int itemPosition;
		CString itemName;
		CString itemPath;
		CString itemExtSave;
		int extSap;
		itemPosition = VIEWLIST.GetNextSelectedItem(pos);
		itemName =VIEWLIST.GetItemText(itemPosition, 0);
		itemPath =VIEWLIST.GetItemText(itemPosition, 1);//����Ʈ���� ���ϸ�, ��� ������



		m_strFile = itemPath + "\\" + itemName;
		//MessageBox(_T("���ϰ���̸�") + m_strFile);//�ΰ� ���ļ� PathName����>>m_strFile�� ����
		m_strFile.Replace("\\", "\\\\");

		extSap=itemName.GetLength();

		itemExtSave=itemName.Right(extSap-itemName.Find("."));
		//MessageBox(_T("����Ȯ����")+itemExtSave);//Ȯ���� ����

		/*for(int i=0; i<itemCount; i++)
		{
		Sleep(200);
		cProgress.StepIt();
		}*/

		mode_t mode;

		mode = CBC;

		CFileException ex;
		CFile src, dst;
		DWORD nLength;
		uchar8_t *pPlainText;
		uchar8_t *pCipherText;

		CString strCreatedFile;

		int findExtention;
		findExtention = m_strFile.Find(".Goo");//Ȯ���ڰ� Goo���� Ȯ��

		//LockingFile(m_strFile, itemExtSave);

		///////////////////////////////////////AES����////////////////////////////////////////
		if(findExtention == -1)
		{
			//		MessageBox(_T("��ȣȭ"));
			CString m_strOriKey = m_strKeyValue;

			m_strKeyValue = m_strKeyValue.Left(8);
			m_strKeyValue += m_strKeyValue;

			///����ó��//
			if ( !src.Open(m_strFile, CFile::modeRead|CFile::shareDenyWrite, &ex) )
			{
				TCHAR szMsg[1024];
				ex.GetErrorMessage(szMsg, 1024);
				AfxMessageBox(szMsg);
				return;
			}

			CString strName = src.GetFileName();
			CString strTitle;
			CString strExt;
			int nPos;
			if ( (nPos = strName.ReverseFind(_T('.'))) == -1 ) {
				strTitle = strName;
				strExt = _T("");
			} else {
				strTitle = strName.Left(nPos);
				strExt = strName.Right(strName.GetLength()-nPos-1);
			}
			CString strPath = src.GetFilePath();
			nPos = strPath.ReverseFind(_T('\\'));
			strPath = strPath.Left(nPos);
			nLength = src.GetLength();

			uint32_t nPlainTextLen = nLength;
			uint32_t nCipherTextLen;

			pPlainText = new uchar8_t[(nLength/16+1)*16+1];
			pCipherText = new uchar8_t[(nLength/16+1)*16+1];

			memset(pPlainText, NULL, (nLength/16+1)*16+1);
			memset(pCipherText, NULL, (nLength/16+1)*16+1);

			DWORD dwBytes;
			DWORD dwTotal = 0;

			do {
				dwBytes = src.Read(pPlainText+dwTotal, nLength);
				dwTotal += dwBytes;
			} while ( dwTotal != nLength );
			MSG msg;
			while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}


			src.Close();

			CFWAES aes;
			aes.Cipher(mode, (uchar8_t*)(LPCTSTR)m_strKeyValue, nPlainTextLen, pPlainText, &nCipherTextLen, pCipherText, m_nRound);


			/////////////////////////////////////�����̸� ����//////////////////////////////////////
			CString ext;
			ext.Format(".Goo");//Ȯ���� ����
			//		MessageBox(ext);

			CString strDstFile = strPath + _T('\\') + strTitle + itemExtSave +ext;//�̸�+���+Ȯ����1+Ȯ����2�� ����
			//	MessageBox(_T("���")+strPath);
			//	MessageBox(_T("�̸�") +strTitle);
			//	MessageBox(_T("Ȯ����1")+itemExtSave);
			//	MessageBox(_T("Ȯ����2")+ext);

			//	MessageBox(strDstFile);

			strCreatedFile = strTitle + itemExtSave + ext;

			if ( !dst.Open(strDstFile, CFile::modeWrite|CFile::shareExclusive|CFile::modeCreate, &ex) ) {
				TCHAR szMsg[1024];
				ex.GetErrorMessage(szMsg, 1024);
				AfxMessageBox(szMsg);
				return;
			}

			dst.Write(pCipherText, nCipherTextLen);
			dst.Close();

			delete [] pPlainText;
			delete [] pCipherText;

			m_strKeyValue = m_strOriKey;

			for (int i=0; i<100; i++)
			{	
				if(data[i].name==strName && data[i].path==strPath)
				{	
					data[i].name=strCreatedFile;
					data[i].state=_T("O");
				}
			}
			//		AfxMessageBox(_T("��ȣȭ �۾��� �Ϸ�Ǿ����ϴ�.\n\n������ ����: [") + strCreatedFile + _T("]"));
			VIEWLIST.SetItemText(itemPosition, 0, strCreatedFile);
			VIEWLIST.SetItemText(itemPosition, 2, _T("O"));
		}
		///////////////////////////////��ȣȭ///////////////////////////////////
		else
		{
			//	MessageBox(_T("��ȣȭ"));

			if ( !src.Open(m_strFile, CFile::modeRead|CFile::shareDenyWrite, &ex) ) {
				TCHAR szMsg[1024];
				ex.GetErrorMessage(szMsg, 1024);
				AfxMessageBox(szMsg);
				return;
			}
			CString strName = src.GetFileName();
			CString strTitle;
			CString strExt;
			int nPos;
			if ( (nPos = strName.ReverseFind(_T('.'))) == -1 ) {
				strTitle = strName;
				strExt = _T("");
			} else {
				strTitle = strName.Left(nPos);
				strExt = strName.Right(strName.GetLength()-nPos-1);
			}
			CString strPath = src.GetFilePath();
			nPos = strPath.ReverseFind(_T('\\'));
			strPath = strPath.Left(nPos);
			nLength = src.GetLength();

			//		MessageBox(_T("���ϰ��") +strPath);
			//		MessageBox(_T("���ϸ�") +strTitle);
			//		MessageBox(_T("���ϸ�+Ȯ����") +strName);


			mode_t mode;
			mode = CBC;


			///////////////////////////////////AES��ȣȭ ����//////////////////////////////
			CString m_strOriKey = m_strKeyValue;

			m_strKeyValue = m_strKeyValue.Left(8);
			m_strKeyValue += m_strKeyValue;

			uint32_t nPlainTextLen;
			uint32_t nCipherTextLen = nLength;

			pPlainText = new uchar8_t[(nLength/16+1)*16+1];
			pCipherText = new uchar8_t[(nLength/16+1)*16+1];

			memset(pPlainText, NULL, (nLength/16+1)*16+1);
			memset(pCipherText, NULL, (nLength/16+1)*16+1);

			DWORD dwBytes;
			DWORD dwTotal = 0;
			do {
				dwBytes = src.Read(pCipherText+dwTotal, nLength);
				dwTotal += dwBytes;
			} while ( dwTotal != nLength );
			MSG msg;
			while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			src.Close();

			CFWAES aes;
			aes.Decipher(mode, (uchar8_t*)(LPCTSTR)m_strKeyValue, nCipherTextLen, pCipherText, &nPlainTextLen, pPlainText, m_nRound);


			////////////////////////////���� �̸� ����/////////////////////////////
			CString strDstFile = strPath + _T('\\') + strTitle;

			strCreatedFile = strTitle;


			if ( !dst.Open(strDstFile, CFile::modeWrite|CFile::shareExclusive|CFile::modeCreate, &ex) ) {
				TCHAR szMsg[1024];
				ex.GetErrorMessage(szMsg, 1024);
				AfxMessageBox(szMsg);
				return;
			}

			dst.Write(pPlainText, nPlainTextLen);
			dst.Close();

			delete [] pPlainText;
			delete [] pCipherText;

			m_strKeyValue = m_strOriKey;
			//	AfxMessageBox(_T("��ȣȭ �۾��� �Ϸ�Ǿ����ϴ�.\n\n������ ����: [") + strCreatedFile + _T("]"));

			for (int i=0; i<100; i++)
			{	
				if(data[i].name==strName && data[i].path==strPath)
				{	
					data[i].name=strTitle;
					data[i].state = _T("X");
				}
			}
			VIEWLIST.SetItemText(itemPosition, 0, strTitle);
			VIEWLIST.SetItemText(itemPosition, 2, _T("X"));

		}
		DeleteFile(m_strFile);		
		cProgress.StepIt();
		MSG msg;
		while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}//while
	MessageBox(_T("��ȣȭ �� ��ȣȭ �۾��� ��� �Ϸ�Ǿ����ϴ�."));
}


void CPasswordDlg::OnLvnItemchangedViewlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}





////////////////////////////////////////////�����߰�///////////////////////////////////
void CPasswordDlg::OnBnClickedAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	CFileDialog fileDlg(TRUE, _T("*.*"), _T("*.*")
		, OFN_HIDEREADONLY| OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT , _T("*.*"), this);

	POSITION pos;
	CString tempNamePath;
	CString selectedName;
	CString selectedPath;
	ULONGLONG tempSize;
	

	/*char buffer[4096] = {0}; 
	fileDlg.m_ofn.lpstrFile = buffer; 
	fileDlg.m_ofn.nMaxFile = 4096; */

	if(fileDlg.DoModal() == IDOK)
	{ 
		pos = fileDlg.GetStartPosition();
		int selectCnt = 0;
		CString loadFail[10];
		int failCnt = 0;
		for(int i = 0 ; i<10 ; i++)
		{
			loadFail[i] = _T("");
		}
		while(pos)
		{
			selectCnt++;
			if(selectCnt > 10)
			{
				MessageBox(_T("������ �ִ� 10�������� �ѹ��� ��� �����մϴ�."));
				return;
			}
			tempNamePath = fileDlg.GetNextPathName(pos);
			selectedPath = tempNamePath.Left(tempNamePath.ReverseFind('\\'));
			selectedName = tempNamePath.Right(tempNamePath.GetLength() - tempNamePath.ReverseFind('\\') - 1);

			CFile temp;
			temp.Open(tempNamePath, CFile::modeRead);

			tempSize=temp.GetLength();

			
			for(int i=0;i<100;i++)
			{

				if(data[i].name==selectedName && data[i].path==selectedPath)
				{
					loadFail[failCnt] = selectedName;
					failCnt++;
					//MessageBox(_T("���� ������ �̹� �����մϴ�."));
					goto label;
				}
				if(tempSize > 20000000|| tempSize < 0)
				{
					loadFail[failCnt] = selectedName;
					failCnt++;
					//MessageBox(_T("���� ������ �̹� �����մϴ�."));
					goto label;
				}
			}
			for(int i=0;i<100;i++)
			{
				if(data[i].name=="")
				{
					VIEWLIST.InsertItem(0,(selectedName));
					VIEWLIST.SetItemText(0,1,(selectedPath));
					data[i].name=selectedName;
					data[i].path=selectedPath;
					int findExtention;
					findExtention = selectedName.Find(".Goo");
					if(findExtention == -1)
					{
						VIEWLIST.SetItemText(0,2,_T("X"));
						data[i].state = _T("X");
					}
					else
					{
						VIEWLIST.SetItemText(0,2,(_T("O")));
						data[i].state=_T("O");
					}
label : break;
				}
			}
		}//while
		if(failCnt != 0)
		{
			CString failFiles=_T("");
			for(int i = 0 ; i<failCnt ; i++)
			{
				failFiles = failFiles + loadFail[i] + _T("\n");
			}
			MessageBox(_T("���� ���ϵ��� �ߺ��� ���� Ȥ�� �뷮�� ū ���ϵ��Դϴ�.\n\n") + failFiles);
		}
	}
}




////////////////////////////////////////////���ϻ���///////////////////////////////////////////
void CPasswordDlg::OnBnClickedDelete()
{
	POSITION pos = VIEWLIST.GetFirstSelectedItemPosition();

	while (pos != NULL)
	{
		int itemPosition;
		CString itemName;
		CString itemPath;

		itemPosition = VIEWLIST.GetNextSelectedItem(pos);

		itemName = VIEWLIST.GetItemText(itemPosition, 0);
		itemPath = VIEWLIST.GetItemText(itemPosition, 1);

		for (int i=0; i<100; i++)
		{	
			if(data[i].name==itemName && data[i].path==itemPath)
			{	
				data[i].name="";
				data[i].path="";
			}
		}
		VIEWLIST.DeleteItem(itemPosition);

		pos = VIEWLIST.GetFirstSelectedItemPosition();
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}




////////////////////////////////////////////���͸�////////////////////////////////////////////////
void CPasswordDlg::OnBnClickedViewopt()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	buttonCount++;

	if(buttonCount>3)
		buttonCount = 1;

	ViewControl(buttonCount);
	Invalidate();
	UpdateData(FALSE);
}



/////////////////////////////////////////���͸� �Լ�//////////////////////////////////////////////
void CPasswordDlg::ViewControl(int buttonNum)
{
	CString selectedName;
	CString selectedPath;
	CString selectedState;

	VIEWLIST.DeleteAllItems();

	switch (buttonNum)
	{
		//////////////////////////***ViewAll***///////////////////////
	case 1: 
		{
			UpdateData(TRUE);
			VIEW="VIEW : ALL";
			
			for(int i=0; i<100; i++)
			{
				if(data[i].name != "")
				{
					selectedName = data[i].name;
					selectedPath = data[i].path;
					selectedState = data[i].state;

					VIEWLIST.InsertItem(0,(selectedName));
					VIEWLIST.SetItemText(0,1,(selectedPath));
					VIEWLIST.SetItemText(0,2,(selectedState));
				}
			}
			buttonCount = 1;
			UpdateData(FALSE);
			break;
		}


		////////////////////////***��ȣȭ�� ����***/////////////////////
	case 2: 
		{
			UpdateData(TRUE);
			int findExtention;
			VIEW="VIEW : ��ȣȭ �� ����";
			
			for(int i=0; i<100; i++)
			{
				if(data[i].name != "")
				{
					findExtention = data[i].name.Find(".Goo");

					if(findExtention == -1)
					{
						selectedName = data[i].name;
						selectedPath = data[i].path;
						selectedState = data[i].state;

						VIEWLIST.InsertItem(0,(selectedName));
						VIEWLIST.SetItemText(0,1,(selectedPath));
						VIEWLIST.SetItemText(0,2,(selectedState));
					}
				}
			}
			buttonCount = 2;
			UpdateData(FALSE);
			break;
		}


		//////////////////***��ȣȭ�� ����***///////////////
	case 3: 
		{	
			UpdateData(TRUE);
			int findExtention;
			VIEW="VIEW : ��ȣȭ �� ����";
			
			for(int i=0; i<100; i++)
			{
				if(data[i].name != "")
				{
					findExtention = data[i].name.Find(".Goo");

					if(findExtention != -1)
					{
						selectedName = data[i].name;
						selectedPath = data[i].path;
						selectedState = data[i].state;

						VIEWLIST.InsertItem(0,(selectedName));
						VIEWLIST.SetItemText(0,1,(selectedPath));
						VIEWLIST.SetItemText(0,2,(selectedState));
					}
				}
			}
			buttonCount = 3;
			UpdateData(FALSE);
			break;
		}
	}
}




void CPasswordDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CString tempNamePath, selectedName, selectedPath;
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if(VIEWLIST.ifDrag == 1)
	{
		KillTimer(0);
		CString loadFail[10];
		int failCnt = 0;
		for(int i = 0 ; i<10 ; i++)
		{
			loadFail[i] = _T("");
		}
		for(int i = 0;i < VIEWLIST.uCount ; i++)
		{		
			if(i > 9)
			{
				MessageBox(_T("������ �ִ� 10�������� �ѹ��� ��� �����մϴ�."));
				VIEWLIST.ifDrag = 0;
				SetTimer(0, 100, NULL);
				return;
			}
			tempNamePath = VIEWLIST.buffer[i];
			selectedPath = tempNamePath.Left(tempNamePath.ReverseFind('\\'));
			selectedName = tempNamePath.Right(tempNamePath.GetLength() - tempNamePath.ReverseFind('\\') - 1);
			ULONGLONG tempSize;
			CFile temp;
			temp.Open(tempNamePath, CFile::modeRead);

			tempSize=temp.GetLength();
			
			if( selectedName.Left(selectedName.ReverseFind('.')) ==_T("") )
			{
				VIEWLIST.ifDrag = 0;
				goto label;
			}
			for(int ii=0;ii<100;ii++)
			{

				if(data[ii].name==selectedName && data[ii].path==selectedPath)
				{
					loadFail[failCnt] = selectedName;
					failCnt++;
					//MessageBox(selectedName+_T("\n\n���� ������ �̹� �����մϴ�."));
					VIEWLIST.ifDrag = 0;
					goto label;
				}
				if(tempSize > 20000000|| tempSize < 0)
				{
					loadFail[failCnt] = selectedName;
					failCnt++;
					//MessageBox(_T("���� ������ �̹� �����մϴ�."));
					goto label;
				}
			}
			for(int ii=0;ii<100;ii++)
			{
				if(data[ii].name=="")
				{
					VIEWLIST.InsertItem(0,(selectedName));
					VIEWLIST.SetItemText(0,1,(selectedPath));
					data[ii].name=selectedName;
					data[ii].path=selectedPath;
					int findExtention;
					findExtention = selectedName.Find(".Goo");
					if(findExtention == -1)
					{
						VIEWLIST.SetItemText(0,2,_T("X"));
						data[ii].state = _T("X");
					}
					else
					{
						VIEWLIST.SetItemText(0,2,(_T("O")));
						data[ii].state = _T("O");
					}
label : break;
				}
			}
		}// while
		if(failCnt != 0)
		{
			CString failFiles=_T("");
			for(int i = 0 ; i<failCnt ; i++)
			{
				failFiles = failFiles + loadFail[i] + _T("\n");
			}
			MessageBox(_T("���� ���ϵ��� �ߺ��� ���� Ȥ�� �뷮�� �ʹ� ū ���ϵ��Դϴ�.\n\n") + failFiles);
		}
		VIEWLIST.ifDrag = 0;
		SetTimer(0, 100, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CPasswordDlg::OnNMDblclkResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int itemindex = pNMListView->iItem;
	CString path;
	path = VIEWLIST.GetItemText(itemindex,1);
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOW);
	*pResult = 0;
}

HBRUSH CPasswordDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_VIEW)
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


HRGN CPasswordDlg::BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor , COLORREF cTolerance )
{
	//cTolerance = 0x101010;
	HRGN hRgn = NULL;
	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap content
		HDC hMemDC = CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			// Get bitmap size
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);
			// Create a 32 bits depth bitmap and select it into the memory DC 
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = { 
				sizeof(BITMAPINFOHEADER), // biSize 
				bm.bmWidth,     // biWidth; 
				bm.bmHeight,    // biHeight; 
				1,       // biPlanes; 
				32,       // biBitCount 
				BI_RGB,      // biCompression; 
				0,       // biSizeImage; 
				0,       // biXPelsPerMeter; 
				0,       // biYPelsPerMeter; 
				0,       // biClrUsed; 
				0       // biClrImportant; 
			};
			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);
				// Create a DC just to copy the bitmap into the memory DC
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;
					// Copy the bitmap into the memory DC
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);
					// For better performances, we will use the ExtCreateRegion() function to create the
					// region. This function take a RGNDATA structure on entry. We will add rectangles by
					// amount of ALLOC_UNIT number in this structure.
#define ALLOC_UNIT 100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
					// Keep on hand highest and lowest values for the "transparent" pixels
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));
					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to right
						for (int x = 0; x < bm.bmWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}
							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;
								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}
						// Go to next row (remember, the bitmap is inverted vertically)
						p32 -= bm32.bmWidthBytes;
					}
					// Create or extend the region with the remaining rectangles
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;
					// Clean up
					GlobalFree(hData);
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}
				DeleteObject(SelectObject(hMemDC, holdBmp));
			}
			DeleteDC(hMemDC);
		} 
	}
	return hRgn;
}



LRESULT CPasswordDlg::OnNcHitTest(CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;

	return nHit;
	return CDialogEx::OnNcHitTest(point);
}

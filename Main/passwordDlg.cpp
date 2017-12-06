
// passwordDlg.cpp : 구현 파일
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


// CpasswordDlg 메시지 처리기

BOOL CPasswordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

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

	VIEWLIST.DeleteAllItems();   //리스트 초기화

	VIEWLIST.SetExtendedStyle(LVS_EX_FULLROWSELECT);   //리스트 스타일 초기화??

	VIEWLIST.InsertColumn(0, _T("파일명"), LVCFMT_LEFT/*항목이름 정렬: CENTER, RIGHT, LEFT etc...*/, 100/*항목 크기*/);   //항목추가1
	VIEWLIST.InsertColumn(1, _T("위치"), LVCFMT_LEFT, 300);   //항목추가2
	VIEWLIST.InsertColumn(2, _T("암호화"), LVCFMT_LEFT, 60);   //항목추가3

	//-----------------------------------암호화 기능 관련-----------------------------------//
	m_nRound = 5;																			//
	m_strKeyValue = _T("12345678");															//
	m_strIV = _T("All Zero 128 bits");														//
	//
	UpdateData(FALSE);																		//		
	//--------------------------------------------------------------------------------------//

	SetTimer(0, 100, NULL); // 파일드래그 여부 지속적 체크

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


//////////////////////////////////암호화,복호화/////////////////////////////////////
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

	//-----------------프로그레스 컨트롤 영역-------------------//
	int itemCount;												//
	itemCount = VIEWLIST.GetSelectedCount();					//
	cProgress.SetRange(0, itemCount);							//
	cProgress.SetStep(1);										//
	cProgress.SetPos(0);
	//----------------------------------------------------------//

	if ( VIEWLIST.GetSelectedCount() <= 0 )
	{
		AfxMessageBox(_T("파일을 선택하세요."));
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
		itemPath =VIEWLIST.GetItemText(itemPosition, 1);//리스트에서 파일명, 경로 가져옴



		m_strFile = itemPath + "\\" + itemName;
		//MessageBox(_T("파일경로이름") + m_strFile);//두개 합쳐서 PathName만듬>>m_strFile에 넣음
		m_strFile.Replace("\\", "\\\\");

		extSap=itemName.GetLength();

		itemExtSave=itemName.Right(extSap-itemName.Find("."));
		//MessageBox(_T("파일확장자")+itemExtSave);//확장자 저장

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
		findExtention = m_strFile.Find(".Goo");//확장자가 Goo인지 확인

		//LockingFile(m_strFile, itemExtSave);

		///////////////////////////////////////AES시작////////////////////////////////////////
		if(findExtention == -1)
		{
			//		MessageBox(_T("암호화"));
			CString m_strOriKey = m_strKeyValue;

			m_strKeyValue = m_strKeyValue.Left(8);
			m_strKeyValue += m_strKeyValue;

			///에러처리//
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


			/////////////////////////////////////파일이름 지정//////////////////////////////////////
			CString ext;
			ext.Format(".Goo");//확장자 설정
			//		MessageBox(ext);

			CString strDstFile = strPath + _T('\\') + strTitle + itemExtSave +ext;//이름+경로+확장자1+확장자2로 저장
			//	MessageBox(_T("경로")+strPath);
			//	MessageBox(_T("이름") +strTitle);
			//	MessageBox(_T("확장자1")+itemExtSave);
			//	MessageBox(_T("확장자2")+ext);

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
			//		AfxMessageBox(_T("암호화 작업이 완료되었습니다.\n\n생성된 파일: [") + strCreatedFile + _T("]"));
			VIEWLIST.SetItemText(itemPosition, 0, strCreatedFile);
			VIEWLIST.SetItemText(itemPosition, 2, _T("O"));
		}
		///////////////////////////////복호화///////////////////////////////////
		else
		{
			//	MessageBox(_T("복호화"));

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

			//		MessageBox(_T("파일경로") +strPath);
			//		MessageBox(_T("파일명") +strTitle);
			//		MessageBox(_T("파일명+확장자") +strName);


			mode_t mode;
			mode = CBC;


			///////////////////////////////////AES복호화 시작//////////////////////////////
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


			////////////////////////////파일 이름 지정/////////////////////////////
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
			//	AfxMessageBox(_T("복호화 작업이 완료되었습니다.\n\n생성된 파일: [") + strCreatedFile + _T("]"));

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
	MessageBox(_T("암호화 및 복호화 작업이 모두 완료되었습니다."));
}


void CPasswordDlg::OnLvnItemchangedViewlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}





////////////////////////////////////////////파일추가///////////////////////////////////
void CPasswordDlg::OnBnClickedAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


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
				MessageBox(_T("파일은 최대 10개까지만 한번에 등록 가능합니다."));
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
					//MessageBox(_T("같은 파일이 이미 존재합니다."));
					goto label;
				}
				if(tempSize > 20000000|| tempSize < 0)
				{
					loadFail[failCnt] = selectedName;
					failCnt++;
					//MessageBox(_T("같은 파일이 이미 존재합니다."));
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
			MessageBox(_T("다음 파일들은 중복된 파일 혹은 용량이 큰 파일들입니다.\n\n") + failFiles);
		}
	}
}




////////////////////////////////////////////파일삭제///////////////////////////////////////////
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

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}




////////////////////////////////////////////필터링////////////////////////////////////////////////
void CPasswordDlg::OnBnClickedViewopt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	buttonCount++;

	if(buttonCount>3)
		buttonCount = 1;

	ViewControl(buttonCount);
	Invalidate();
	UpdateData(FALSE);
}



/////////////////////////////////////////필터링 함수//////////////////////////////////////////////
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


		////////////////////////***암호화할 파일***/////////////////////
	case 2: 
		{
			UpdateData(TRUE);
			int findExtention;
			VIEW="VIEW : 암호화 할 파일";
			
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


		//////////////////***복호화할 파일***///////////////
	case 3: 
		{	
			UpdateData(TRUE);
			int findExtention;
			VIEW="VIEW : 복호화 할 파일";
			
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString tempNamePath, selectedName, selectedPath;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
				MessageBox(_T("파일은 최대 10개까지만 한번에 등록 가능합니다."));
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
					//MessageBox(selectedName+_T("\n\n같은 파일이 이미 존재합니다."));
					VIEWLIST.ifDrag = 0;
					goto label;
				}
				if(tempSize > 20000000|| tempSize < 0)
				{
					loadFail[failCnt] = selectedName;
					failCnt++;
					//MessageBox(_T("같은 파일이 이미 존재합니다."));
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
			MessageBox(_T("다음 파일들은 중복된 파일 혹은 용량이 너무 큰 파일들입니다.\n\n") + failFiles);
		}
		VIEWLIST.ifDrag = 0;
		SetTimer(0, 100, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CPasswordDlg::OnNMDblclkResultlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// TODO:  여기서 DC의 특성을 변경합니다.

	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_VIEW)
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;

	return nHit;
	return CDialogEx::OnNcHitTest(point);
}

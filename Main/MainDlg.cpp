
// MainDlg.cpp : 구현 파일
//
#pragma once
#include "stdafx.h"
#include "Main.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "PasswordMain.h"
#include <windows.h>
#include <assert.H>
#include "CreatePassword.h"
#include "ModifyPassword.h"
#include "CnewpasswardDlg.h"
#include "fileTreat.h"
#include "FileCompare.h"
#include "FileCompareDlg.h"
#include "passwordDlg.h"
#include "afxwin.h"
#include "jhbitmapbutton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	CStatic aboutBg;
	virtual BOOL OnInitDialog()
	{
		CDialogEx::OnInitDialog();
		HBITMAP hBit=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_SMALLDLG));

		HBITMAP hBit2=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ABOUT_ICON));
		aboutIcon.SetBitmap(hBit2);
		aboutIcon.SetWindowPos(NULL, 50, 20, 65, 65,SWP_SHOWWINDOW);

		aboutBg.SetBitmap(hBit);
		CRect rt;
		GetClientRect(&rt);
		aboutBg.SetWindowPos(NULL,0,0,rt.Width(), rt.Height(),SWP_SHOWWINDOW);

		okok.LoadBitmaps(IDB_BACK, IDB_BACK_C, IDB_BACK, IDB_BACK);
		okok.SizeToContent();
		okok.SetHoverBitmapID(IDB_BACK_C);

		return TRUE;
	}

	JHBitmapButton okok;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic aboutIcon;
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ABOUTBG, aboutBg);
	DDX_Control(pDX, IDOK, okok);
	DDX_Control(pDX, IDC_ABOUTICON, aboutIcon);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMainDlg 대화 상자




CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BG, m_imgBg);
	DDX_Control(pDX, IDC_BUTTON1, m_searchButton);
	DDX_Control(pDX, IDC_BUTTON3, m_encryptBtn);
	DDX_Control(pDX, IDC_PRECEDE_FILE, m_autoLockBtn);
	DDX_Control(pDX, IDOK, mainCloseBtn);
	DDX_Control(pDX, IDC_CHANGEPW, changePwBtn);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(MYWM_NOTIFYICON, onTrayNotify)
	ON_WM_CLOSE()
	ON_COMMAND(ID_TRAYICON_OPEN, &CMainDlg::OnTrayiconOpen)
	ON_COMMAND(ID_TRAYICON_CLOSE, &CMainDlg::OnTrayiconClose)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_PRECEDE_FILE, &CMainDlg::OnBnClickedPrecedeFile)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CMainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHANGEPW, &CMainDlg::OnBnClickedModpw)
	ON_COMMAND(ID_TRAYICON_INFO, &CMainDlg::OnTrayiconInfo)
END_MESSAGE_MAP()


// CMainDlg 메시지 처리기

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	nowRunMainDlg = 0;
	
	////////////////투명

	
	////////////
	
	
	/////////////////////////////////////////////////////이미지 씌우기///////////////////////////////////////


	HBITMAP hBit=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_MAIN_BG));
	
	HRGN h = BitmapToRegion(hBit,RGB(255,255,255),0x101010);
	if (h)
		SetWindowRgn(h, TRUE);

	m_imgBg.SetBitmap(hBit);
	CRect rt;
	GetClientRect(&rt);
	m_imgBg.SetWindowPos(NULL,0,0,rt.Width(), rt.Height(),SWP_SHOWWINDOW);


	m_searchButton.LoadBitmaps(IDB_SEARCHBTN_N, IDB_SEARCHBTN_C, IDB_SEARCHBTN_F, IDB_SEARCHBTN_U);
	m_searchButton.SizeToContent();
	m_searchButton.SetHoverBitmapID(IDB_SEARCHBTN_C);
	m_encryptBtn.LoadBitmaps(IDB_ENCRYPTBTN_N, IDB_ENCRYPTBTN_U, IDB_ENCRYPTBTN_F, IDB_ENCRYPTBTN_U);
	m_encryptBtn.SizeToContent();
	m_encryptBtn.SetHoverBitmapID(IDB_ENCRYPTBTN_C);
	m_autoLockBtn.LoadBitmaps(IDB_AUTOLOCKBTN_N, IDB_AUTOLOCKBTN_C, IDB_AUTOLOCKBTN_F, IDB_AUTOLOCKBTN_U);
	m_autoLockBtn.SizeToContent();
	m_autoLockBtn.SetHoverBitmapID(IDB_AUTOLOCKBTN_C);
	mainCloseBtn.LoadBitmaps(IDB_BOOKN, IDB_BOOKC, IDB_BOOKN, IDB_BOOKN);
	mainCloseBtn.SizeToContent();
	mainCloseBtn.SetHoverBitmapID(IDB_BOOKC);
	changePwBtn.LoadBitmaps(IDB_CHANGEPW_N, IDB_CHANGEPW_C, IDB_CHANGEPW_N, IDB_CHANGEPW_N);
	changePwBtn.SizeToContent();
	changePwBtn.SetHoverBitmapID(IDB_CHANGEPW_C);
	
	

	///////////////////////////////////////////////////////추가초기화/////////////////////////////////////////////////////
	fileTreat dlg;
	dlg.initlist(existFileList);
	dlg.passwardUnPassed(existFileList, h_file);

	cs_util_register_start_Prog("Main","C:\\Users\\한창희\\Desktop\\sherlock mainboard\\Release\\Main.exe");

	windowState = FALSE;

	TrayIcon();

	DWORD dwMessage = NIM_ADD;
	Shell_NotifyIcon(dwMessage, &tnd);

	m_bShowStatus = FALSE;

	/*DWORD process_id;
	DWORD thread_id = GetWindowThreadProcessId(NULL, &process_id);

	RegisterServiceProgress2(process_id,1);*/

	HideApp(true);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMainDlg::OnPaint()
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
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




//////////////////////////////////////////////////////////////트레이 생성/////////////////////////////////////////
void CMainDlg::TrayIcon(void)
{
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = this->GetSafeHwnd();
	tnd.uID=IDR_MAINFRAME;

	tnd.uFlags = NIF_MESSAGE | NIF_ICON;
	tnd.uCallbackMessage = MYWM_NOTIFYICON;
	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	CString title = (_T("sherLock"));
	lstrcpy(tnd.szTip, title);
}



////////////////////////////////////////////////////트레이 아이콘 삭제//////////////////////////////////////////////////
void CMainDlg::DeleteTrayIcon(void)
{
	NOTIFYICONDATA trayicon;
	trayicon.cbSize = sizeof(trayicon);
	trayicon.uID = IDR_MAINFRAME;
	trayicon.hWnd = m_hWnd;
	Shell_NotifyIcon(NIM_DELETE, &trayicon);
	PostQuitMessage(WM_QUIT);

	HWND  hWnd;
	CRect re;
	DWORD dwWidth, dwHeight, x, y;

	// find a handle of a tray
	hWnd = ::FindWindow( _T("Shell_TrayWnd"), NULL );

	if( hWnd != NULL )
		hWnd = ::FindWindowEx( hWnd, 0, _T("TrayNotifyWnd"), NULL );

	if( hWnd != NULL )
		hWnd = ::FindWindowEx( hWnd, 0, _T("SysPager"), NULL );

	if( hWnd != NULL )
		hWnd = ::FindWindowEx( hWnd, 0, _T("ToolbarWindow32"), NULL );

	// get client rectangle (needed for width and height of tray)

	if( hWnd!=NULL )
	{
		::GetClientRect( hWnd, &re );

		// get size of small icons
		dwWidth = (DWORD)GetSystemMetrics(SM_CXSMICON);   //  sm_cxsmicon);
		dwHeight = (DWORD)GetSystemMetrics(SM_CYSMICON);   //  sm_cysmicon);

		// initial y position of mouse - half of height of icon
		y = dwHeight >> 1;
		while( y < (DWORD)re.bottom )  // while y < height of tray
		{
			x = dwHeight>>1;              // initial x position of mouse - half of width of icon
			while( x < (DWORD)
				re.right ) // while x < width of tray
			{
				::SendMessage( hWnd, WM_MOUSEMOVE, 0, (y<<16)|x); // simulate moving mouse over an icon
				x = x + dwWidth; // add width of icon to x position
			}
			y = y + dwHeight; // add height of icon to y position
		}
	}
}

void CMainDlg::OnDestroy()
{
	DeleteTrayIcon();
	//NOTIFYICONDATA nid;
	//nid.cbSize = sizeof(nid);
	//nid.hWnd = m_hWnd;
	//nid.uID = IDR_MAINFRAME; // 삭제할 트레이 ID
	//Shell_NotifyIcon(NIM_DELETE, &nid);
	CDialogEx::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}




////////////////////////////////////////////////////////////트레이 팝업///////////////////////////////////////////////////
LRESULT CMainDlg::onTrayNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg = (UINT)lParam;
	switch (uMsg)
	{
	case WM_LBUTTONDBLCLK:
		{
			OnTrayiconOpen();
			break;
		}

	case WM_RBUTTONUP:
		{
			CPoint pt;
			GetCursorPos(&pt);

			CMenu m_TrayMenu;
			m_TrayMenu.LoadMenu(IDR_MENU1);
			CMenu* pCONTEXTMenu = m_TrayMenu.GetSubMenu(0);
			pCONTEXTMenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN, pt.x, pt.y, this);
		}
	case WM_RBUTTONDOWN:
		{
			m_bShowStatus = false;
			SetForegroundWindow();
		}

	}
	return TRUE;
}




///////////////////////////////////////////////////////트레이 팝업 이벤트///////////////////////////////////////////////////////

///////////////////////////////////////////////////////패스워드 창 생성///////////////////////////////////////////////////////////
void CMainDlg::OnTrayiconOpen()
{
	CStdioFile passwordFile;
	CString savedPassword;
	fileTreat existfile;
	m_bShowStatus = false;
	CCreatePassword dlg2;

	if (!(passwordFile.Open("D:\\listmemory\\password.dat", CFile::modeReadWrite | CFile::typeText)))
	{
		dlg2.DoModal();
	}

	else
	{
	passwordFile.ReadString(savedPassword);
	passwordFile.Close();
	}

	CPasswordMain dlg;
	if(windowState == FALSE)
		dlg.DoModal();

	else
	{
		SetForegroundWindow();
	}
	

	if(dlg.isPw==TRUE)
	{	
		existfile.passwardPass(existFileList, h_file);
		m_bShowStatus = true;
		ShowWindow(SW_RESTORE);
		windowState = TRUE;
		dlg.isPw = FALSE;
	}
/*	else
	{	
		OnClose();
	}*/
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CMainDlg::OnTrayiconClose()
{

	CPasswordMain dlg;
	dlg.DoModal();
	if(dlg.isPw==TRUE)
	{
		OnDestroy();
		PostQuitMessage(WM_QUIT);
	}
	else
	{
		OnClose();
	}
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}





///////////////////////////////////////////////////////종료버튼 막음//////////////////////////////////////////////////////////
void CMainDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	fileTreat dlg;
	windowState = FALSE;

	ShowWindow(SW_HIDE);
	dlg.passwardUnPassed(existFileList, h_file);
}





///////////////////////////////////////////esc, enter, alt+f4막기//////////////////////////////////////
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		return TRUE;


	return CDialogEx::PreTranslateMessage(pMsg);
}






/////////////////////////////////////////////////시작시 감추기////////////////////////////////////////////
void CMainDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);

	if( m_bShowStatus == true )
	{
		lpwndpos->flags |= SWP_SHOWWINDOW;
	}
	else
	{
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	}
	//lpwndpos->flags&=~SWP_SHOWWINDOW;
}





void CMainDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{

	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



///////////////////////////////////////////자동실행//////////////////////////////////////
bool CMainDlg::cs_util_register_start_Prog  (const char*  sValue, const char*   sPath)
{
	HKEY hKey;
	DWORD dwDisp;
	DWORD dwSize;

	if( sValue == NULL || sPath == NULL)
	{
		return false;
	}

	assert(sValue != NULL);
	assert(sPath != NULL);

	if( RegCreateKeyEx (HKEY_LOCAL_MACHINE, 
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
		NULL, &hKey, &dwDisp) != ERROR_SUCCESS)
	{
		if(hKey != NULL)
		{
			RegCloseKey(hKey);
			return false;
		}
	}

	dwSize = strlen(sPath);

	if( RegSetValueEx (hKey, sValue, 0, REG_SZ ,(LPBYTE)sPath, dwSize ) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return false;
	}

	RegCloseKey(hKey);
	RegFlushKey(hKey);

	return true;
}



//////////////////////////////////////감추기///////////////////////////////////////////


void CMainDlg::HideApp(bool Hide)
{
	// define the RegisterServiceProcess function pointer
	typedef DWORD ( __stdcall *MYPROC )( DWORD, DWORD );

	HINSTANCE hinstLib;
	MYPROC RegisterServiceProcess;

	// load kernel32
	hinstLib = LoadLibrary( "KERNEL32.DLL" );

	if ( hinstLib )
	{
		// Load the function and execute it
		RegisterServiceProcess = (MYPROC) GetProcAddress( hinstLib, "RegisterServiceProcess" );

		if ( RegisterServiceProcess )
			RegisterServiceProcess( GetCurrentProcessId(), (DWORD) Hide );

		FreeLibrary(hinstLib);

	}
}

void CMainDlg::OnBnClickedPrecedeFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CnewpasswardDlg dlg;

	for(int i=0; i<512; i++){
		dlg.existFileList[i].conFileName=existFileList[i].conFileName;
		dlg.existFileList[i].conFilePath=existFileList[i].conFilePath;
	}

	dlg.DoModal();

	for(int i=0; i<512; i++){
		existFileList[i].conFileName=dlg.existFileList[i].conFileName;
		existFileList[i].conFilePath=dlg.existFileList[i].conFilePath;
	}
}


void CMainDlg::OnBnClickedButton1()
{
	CFileCompareDlg dlg;
	dlg.DoModal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMainDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CPasswordDlg dlg;
	dlg.DoModal();
}


void CMainDlg::OnBnClickedUnlock()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	fileTreat dlg;
	CloseHandle(h_file[1]);
	CloseHandle(h_file[2]);
}

void CMainDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnClose();
}



HRGN CMainDlg::BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor , COLORREF cTolerance )
{
	cTolerance = 0x101010;
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


LRESULT CMainDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int nHit = CDialog::OnNcHitTest(point);

	if (nHit == HTCLIENT)
		nHit = HTCAPTION;

	return nHit;
}

void CMainDlg::OnBnClickedModpw()
{
	CModifyPassword dlg;
	dlg.DoModal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMainDlg::OnTrayiconInfo()
{
	CAboutDlg dlg;
	dlg.DoModal();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	CRect rect;
	UINT nID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC)
	{    
		if(nID == IDC_STATIC)
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

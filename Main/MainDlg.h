
// MainDlg.h : 헤더 파일
//
#pragma once
#define MYWM_NOTIFYICON (WM_USER+1)
#include "fileTreat.h"
#include "afxwin.h"
#include "afxext.h"
#include "JHBitmapButton.h"
#include "JHPicture.h"


// CMainDlg 대화 상자
class CMainDlg : public CDialogEx
{
// 생성입니다.
public:
	CMainDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	bool m_bShowStatus;
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	
	NOTIFYICONDATA tnd;

	afx_msg LRESULT onTrayNotify(WPARAM, LPARAM);
public:
	afx_msg void OnClose();
	afx_msg void OnTrayiconOpen();
	afx_msg void OnTrayiconClose();
	void TrayIcon(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	bool cs_util_register_start_Prog  (const char*  sValue, const char*   sPath);
	void DeleteTrayIcon(void);
	afx_msg void OnDestroy();
	BOOL windowState;
	void HideApp(bool Hide);
	afx_msg void OnBnClickedPrecedeFile();
	fileTreat existFileList[512];
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedUnlock();
	HANDLE h_file[256];
	CStatic m_imgBg;
	JHBitmapButton m_searchButton;
	JHBitmapButton m_encryptBtn;
	JHBitmapButton m_autoLockBtn;
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT CMainDlg::OnNcHitTest(CPoint point);
	HRGN CMainDlg::BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor , COLORREF cTolerance);

	JHBitmapButton mainCloseBtn;
	afx_msg void OnBnClickedModpw();
	JHBitmapButton changePwBtn;
	afx_msg void OnTrayiconInfo();
};

//typedef dword (winapi *lpregsrvproc)(dword, dword);
//void registerserviceprogress(dword dwprocessld, dword dwtype)
//{
//	hmodule hmod = getmodulehandle("kernel32.dll");
//	if(hmod)
//	{
//		lpregsrvproc pfn
//			=(lpregsrvproc)getprocaddress;
//		(hmod, "registerserviceprocess");
//		if(null != pfn)
//			pfn(dwprocessld, dwtype);
//	}
//}
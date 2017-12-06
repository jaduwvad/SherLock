
// passwordDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "SAVE.h"
#include "DIALOG1.h"
#include "JHCListCtrl.h"
#include "afxwin.h"
#include "JHBitmapButton.h"
#include "JHPicture.h"
#include "afxext.h"


// CpasswordDlg 대화 상자
class CPasswordDlg : public CDialogEx
{
// 생성입니다.
public:
	CPasswordDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PASSWORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	JHCListCtrl VIEWLIST;
	CString VIEW;
	afx_msg void OnLvnItemchangedViewlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	int buttonCount;
	

	SAVE data[100];
	afx_msg void OnBnClickedViewopt();
	void ViewControl(int buttonNum);
	CProgressCtrl cProgress;
	CString m_strKeyValue;
	UINT m_nRound;
	CString m_strIV;
	CString m_strFile;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMDblclkResultlist(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic encryptBG;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	JHBitmapButton changeBtn;
	JHBitmapButton back;
	HRGN CPasswordDlg::BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor , COLORREF cTolerance );
	afx_msg LRESULT OnNcHitTest(CPoint point);
	JHBitmapButton e_LoadBtn;
	JHBitmapButton e_EnCBtn;
	JHBitmapButton e_DelBtn;
};


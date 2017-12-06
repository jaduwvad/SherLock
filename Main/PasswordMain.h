#pragma once
#include "afxwin.h"
#include "jhbitmapbutton.h"


// CPassword 대화 상자입니다.

class CPasswordMain : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordMain)

public:
	CPasswordMain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPasswordMain();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PASSWORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CString userInputPassword;
	BOOL isPw;
	afx_msg void OnBnClickedPasswordOk();
	afx_msg void OnBnClickedModButton();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	CStatic pwBg;
	JHBitmapButton pwOkBtn;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	JHBitmapButton pwCancel;
	afx_msg void OnBnClickedPwcancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL ButtonEnter(MSG* pMsg);
	
};

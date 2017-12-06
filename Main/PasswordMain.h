#pragma once
#include "afxwin.h"
#include "jhbitmapbutton.h"


// CPassword ��ȭ �����Դϴ�.

class CPasswordMain : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordMain)

public:
	CPasswordMain(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPasswordMain();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PASSWORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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

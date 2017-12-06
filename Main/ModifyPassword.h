#pragma once
#include "afxwin.h"
#include "afxext.h"
#include "JHBitmapButton.h"


// CModifyPassword 대화 상자입니다.

class CModifyPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyPassword)

public:
	CModifyPassword(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CModifyPassword();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOD_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	
	CString password_New;
	CString password_Confirm;
	afx_msg void OnBnClickedPasswordOk();
	CString password_Old;
	CStatic modPwBg;
	JHBitmapButton pwOkBtn;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancel();
	JHBitmapButton cancelBtn;
};

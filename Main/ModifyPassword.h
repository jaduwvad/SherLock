#pragma once
#include "afxwin.h"
#include "afxext.h"
#include "JHBitmapButton.h"


// CModifyPassword ��ȭ �����Դϴ�.

class CModifyPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyPassword)

public:
	CModifyPassword(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CModifyPassword();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MOD_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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

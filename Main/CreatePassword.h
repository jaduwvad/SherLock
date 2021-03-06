#pragma once


// CCreatePassword 대화 상자입니다.

class CCreatePassword : public CDialogEx
{
	DECLARE_DYNAMIC(CCreatePassword)

public:
	CCreatePassword(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCreatePassword();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CREATE_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString password_New;

	CString password_Confirm;
	afx_msg void OnBnClickedPasswordOk();
};

#pragma once


// CCreatePassword ��ȭ �����Դϴ�.

class CCreatePassword : public CDialogEx
{
	DECLARE_DYNAMIC(CCreatePassword)

public:
	CCreatePassword(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCreatePassword();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CREATE_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString password_New;

	CString password_Confirm;
	afx_msg void OnBnClickedPasswordOk();
};

#pragma once

#include "passwordDlg.h"
#include "SAVE.h"

// DIALOG1 ��ȭ �����Դϴ�.

class DIALOG1 : public CDialog
{
	DECLARE_DYNAMIC(DIALOG1)

public:
	DIALOG1(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~DIALOG1();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedViewall();
	afx_msg void OnBnClickedView1();
	afx_msg void OnBnClickedView2();
	
	int buttonNum;

};

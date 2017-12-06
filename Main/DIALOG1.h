#pragma once

#include "passwordDlg.h"
#include "SAVE.h"

// DIALOG1 대화 상자입니다.

class DIALOG1 : public CDialog
{
	DECLARE_DYNAMIC(DIALOG1)

public:
	DIALOG1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DIALOG1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedViewall();
	afx_msg void OnBnClickedView1();
	afx_msg void OnBnClickedView2();
	
	int buttonNum;

};

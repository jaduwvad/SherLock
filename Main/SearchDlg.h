#pragma once
#include "afxcmn.h"
#include "Sort.h"
#include "afxwin.h"
#include "jhbitmapbutton.h"


// SearchDlg ��ȭ �����Դϴ�.

class SearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SearchDlg)

public:
	SearchDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~SearchDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	void searchFileBasic(CString);
	int searchType;
	CString searchScope;
	CString targetFileName;
	double targetFileSize;
	CString targetFilePath;
	CSortList resultList;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int fileCnt;
	BOOL progress;
	int numOfFile;
	CProgressCtrl progressCtrl;
	int timerCnt;
	BOOL isGui;
	double detail;
	int numOfFileTemp;
	BOOL fault;
	int progressPos;

	afx_msg void OnLvnColumnclickResultlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkResultlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedSearchcancel();
	CStatic searchBg;
	JHBitmapButton stopBtn;
	JHBitmapButton closeBtn;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
typedef void (SearchDlg::*pSearchBasic)(CString);

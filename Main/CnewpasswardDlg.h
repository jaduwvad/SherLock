#pragma once
#include "afxcmn.h"
#include "fileTreat.h"
#include "afxwin.h"
#include "jhbitmapbutton.h"
#include "JHCListCtrl.h"


// CnewpasswardDlg 대화 상자입니다.

class CnewpasswardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CnewpasswardDlg)

public:
	CnewpasswardDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CnewpasswardDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PRECEDE_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	virtual BOOL OnInitDialog();
	JHCListCtrl f_list;
	CString fileName;
	CString filePath;
	fileTreat existFileList[512];
	afx_msg void OnBnClickedFileChoice();
	afx_msg void OnBnClickedFileDelete();
	afx_msg void OnBnClickedListReset();
	afx_msg void OnBnClickedAddLock();
	CStatic preBg;
	JHBitmapButton loadFileBtn;
	JHBitmapButton delFileBtn;
	JHBitmapButton closeBtn;
	JHBitmapButton clearBtn;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMDblclkResultlist(NMHDR *pNMHDR, LRESULT *pResult);
};

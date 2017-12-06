#pragma once
#include "afxcmn.h"
#include "fileTreat.h"
#include "afxwin.h"
#include "jhbitmapbutton.h"
#include "JHCListCtrl.h"


// CnewpasswardDlg ��ȭ �����Դϴ�.

class CnewpasswardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CnewpasswardDlg)

public:
	CnewpasswardDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CnewpasswardDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PRECEDE_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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

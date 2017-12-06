
// passwordDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "SAVE.h"
#include "DIALOG1.h"
#include "JHCListCtrl.h"
#include "afxwin.h"
#include "JHBitmapButton.h"
#include "JHPicture.h"
#include "afxext.h"


// CpasswordDlg ��ȭ ����
class CPasswordDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CPasswordDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PASSWORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	JHCListCtrl VIEWLIST;
	CString VIEW;
	afx_msg void OnLvnItemchangedViewlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	int buttonCount;
	

	SAVE data[100];
	afx_msg void OnBnClickedViewopt();
	void ViewControl(int buttonNum);
	CProgressCtrl cProgress;
	CString m_strKeyValue;
	UINT m_nRound;
	CString m_strIV;
	CString m_strFile;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMDblclkResultlist(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic encryptBG;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	JHBitmapButton changeBtn;
	JHBitmapButton back;
	HRGN CPasswordDlg::BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor , COLORREF cTolerance );
	afx_msg LRESULT OnNcHitTest(CPoint point);
	JHBitmapButton e_LoadBtn;
	JHBitmapButton e_EnCBtn;
	JHBitmapButton e_DelBtn;
};


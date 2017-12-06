
// FileCompareDlg.h : ��� ����
//

#pragma once
#include "afxshelltreectrl.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "afxext.h"
#include "JHBitmapButton.h"
#include "BitmapSlider.h"
//#include "JHPicture.h"
//#include "JHCListCtrl.h"


// CFileCompareDlg ��ȭ ����
class CFileCompareDlg : public CDialogEx
{
	// �����Դϴ�.
public:
	CFileCompareDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_FILECOMPARE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


	// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFindfile();
	CString findPath;
	CMFCShellTreeCtrl fileTree;
	afx_msg void OnTvnSelchangedMfcshelltree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFindstart();
	afx_msg void OnBnClickedRadio(UINT msg);
	int searchType;
	CString searchScope;
	CString targetFileName;
	double targetFileSize;
	CString targetFilePath;
	int numOfFile;
	BOOL isGui;
	afx_msg void OnBnClickedGuiCheck();
	CBitmapSlider sliderDetail;
	double detail;
	afx_msg void OnNMReleasedcaptureSliderDetail(NMHDR *pNMHDR, LRESULT *pResult);
	CString infoEdit;
	int fault;
	CString dir;
	CStatic fcBg;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	JHBitmapButton s_LoadBtn;
	JHBitmapButton s_Cancel;
	JHBitmapButton s_Start;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CBitmapSlider null;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
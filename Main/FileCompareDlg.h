
// FileCompareDlg.h : 헤더 파일
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


// CFileCompareDlg 대화 상자
class CFileCompareDlg : public CDialogEx
{
	// 생성입니다.
public:
	CFileCompareDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_FILECOMPARE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


	// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
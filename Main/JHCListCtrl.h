#pragma once
typedef struct  SORT_PARAMS2
{
	HWND hWnd;
	int       nCol;
	BOOL bAscend;
}SORT_PARAMS;
// JHCListCtrl

class JHCListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(JHCListCtrl)

public:
	JHCListCtrl();
	virtual ~JHCListCtrl();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	char temp[256];
	CString buffer[100];
	int uCount;
	int ifDrag;

	BOOL    m_bAscend;
	afx_msg void  OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); //������ ����
	static int CALLBACK iSortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); //������ ����

protected:
	DECLARE_MESSAGE_MAP()

};
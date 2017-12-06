#pragma once
typedef struct  SORT_PARAMS
{
	HWND hWnd;
	int       nCol;
	BOOL bAscend;
}SORT_PARAMS;

//����Ŭ����
class CSortList : public CListCtrl   //CListCtrl�� ��ӹ޾���.
{
	DECLARE_DYNAMIC(CSortList)
public:
	CSortList();
	virtual ~CSortList();
protected:
	DECLARE_MESSAGE_MAP();
public:
	BOOL    m_bAscend;
	afx_msg void  OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); //������ ����
	static int CALLBACK iSortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); //������ ����
};


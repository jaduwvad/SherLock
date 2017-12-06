#pragma once
typedef struct  SORT_PARAMS
{
	HWND hWnd;
	int       nCol;
	BOOL bAscend;
}SORT_PARAMS;

//소팅클래스
class CSortList : public CListCtrl   //CListCtrl을 상속받았음.
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
	static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); //문자형 소팅
	static int CALLBACK iSortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); //정수형 소팅
};


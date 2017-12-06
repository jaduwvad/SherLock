#pragma once
#include "stdafx.h"
#include "Sort.h"

IMPLEMENT_DYNAMIC(CSortList, CListCtrl)

	CSortList::CSortList()
	: m_bAscend(FALSE)
{
}
CSortList::~CSortList()
{
}
BEGIN_MESSAGE_MAP(CSortList, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK,&CSortList::OnLvnColumnclick)
END_MESSAGE_MAP()


void CSortList::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLY = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nColumn = pNMLY->iSubItem;
	for(int i=0; i<GetItemCount(); i++)
	{
		SetItemData(i, i);
	}
	m_bAscend = !m_bAscend;
	SORT_PARAMS sort_params;
	sort_params.hWnd = GetSafeHwnd();
	sort_params.nCol = nColumn;
	sort_params.bAscend = m_bAscend;
	if(nColumn==3)  // 컬럼 정수비교
	{
		SortItems(&iSortFunc,(LPARAM)&sort_params);
	}
	else               // 문자비교.
	{
		SortItems(&SortFunc, (LPARAM)&sort_params);
	}
	*pResult = 0;
}

/////////////////////////////////////////////////////////// 문자타입 소팅
int CALLBACK CSortList::SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SORT_PARAMS *pSortParams  = (SORT_PARAMS*)lParamSort;
	CListCtrl  *pListCtrl = (CListCtrl*)CWnd::FromHandle(pSortParams->hWnd);
	BOOL bAscend = pSortParams->bAscend;
	int nCol = pSortParams->nCol;
	CString strItem1 = pListCtrl->GetItemText(lParam1, nCol);
	CString strItem2 = pListCtrl->GetItemText(lParam2, nCol);
	strItem1.MakeLower();
	strItem2.MakeLower();
	if(bAscend)
		return strItem1.Compare(strItem2);
	else
		return strItem2.Compare(strItem1);
}

///////////////////////////////////////////////////////////// int형 소팅
int CALLBACK CSortList::iSortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SORT_PARAMS *pSortParams  = (SORT_PARAMS*)lParamSort;
	CListCtrl  *pListCtrl = (CListCtrl*)CWnd::FromHandle(pSortParams->hWnd);
	BOOL bAscend = pSortParams->bAscend;
	int nCol = pSortParams->nCol;
	int istrItem1 = _ttoi(pListCtrl->GetItemText(lParam1, nCol));
	int istrItem2 = _ttoi(pListCtrl->GetItemText(lParam2, nCol));

	int iReturn = istrItem1-istrItem2;
	if(iReturn<0) iReturn = -1;
	else if(iReturn>0) iReturn = 1;

	if(bAscend)
		return iReturn;
	else 
		return -iReturn;
}
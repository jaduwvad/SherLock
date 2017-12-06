// JHCListCtrl.cpp : ���� �����Դϴ�.
//
#pragma once
#include "stdafx.h"
#include "password.h"
#include "JHCListCtrl.h"


// JHCListCtrl

IMPLEMENT_DYNAMIC(JHCListCtrl, CListCtrl)

JHCListCtrl::JHCListCtrl()
{

}

JHCListCtrl::~JHCListCtrl()
{
}

void JHCListCtrl::OnDropFiles(HDROP hDropInfo)
{
	ifDrag = 1;
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//��ӵ� ������ ����
	uCount = DragQueryFile( hDropInfo, 0xFFFFFFFF, NULL, 0 );
	
	if(uCount > 100)
	{
		MessageBox(_T("������ �ѹ��� 100�������� �ҷ� �� �� �ֽ��ϴ�."));
		return;
	}
	for(int i = 0;i < uCount ; i++)
	{
		// ������ ��� ����
		DragQueryFile(hDropInfo, i, temp, 256 );
		buffer[i] = temp;
	}
	::DragFinish(hDropInfo);
		

	CListCtrl::OnDropFiles(hDropInfo);
}

BEGIN_MESSAGE_MAP(JHCListCtrl, CListCtrl)
		ON_WM_DROPFILES()
		ON_NOTIFY_REFLECT(LVN_COLUMNCLICK,&JHCListCtrl::OnLvnColumnclick)
END_MESSAGE_MAP()



// JHCListCtrl �޽��� ó�����Դϴ�.

void JHCListCtrl::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
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
	if(nColumn==3)  // �÷� ������
	{
		SortItems(&iSortFunc,(LPARAM)&sort_params);
	}
	else               // ���ں�.
	{
		SortItems(&SortFunc, (LPARAM)&sort_params);
	}
	*pResult = 0;
}

/////////////////////////////////////////////////////////// ����Ÿ�� ����
int CALLBACK JHCListCtrl::SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

///////////////////////////////////////////////////////////// int�� ����
int CALLBACK JHCListCtrl::iSortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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


// FileCompare.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CFileCompareApp:
// �� Ŭ������ ������ ���ؼ��� FileCompare.cpp�� �����Ͻʽÿ�.
//

class CFileCompareApp : public CWinApp
{
public:
	CFileCompareApp();

	// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


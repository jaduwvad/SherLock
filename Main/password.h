
// password.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CpasswordApp:
// �� Ŭ������ ������ ���ؼ��� password.cpp�� �����Ͻʽÿ�.
//

class CpasswordApp : public CWinApp
{
public:
	CpasswordApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

//extern CpasswordApp theApp;
// Crypt.h : main header file for the CRYPT application
//

#if !defined(AFX_CRYPT_H__3514C1C1_FB15_4661_83C3_C8B297408D25__INCLUDED_)
#define AFX_CRYPT_H__3514C1C1_FB15_4661_83C3_C8B297408D25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCryptApp:
// See Crypt.cpp for the implementation of this class
//

class CCryptApp : public CWinApp
{
public:
	CCryptApp();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCryptApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	// Implementation

	//{{AFX_MSG(CCryptApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif  !defined(AFX_CRYPT_H__3514C1C1_FB15_4661_83C3_C8B297408D25__INCLUDED_)

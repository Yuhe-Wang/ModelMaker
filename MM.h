// MM.h : main header file for the MM application
//

#if !defined(AFX_MM_H__6B22E067_E9CB_4B15_B7DE_E63C9F0BCB19__INCLUDED_)
#define AFX_MM_H__6B22E067_E9CB_4B15_B7DE_E63C9F0BCB19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMMApp:
// See MM.cpp for the implementation of this class
//

class CMMApp : public CWinApp
{
public:
	CMMApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMMApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MM_H__6B22E067_E9CB_4B15_B7DE_E63C9F0BCB19__INCLUDED_)

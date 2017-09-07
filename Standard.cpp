// Standard.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "Standard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStandard dialog


CStandard::CStandard(CWnd* pParent /*=NULL*/)
	: CDialog(CStandard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStandard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStandard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStandard)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStandard, CDialog)
	//{{AFX_MSG_MAP(CStandard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStandard message handlers

void CStandard::OnOK() 
{	
	//CDialog::OnOK();
}

void CStandard::OnCancel() 
{	
	//CDialog::OnCancel();
}


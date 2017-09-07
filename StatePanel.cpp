// StatePanel.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "MMDlg.h"
#include "StatePanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatePanel dialog
extern CMMDlg *g_p;

CStatePanel::CStatePanel(CWnd* pParent /*=NULL*/)
	: CDialog(CStatePanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatePanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStatePanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatePanel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatePanel, CDialog)
	//{{AFX_MSG_MAP(CStatePanel)
	ON_BN_CLICKED(IDC_CHANGE_VIEWSTYLE, OnChangeViewstyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatePanel message handlers

void CStatePanel::OnOK() 
{
	// TODO: Add extra validation here
	//CDialog::OnOK();
}

void CStatePanel::OnCancel() 
{
	// TODO: Add extra cleanup here	
	//CDialog::OnCancel();
}

void CStatePanel::OnChangeViewstyle() 
{
	g_p->changeViewStyle();	
}

BOOL CStatePanel::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam==87||pMsg->wParam==83||pMsg->wParam==68||pMsg->wParam==65)
	{
		g_p->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);	
		return TRUE;
	}
	else return CDialog::PreTranslateMessage(pMsg);
}

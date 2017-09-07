// Pipe.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "Pipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPipe dialog


CPipe::CPipe(CWnd* pParent /*=NULL*/)
	: CDialog(CPipe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPipe)
	m_KBH = 1.0;
	m_KBr = 0.7;
	m_KBR = 1.0;
	m_KBX = 0.0;
	m_KBY = 0.0;
	m_KBZ = 0.0;
	m_paraH = 0.0;
	m_para_r = 0.0;
	m_para_R = 0.0;
	//}}AFX_DATA_INIT
}


void CPipe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPipe)
	DDX_Control(pDX, IDC_LIST_NR, m_NR);
	DDX_Control(pDX, IDC_LIST_NH, m_NH);
	DDX_Control(pDX, IDC_LIST_N, m_N);
	DDX_Text(pDX, IDC_KB_HEIGHT, m_KBH);
	DDX_Text(pDX, IDC_KB_INNER_R, m_KBr);
	DDX_Text(pDX, IDC_KB_R, m_KBR);
	DDX_Text(pDX, IDC_KB_X, m_KBX);
	DDX_Text(pDX, IDC_KB_Y, m_KBY);
	DDX_Text(pDX, IDC_KB_Z, m_KBZ);
	DDX_Text(pDX, IDC_PARA_HEIGHT, m_paraH);
	DDX_Text(pDX, IDC_PARA_INNER_R, m_para_r);
	DDX_Text(pDX, IDC_PARA_R, m_para_R);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPipe, CDialog)
	//{{AFX_MSG_MAP(CPipe)
	ON_BN_CLICKED(IDC_KB_CREATE, OnKBCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPipe::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString s;
	for (int i=ND_MAX;i>=3;i--) //底边可以是3~ND_MAX边形
	{
		s.Format("%d",i);
		m_N.AddString(s);
	}
	m_N.SetTopIndex(ND_MAX-20);
	for (i=ND_MAX;i>=1;i--) //底边可以是3~ND_MAX边形
	{
		s.Format("%d",i);
		m_NH.AddString(s);
		m_NR.AddString(s);
	}
	m_NH.SetTopIndex(ND_MAX-1);
	m_NR.SetTopIndex(ND_MAX-1);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CPipe message handlers

void CPipe::OnOK() 
{
	//CDialog::OnOK();
}

void CPipe::OnCancel() 
{	
	//CDialog::OnCancel();
}

void CPipe::OnKBCreate() 
{
	
}

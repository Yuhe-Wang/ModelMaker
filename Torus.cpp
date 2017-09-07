// Torus.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "Torus.h"
#include "MMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMMDlg *g_p;
/////////////////////////////////////////////////////////////////////////////
// CTorus dialog


CTorus::CTorus(CWnd* pParent /*=NULL*/)
	: CDialog(CTorus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTorus)
	m_KBr = 0.3;
	m_KBR = 1.0;
	m_KBX = 0.0;
	m_KBY = 0.0;
	m_KBZ = 0.0;
	m_r = 0.3;
	m_R = 1.0;
	//}}AFX_DATA_INIT
}


void CTorus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTorus)
	DDX_Control(pDX, IDC_LIST_NW, m_nw);
	DDX_Control(pDX, IDC_LIST_NJ, m_nj);
	DDX_Text(pDX, IDC_KB_INNER_R, m_KBr);
	DDX_Text(pDX, IDC_KB_R, m_KBR);
	DDX_Text(pDX, IDC_KB_X, m_KBX);
	DDX_Text(pDX, IDC_KB_Y, m_KBY);
	DDX_Text(pDX, IDC_KB_Z, m_KBZ);
	DDX_Text(pDX, IDC_PARA_INNER_R, m_r);
	DDX_Text(pDX, IDC_PARA_R, m_R);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTorus, CDialog)
	//{{AFX_MSG_MAP(CTorus)
	ON_BN_CLICKED(IDC_KB_CREATE, OnKBCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTorus::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString s;
	for (int i=ND_MAX;i>=3;i--) //底边可以是3~ND_MAX边形
	{
		s.Format("%d",i);
		m_nj.AddString(s);
		m_nw.AddString(s);
	}
	m_nj.SetTopIndex(ND_MAX-12);
	m_nw.SetTopIndex(ND_MAX-10);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CTorus message handlers

void CTorus::OnOK() 
{
	//CDialog::OnOK();
}

void CTorus::OnCancel() 
{	
	//CDialog::OnCancel();
}

void CTorus::OnKBCreate() 
{
	UpdateData(TRUE);
	if(m_KBR<=0.0||m_KBr<=0) MessageBox("内外径必须为正值 !");
	else if(m_KBR<m_KBr) MessageBox("外径必须大于内径！");
	else
	{
		g_p->m_torus.cx=m_KBX;
		g_p->m_torus.cy=m_KBY;
		g_p->m_torus.cz=m_KBZ;	
		g_p->m_torus.R=m_KBR;
		g_p->m_torus.r=m_KBr;
		g_p->m_torus.nj=ND_MAX-m_nj.GetTopIndex();
		g_p->m_torus.nw=ND_MAX-m_nw.GetTopIndex();
		g_p->createTorus();//根据m_sphere创建球体
	}	
}

// Cone.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "Cone.h"
#include "MMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMMDlg *g_p;
/////////////////////////////////////////////////////////////////////////////
// CCone dialog


CCone::CCone(CWnd* pParent /*=NULL*/)
	: CDialog(CCone::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCone)
	m_KBHeight = 0.0;
	m_KBR = 0.0;
	m_KBX = 0.0;
	m_KBY = 0.0;
	m_KBZ = 0.0;
	m_paraHeight = 0.0;
	m_paraR = 0.0;
	//}}AFX_DATA_INIT
}


void CCone::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCone)
	DDX_Control(pDX, IDC_LIST_NR, m_NR);
	DDX_Control(pDX, IDC_LIST_NH, m_NH);
	DDX_Control(pDX, IDC_LIST_DN, m_DN);
	DDX_Text(pDX, IDC_KB_HEIGHT, m_KBHeight);
	DDX_Text(pDX, IDC_KB_R, m_KBR);
	DDX_Text(pDX, IDC_KB_X, m_KBX);
	DDX_Text(pDX, IDC_KB_Y, m_KBY);
	DDX_Text(pDX, IDC_KB_Z, m_KBZ);
	DDX_Text(pDX, IDC_PARA_HEIGHT, m_paraHeight);
	DDX_Text(pDX, IDC_PARA_R, m_paraR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCone, CDialog)
	//{{AFX_MSG_MAP(CCone)
	ON_BN_CLICKED(IDC_KB_CREATE, OnKBCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCone::OnInitDialog()
{
	CDialog::OnInitDialog();
	//额外初始化
	((CButton *)GetDlgItem(IDC_REGULAR_POLYGON))->SetCheck(1);//默认创建正棱柱
	CString s;
	for (int i=ND_MAX;i>=3;i--) //底边可以是3~ND_MAX边形
	{
		s.Format("%d",i);
		m_DN.AddString(s);
	}
	m_DN.SetTopIndex(ND_MAX-3);
	for (i=20;i>=1;i--) //分段为1~20
	{
		s.Format("%d",i);
		m_NR.AddString(s);
		m_NH.AddString(s);
	}
	m_NR.SetTopIndex(20-1);
	m_NH.SetTopIndex(20-1);
	
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CCone message handlers

void CCone::OnOK() 
{
	//CDialog::OnOK();
}

void CCone::OnCancel() 
{	
	//CDialog::OnCancel();
}

void CCone::OnKBCreate() 
{
	if(((CButton *)GetDlgItem(IDC_REGULAR_POLYGON))->GetCheck()==1)//正棱柱可由键盘创建
	{
		UpdateData(TRUE);
		if(m_KBR==0.0||m_KBHeight==0.0) MessageBox("半径和高度均不能为0");
		else
		{
			g_p->m_cylinder.cx=m_KBX;
			g_p->m_cylinder.cy=m_KBY;
			g_p->m_cylinder.cz=m_KBZ;	
			g_p->m_cylinder.R=m_KBR;
			g_p->m_cylinder.h=m_KBHeight;
			g_p->m_cylinder.nr=20-m_NR.GetTopIndex();
			g_p->m_cylinder.nh=20-m_NH.GetTopIndex();
			g_p->m_cylinder.type=0;
			g_p->m_cylinder.angle=0;
			g_p->m_cylinder.n=ND_MAX-m_DN.GetTopIndex();
			g_p->createCone();//根据m_cuboid创建长方体
		}		
	}
	else MessageBox("只能从键盘创建正棱柱！");	
}

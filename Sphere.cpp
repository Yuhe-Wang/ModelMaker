// Sphere.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "Sphere.h"
#include "MMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMMDlg *g_p;
/////////////////////////////////////////////////////////////////////////////
// CSphere dialog


CSphere::CSphere(CWnd* pParent /*=NULL*/)
	: CDialog(CSphere::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSphere)
	m_KBR = 1.0;
	m_KBX = 0.0;
	m_KBY = 0.0;
	m_KBZ = 0.0;
	m_paraR = 0.0;
	//}}AFX_DATA_INIT
}


void CSphere::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSphere)
	DDX_Control(pDX, IDC_LIST_NW, m_NW);
	DDX_Control(pDX, IDC_LIST_NJ, m_NJ);
	DDX_Text(pDX, IDC_KB_R, m_KBR);
	DDX_Text(pDX, IDC_KB_X, m_KBX);
	DDX_Text(pDX, IDC_KB_Y, m_KBY);
	DDX_Text(pDX, IDC_KB_Z, m_KBZ);
	DDX_Text(pDX, IDC_PARA_R, m_paraR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSphere, CDialog)
	//{{AFX_MSG_MAP(CSphere)
	ON_BN_CLICKED(IDC_KB_CREATE, OnKBCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSphere::OnInitDialog()
{
	CDialog::OnInitDialog();
	//额外初始化
	CString s;
	for (int i=ND_MAX;i>=10;i--) //分段可以是10~ND_MAX边形
	{
		s.Format("%d",i);
		m_NJ.AddString(s);
		m_NW.AddString(s);
	}
	m_NJ.SetTopIndex(ND_MAX-10);
	m_NW.SetTopIndex(ND_MAX-10);
	
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CSphere message handlers

void CSphere::OnOK() 
{
	//CDialog::OnOK();
}

void CSphere::OnCancel() 
{	
	//CDialog::OnCancel();
}

void CSphere::OnKBCreate() 
{
	UpdateData(TRUE);
	if(m_KBR==0.0) MessageBox("半径不能为0 !");
	else
	{
		g_p->m_sphere.cx=m_KBX;
		g_p->m_sphere.cy=m_KBY;
		g_p->m_sphere.cz=m_KBZ;	
		g_p->m_sphere.R=m_KBR;
		g_p->m_sphere.nj=ND_MAX-m_NJ.GetTopIndex();
		g_p->m_sphere.nw=ND_MAX-m_NW.GetTopIndex();
		g_p->createSphere();//根据m_sphere创建球体
	}			
}

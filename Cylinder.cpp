// Cylinder.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "Cylinder.h"
#include "MMDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMMDlg *g_p;
/////////////////////////////////////////////////////////////////////////////
// CCylinder dialog


CCylinder::CCylinder(CWnd* pParent /*=NULL*/)
	: CDialog(CCylinder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCylinder)
	m_KBX = 0.0;
	m_KBY = 0.0;
	m_KBZ = 0.0;
	m_paraHeight = 0.0;
	m_paraR = 0.0;
	m_KBR = 0.0;
	m_KBHeight = 0.0;
	//}}AFX_DATA_INIT
}


void CCylinder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCylinder)
	DDX_Control(pDX, IDC_LIST_NR, m_NR);
	DDX_Control(pDX, IDC_LIST_NH, m_NH);
	DDX_Control(pDX, IDC_LIST_DN, m_DN);
	DDX_Text(pDX, IDC_KB_X, m_KBX);
	DDX_Text(pDX, IDC_KB_Y, m_KBY);
	DDX_Text(pDX, IDC_KB_Z, m_KBZ);
	DDX_Text(pDX, IDC_PARA_HEIGHT, m_paraHeight);
	DDX_Text(pDX, IDC_PARA_R, m_paraR);
	DDX_Text(pDX, IDC_KB_R, m_KBR);
	DDX_Text(pDX, IDC_KB_HEIGHT, m_KBHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCylinder, CDialog)
	//{{AFX_MSG_MAP(CCylinder)
	ON_BN_CLICKED(IDC_KB_CREATE, OnKBCreate)
	ON_BN_CLICKED(IDC_REGULAR_POLYGON, OnRegularPolygon)
	ON_BN_CLICKED(IDC_NORMAL_POLYGON, OnNormalPolygon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCylinder::OnInitDialog()
{
	CDialog::OnInitDialog();
	//�����ʼ��
	((CButton *)GetDlgItem(IDC_REGULAR_POLYGON))->SetCheck(1);//Ĭ�ϴ���������
	CString s;
	for (int i=ND_MAX;i>=3;i--) //�ױ߿�����3~ND_MAX����
	{
		s.Format("%d",i);
		m_DN.AddString(s);
	}
	m_DN.SetTopIndex(ND_MAX-3);
	for (i=20;i>=1;i--) //�ֶ�Ϊ1~20
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
// CCylinder message handlers

void CCylinder::OnKBCreate() //�Ӽ��̴���
{
	if(((CButton *)GetDlgItem(IDC_REGULAR_POLYGON))->GetCheck()==1)//���������ɼ��̴���
	{
		UpdateData(TRUE);
		if(m_KBR==0.0||m_KBHeight==0.0) MessageBox("�뾶�͸߶Ⱦ�����Ϊ0");
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
			g_p->createCylinder();//����m_cuboid����������
		}		
	}
	else MessageBox("ֻ�ܴӼ��̴�����������");
}

void CCylinder::OnRegularPolygon() //�ױ����������
{
	
}

void CCylinder::OnNormalPolygon() //�ױ�����������
{
	
}

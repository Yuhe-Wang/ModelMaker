// Cuboid.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "Cuboid.h"
#include "MMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCuboid dialog
extern CMMDlg *g_p;

CCuboid::CCuboid(CWnd* pParent /*=NULL*/)
	: CDialog(CCuboid::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCuboid)
	m_KBHeight = 0.0;
	m_KBLength = 0.0;
	m_KBWidth = 0.0;
	m_KBX = 0.0;
	m_KBY = 0.0;
	m_KBZ = 0.0;
	m_height = 0.0;
	m_length = 0.0;
	m_width = 0.0;
	//}}AFX_DATA_INIT
}


void CCuboid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCuboid)
	DDX_Control(pDX, IDC_LIST_NZ, m_nz);
	DDX_Control(pDX, IDC_LIST_NY, m_ny);
	DDX_Control(pDX, IDC_LIST_NX, m_nx);
	DDX_Text(pDX, IDC_KB_HEIGHT, m_KBHeight);
	DDX_Text(pDX, IDC_KB_LENGTH, m_KBLength);
	DDX_Text(pDX, IDC_KB_WIDTH, m_KBWidth);
	DDX_Text(pDX, IDC_KB_X, m_KBX);
	DDX_Text(pDX, IDC_KB_Y, m_KBY);
	DDX_Text(pDX, IDC_KB_Z, m_KBZ);
	DDX_Text(pDX, IDC_PARA_HEIGHT, m_height);
	DDX_Text(pDX, IDC_PARA_LENGTH, m_length);
	DDX_Text(pDX, IDC_PARA_WIDTH, m_width);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCuboid, CDialog)
	//{{AFX_MSG_MAP(CCuboid)
	ON_BN_CLICKED(IDC_KB_CREATE, OnKbCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCuboid::OnInitDialog()
{
	CDialog::OnInitDialog();
	//�����ʼ��
	((CButton *)GetDlgItem(IDC_RADIO_CUBOID))->SetCheck(1);//Ĭ�ϴ���������
	CString s;
	for (int i=10;i>0;i--) //���߷�10��
	{
		s.Format("%d",i);
		m_nx.AddString(s);
		m_ny.AddString(s);
		m_nz.AddString(s);
	}
	m_nx.SetTopIndex(9);
	m_ny.SetTopIndex(9);
	m_nz.SetTopIndex(9);
	
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CCuboid message handlers

void CCuboid::OnOK() 
{
	//CDialog::OnOK();
}

void CCuboid::OnCancel() 
{
	//CDialog::OnCancel();
}

bool CCuboid::isEdit()//�жϵ�ǰ�Ƿ������ı�����
{
	CWnd *p=GetFocus();
	if(p==GetDlgItem(IDC_KB_X)||
	   p==GetDlgItem(IDC_KB_Y)||
	   p==GetDlgItem(IDC_KB_Z)||
	   p==GetDlgItem(IDC_KB_LENGTH)||
	   p==GetDlgItem(IDC_KB_WIDTH)||
	   p==GetDlgItem(IDC_KB_HEIGHT)||
	   p==GetDlgItem(IDC_PARA_LENGTH)||
	   p==GetDlgItem(IDC_PARA_WIDTH)||
	   p==GetDlgItem(IDC_PARA_HEIGHT))
	return TRUE;
	return FALSE;
}

BOOL CCuboid::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(g_p->isHotKey(pMsg->wParam)&&!isEdit()) //�������ڷ����ȼ���Ϣ
		{
			g_p->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			return TRUE;
		}		                
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

void CCuboid::OnKbCreate() //�ɼ���������Ϣ����������
{
	UpdateData(TRUE);//��������
	if(m_KBHeight==0.0||m_KBLength==0.0||m_KBWidth==0) MessageBox("����߾�����Ϊ0");
	else
	{
		g_p->m_cuboid.cx=m_KBX;
		g_p->m_cuboid.cy=m_KBY;
		g_p->m_cuboid.cz=m_KBZ;	
		g_p->m_cuboid.lx=m_KBLength;
		g_p->m_cuboid.ly=m_KBWidth;
		g_p->m_cuboid.lz=m_KBHeight;
		g_p->m_cuboid.nx=10-m_nx.GetTopIndex();
		g_p->m_cuboid.ny=10-m_ny.GetTopIndex();
		g_p->m_cuboid.nz=10-m_nz.GetTopIndex();
		g_p->createCuboid();//����m_cuboid����������
	}
	
}

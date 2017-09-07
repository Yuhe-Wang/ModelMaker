// PanelCreate.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "MMDlg.h"
#include "PanelCreate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMMDlg *g_p;
/////////////////////////////////////////////////////////////////////////////
// CPanelCreate dialog


CPanelCreate::CPanelCreate(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelCreate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelCreate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CPanelCreate::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_createTypes.AddString("��׼��");
	m_createTypes.AddString("�Զ���ģ��");
	m_createTypes.SetCurSel(0);

	for(byte i=0;i<6;i++) 
	{
		m_panelExist[i]=0;
		m_pp[i]=NULL;
	}
	GetDlgItem(IDC_PARA_AREA)->GetWindowRect(m_paraArea);
	ScreenToClient(m_paraArea);//��ȡ������ʾ��
	return TRUE;
}

void CPanelCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelCreate)
	DDX_Control(pDX, IDC_COMBO_TYPES, m_createTypes);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPanelCreate, CDialog)
	//{{AFX_MSG_MAP(CPanelCreate)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CUBOID, OnCuboid)
	ON_BN_CLICKED(IDC_CONE, OnCone)
	ON_BN_CLICKED(IDC_SPHERE, OnSphere)
	ON_BN_CLICKED(IDC_CYLINDER, OnCylinder)
	ON_BN_CLICKED(IDC_TORUS, OnTorus)
	ON_BN_CLICKED(IDC_PIPE, OnPipe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelCreate message handlers
void CPanelCreate::OnImport() 
{
	g_p->Openfile(2);
}

BOOL CPanelCreate::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(g_p->isHotKey(pMsg->wParam)&&!isEdit()) //�������ڷ����ȼ���Ϣ
		{
			g_p->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			return TRUE;
		}
		if(pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE ) return TRUE; //���λس���esc��		                
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

void CPanelCreate::OnColor() //�������ɫ��
{		
	g_p->changeColor();	
}

HBRUSH CPanelCreate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	// TODO: Change any attributes of the DC here
	if( pWnd->GetDlgCtrlID()==IDC_COLOR)
    {
		return CreateSolidBrush(g_p->m_activeColor);
    }
	return hbr;
}

bool CPanelCreate::isEdit() //�ж��Ƿ�����������
{
	if(GetDlgItem(IDC_NAME)==GetFocus())return TRUE;
	else return FALSE;
}

void CPanelCreate::OnCuboid() //����������
{
	g_p->changeWorkMode(0);
	g_p->m_create=1;
	g_p->m_page2.unCheck();
	//���������������
	for (byte i=1;i<6;i++)
	{
		if(m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[0]) //������廹�����ڣ�������
	{
		m_cuboidPanel.Create(IDD_CUBOID,this);
		m_pp[0]=&m_cuboidPanel;
		m_cuboidPanel.MoveWindow(&m_paraArea);		
	}
	m_cuboidPanel.ShowWindow(SW_SHOW);//��ʾ�����
	GetDlgItem(IDC_CUBOID)->SetFocus();
}

void CPanelCreate::OnCone() 
{
	g_p->changeWorkMode(0);
	g_p->m_create=4;
	g_p->m_page2.unCheck();
	//���������������
	for (byte i=0;i<6;i++)
	{
		if(i!=3&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[3]) //������廹�����ڣ�������
	{
		m_conePanel.Create(IDD_CONE,this);
		m_pp[3]=&m_conePanel;
		m_conePanel.MoveWindow(&m_paraArea);		
	}
	m_conePanel.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CONE)->SetFocus();		
}

void CPanelCreate::OnSphere() 
{
	g_p->changeWorkMode(0);
	g_p->m_create=3;
	g_p->m_page2.unCheck();
	//���������������
	for (byte i=0;i<6;i++)
	{
		if(i!=2&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[2]) //������廹�����ڣ�������
	{
		m_spherePanel.Create(IDD_SPHERE,this);
		m_pp[2]=&m_spherePanel;
		m_spherePanel.MoveWindow(&m_paraArea);		
	}
	m_spherePanel.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SPHERE)->SetFocus();		
}

void CPanelCreate::OnCylinder() 
{
	g_p->changeWorkMode(0);
	g_p->m_create=2;
	g_p->m_page2.unCheck();
	//���������������
	for (byte i=0;i<6;i++)
	{
		if(i!=1&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[1]) //������廹�����ڣ�������
	{
		m_cylinderPanel.Create(IDD_CYLINDER,this);
		m_pp[1]=&m_cylinderPanel;
		m_cylinderPanel.MoveWindow(&m_paraArea);		
	}
	m_cylinderPanel.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CYLINDER)->SetFocus();
}

void CPanelCreate::OnTorus() 
{
	g_p->changeWorkMode(0);
	g_p->m_create=5;
	g_p->m_page2.unCheck();
	//���������������
	for (byte i=0;i<6;i++)
	{
		if(i!=4&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[4]) //������廹�����ڣ�������
	{
		m_torusPanel.Create(IDD_TORUS,this);
		m_pp[4]=&m_torusPanel;
		m_torusPanel.MoveWindow(&m_paraArea);		
	}
	m_torusPanel.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_TORUS)->SetFocus();
}

void CPanelCreate::OnPipe() 
{
	g_p->changeWorkMode(0);
	g_p->m_create=6;
	g_p->m_page2.unCheck();
	//���������������
	for (byte i=0;i<6;i++)
	{
		if(i!=5&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[5]) //������廹�����ڣ�������
	{
		m_pipePanel.Create(IDD_PIPE,this);
		m_pp[5]=&m_pipePanel;
		m_pipePanel.MoveWindow(&m_paraArea);		
	}
	m_pipePanel.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_PIPE)->SetFocus();
}

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
	m_createTypes.AddString("标准体");
	m_createTypes.AddString("自定义模型");
	m_createTypes.SetCurSel(0);

	for(byte i=0;i<6;i++) 
	{
		m_panelExist[i]=0;
		m_pp[i]=NULL;
	}
	GetDlgItem(IDC_PARA_AREA)->GetWindowRect(m_paraArea);
	ScreenToClient(m_paraArea);//获取参数显示区
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
		if(g_p->isHotKey(pMsg->wParam)&&!isEdit()) //向主窗口发送热键消息
		{
			g_p->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			return TRUE;
		}
		if(pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE ) return TRUE; //屏蔽回车和esc键		                
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

void CPanelCreate::OnColor() //点击到颜色框
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

bool CPanelCreate::isEdit() //判断是否有文字输入
{
	if(GetDlgItem(IDC_NAME)==GetFocus())return TRUE;
	else return FALSE;
}

void CPanelCreate::OnCuboid() //创建长方体
{
	g_p->changeWorkMode(0);
	g_p->m_create=1;
	g_p->m_page2.unCheck();
	//隐藏其他所有面板
	for (byte i=1;i<6;i++)
	{
		if(m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[0]) //参数面板还不存在，创建它
	{
		m_cuboidPanel.Create(IDD_CUBOID,this);
		m_pp[0]=&m_cuboidPanel;
		m_cuboidPanel.MoveWindow(&m_paraArea);		
	}
	m_cuboidPanel.ShowWindow(SW_SHOW);//显示该面板
	GetDlgItem(IDC_CUBOID)->SetFocus();
}

void CPanelCreate::OnCone() 
{
	g_p->changeWorkMode(0);
	g_p->m_create=4;
	g_p->m_page2.unCheck();
	//隐藏其他所有面板
	for (byte i=0;i<6;i++)
	{
		if(i!=3&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[3]) //参数面板还不存在，创建它
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
	//隐藏其他所有面板
	for (byte i=0;i<6;i++)
	{
		if(i!=2&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[2]) //参数面板还不存在，创建它
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
	//隐藏其他所有面板
	for (byte i=0;i<6;i++)
	{
		if(i!=1&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[1]) //参数面板还不存在，创建它
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
	//隐藏其他所有面板
	for (byte i=0;i<6;i++)
	{
		if(i!=4&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[4]) //参数面板还不存在，创建它
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
	//隐藏其他所有面板
	for (byte i=0;i<6;i++)
	{
		if(i!=5&&m_pp[i]) m_pp[i]->ShowWindow(SW_HIDE);
	}
	if (!m_pp[5]) //参数面板还不存在，创建它
	{
		m_pipePanel.Create(IDD_PIPE,this);
		m_pp[5]=&m_pipePanel;
		m_pipePanel.MoveWindow(&m_paraArea);		
	}
	m_pipePanel.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_PIPE)->SetFocus();
}

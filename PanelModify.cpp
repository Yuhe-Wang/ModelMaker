// PanelModify.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "PanelModify.h"
#include "MMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelModify dialog
extern CMMDlg *g_p;

CPanelModify::CPanelModify(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelModify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelModify)
	m_nMove = 0.0;
	m_chooseDensify = FALSE;
	//}}AFX_DATA_INIT
}

void CPanelModify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelModify)
	DDX_Control(pDX, IDC_SLIDER_MOVE, m_moveDistance);
	DDX_Control(pDX, IDC_KNIFE_RATIO, m_knifeRatio);
	DDX_Control(pDX, IDC_CURSOR_WIDTH, m_sliderWidth);
	DDX_Control(pDX, IDC_CURSOR_HEIGHT, m_sliderHeight);
	DDX_Control(pDX, IDC_CURSOR_BOX, m_cursorBox);
	DDX_Control(pDX, IDC_POINT_SIZE, m_pointSize);
	DDX_Control(pDX, IDC_START_DRAG, m_startDrag);
	DDX_Control(pDX, IDC_START_DISSECT, m_startDissect);
	DDX_Control(pDX, IDC_START_DENSIFY, m_startDensify);
	DDX_Text(pDX, IDC_PARALLEL_MOVE, m_nMove);
	DDX_Check(pDX, IDC_DENSIFY_CHOOSED, m_chooseDensify);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelModify, CDialog)
	//{{AFX_MSG_MAP(CPanelModify)
	ON_EN_CHANGE(IDC_EDIT_DESITY, OnChangeEditDesity)
	ON_BN_CLICKED(IDC_POINT_CHOOSE, OnPointChoose)
	ON_BN_CLICKED(IDC_BOX_CHOOSE, OnBoxChoose)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_POINT_SIZE, OnReleasedcapturePointSize)
	ON_BN_CLICKED(IDC_CURSOR_BOX, OnCursorBox)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_CURSOR_WIDTH, OnReleasedcaptureCursorWidth)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_CURSOR_HEIGHT, OnReleasedcaptureCursorHeight)
	ON_BN_CLICKED(IDC_START_DISSECT, OnStartDissect)
	ON_BN_CLICKED(IDC_MOVE, OnMove)
	ON_BN_CLICKED(IDC_ROTATE, OnRotate)
	ON_BN_CLICKED(IDC_KNIFE_SIZE, OnKnifeSize)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_KNIFE_RATIO, OnReleasedcaptureKnifeRatio)
	ON_BN_CLICKED(IDC_START_DENSIFY, OnStartDensify)
	ON_BN_CLICKED(IDC_3POINTS_KNIFE, On3PointsKnife)
	ON_BN_CLICKED(IDC_CUT, OnCut)
	ON_BN_CLICKED(IDC_DENSIFY, OnDensify)
	ON_BN_CLICKED(IDC_START_DRAG, OnStartDrag)
	ON_BN_CLICKED(IDC_DENSIFY_CHOOSED, OnDensifyChoosed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPanelModify::OnInitDialog()
{
	CDialog::OnInitDialog();
	((CButton *)GetDlgItem(IDC_POINT_CHOOSE))->SetCheck(1);//默认点选
	m_cursorBox.SetCheck(TRUE);
	m_pointSize.SetRange(1,20);
	m_pointSize.SetPos(5);
	m_sliderWidth.SetRange(4,64);
	m_sliderWidth.SetPos(10);
	m_cursorW=10;
	m_sliderHeight.SetRange(4,64);
	m_sliderHeight.SetPos(10);
	m_cursorH=10;
	m_knifeRatio.SetRange(0,60);
	m_knifeRatio.SetPos(30);
	m_moveDistance.SetRange(0,100);
	m_moveDistance.SetPos(50);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CPanelModify message handlers

void CPanelModify::OnChangeEditDesity() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
}

BOOL CPanelModify::PreTranslateMessage(MSG* pMsg) 
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

bool CPanelModify::isEdit()//判断是否有控件处于编辑状态
{
	return FALSE;
}

void CPanelModify::OnStartDrag() //开始拖动
{
	int num=g_p->chooseOneObject();//一次只编辑一个对象
	if (num!=0)
	{
		if(num>1) MessageBox("一次只能编辑一个物体，自动取其中一个\n若要更改，请点右键显示全部");
		g_p->changeWorkMode(5);
		if(((CButton *)GetDlgItem(IDC_POINT_CHOOSE))->GetCheck()==1) //点选
		{
			g_p->m_drag=1;
			g_p->m_cursorBox=1;
		}
		else g_p->m_drag=2; //框选
		g_p->hideUnselected();
		g_p->renderScene();
	}
	else 
	{
		MessageBox("必须先选中一个物体！");	
		m_startDrag.SetCheck(FALSE);
	}
}

void CPanelModify::OnPointChoose() 
{
	if(m_startDrag.GetCheck()) 
	{
		g_p->changeWorkMode(5);
		g_p->m_drag=1;	
	}
}

void CPanelModify::OnBoxChoose() 
{	
	if(m_startDrag.GetCheck()) 
	{
		g_p->changeWorkMode(5);
		g_p->m_drag=2;	
	}
}

void CPanelModify::unCheck()//使不选中任何功能
{
	if (m_startDrag.GetCheck())
	{
		m_startDrag.SetCheck(0);
		return;
	}
	if (m_startDissect.GetCheck())
	{
		m_startDissect.SetCheck(0);
		return;
	}
	if (m_startDensify.GetCheck())
	{
		m_startDensify.SetCheck(0);
		return;
	}
}

void CPanelModify::OnReleasedcapturePointSize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	glPointSize(m_pointSize.GetPos());
	g_p->renderScene();
	*pResult = 0;
}

void CPanelModify::OnCursorBox() 
{
	if (m_cursorBox.GetCheck()==1) g_p->m_cursorBox=1;
	else g_p->m_cursorBox=0;
	g_p->renderScene();
}

void CPanelModify::OnReleasedcaptureCursorWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_cursorW=m_sliderWidth.GetPos();
	g_p->renderScene();
	*pResult = 0;
}

void CPanelModify::OnReleasedcaptureCursorHeight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_cursorH=m_sliderHeight.GetPos();
	g_p->renderScene();
	*pResult = 0;
}

void CPanelModify::OnStartDissect() 
{
	int num=g_p->chooseOneObject();//一次只编辑一个对象
	if (num!=0)
	{
		if(num>1) MessageBox("一次只能编辑一个物体，自动取其中一个\n若要更改，请点右键显示全部");
		g_p->changeWorkMode(6);
		g_p->hideUnselected();
		g_p->renderScene();
	}
	else 
	{
		MessageBox("必须先选中一个物体！");	
		m_startDissect.SetCheck(FALSE);
	}	
}

void CPanelModify::OnMove() //进入移动刀面
{
	g_p->m_knife.state=1;	
}

void CPanelModify::OnRotate()//进入旋转刀面
{
	g_p->m_knife.state=2;	
}

void CPanelModify::OnKnifeSize()//切换缩放 
{
	g_p->m_knife.mouseWheelSize=!g_p->m_knife.mouseWheelSize;
	if(g_p->m_knife.mouseWheelSize) GetDlgItem(IDC_KNIFE_SIZE)->SetWindowText("+ -");
	else GetDlgItem(IDC_KNIFE_SIZE)->SetWindowText("缩放");
}

void CPanelModify::OnReleasedcaptureKnifeRatio(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int pos=m_knifeRatio.GetPos();
	if (pos>=30)
	{
		g_p->m_knife.dx=pos/10.0-2;
		g_p->m_knife.dy=1;
	}
	else
	{
		g_p->m_knife.dx=1;
		g_p->m_knife.dy=4-pos/10.0;
	}
	g_p->renderScene();//需重绘
	*pResult = 0;
}

void CPanelModify::OnStartDensify() 
{
	int num=g_p->chooseOneObject();//一次只编辑一个对象
	if (num!=0)
	{
		if(num>1) MessageBox("一次只能编辑一个物体，自动取其中一个\n若要更改，请点右键显示全部");
		g_p->changeWorkMode(7);//加密
		g_p->hideUnselected();
		g_p->renderScene();
	}
	else 
	{
		MessageBox("必须先选中一个物体！");	
		m_startDensify.SetCheck(FALSE);
	}	
}

void CPanelModify::On3PointsKnife()//三点创建一个切割面
{
	
}

void CPanelModify::OnCut() //切割曲面
{
	if(m_startDissect.GetCheck())
	{
		unCheck();
		g_p->cutCS();
	}	
}

void CPanelModify::OnDensify() //加密网格
{
	if(m_startDensify.GetCheck())
	{
		unCheck();
		UpdateData(TRUE);
		if (m_chooseDensify)
		{
			g_p->densifySelected();
		}
		else g_p->densify(0);
		g_p->changeWorkMode(1);//选择模式
	}
}

void CPanelModify::OnDensifyChoosed() //开启选择性加密
{
	if (m_startDensify.GetCheck())
	{
		UpdateData(TRUE);
		if (m_chooseDensify) g_p->changeWorkMode(7.1);//选择性加密模式
		else g_p->changeWorkMode(7);
		if(m_cursorBox.GetCheck()) g_p->m_cursorBox=1;
		g_p->renderScene();	
	}
}

// MMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "MMDlg.h"
#include "Queue.h"

#define Lw 100.0f //定义视见空间边长
#define KAXIS 0.7f//定义坐标轴比例
#define NUM_LATTICE 20 //3D栅格个数
#define INCREASE 6 //显示区间隔像素
#define SIZEOFLATTICE 15.0 //纵向显示的栅格数
#define PRECISION 8
#define AREA 35
#define DELTA 0.000001

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMMDlg *g_p; //全局变量，以便访问主框架
bool g_renderMode;//渲染状态,便于CS访问
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMDlg dialog


CMMDlg::CMMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMMDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMMDlg)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMMDlg, CDialog)
	//{{AFX_MSG_MAP(CMMDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_REDO, OnRedo)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_SELECT_MOVE, OnSelectMove)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_NEWFILE, OnNewfile)
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_SELECT_SIZE, OnSelectSize)
	ON_BN_CLICKED(IDC_SELECT, OnSelect)
	ON_BN_CLICKED(IDC_ROTATE, OnRotate)
	ON_BN_CLICKED(IDC_UNDO, OnUndo)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_OUTPUT, OnOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMDlg message handlers

BOOL CMMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//////////////////变量初始化/////////////////
	g_p=this;
	g_renderMode=1;//1是正常渲染模式，0是挑选模式
	m_theta=70;
	m_phi=30;
	m_R=70;
	m_num_view=4;
	m_projectManner=TRUE;//TRUE表示正投影，FALSE表示透射投影
	m_activeView=0;
	m_viewport=0;//0表示3d窗口在中央，1，2，3依次表示top，front和left视图在中央
	m_view_style=0;//0表示四个均分窗口，1表示3大1小窗口
	m_numOfLattice=15.0;
	m_size[0]=m_size[1]=m_size[2]=m_size[3]=1;
	//m_update[0]=m_update[1]=m_update[2]=m_update[3]=0;
	m_viewManner[1]=m_viewManner[2]=m_viewManner[3]=1;
	m_viewManner[0]=1;
	m_workMode=0;//工作模式，0表示创建，1表示选择，2是移动，3是均匀放缩，4是旋转，5点的拖动
	m_select=0;//开始时没有选中任何点
	m_num_model=0;//开始时载入数为0
	m_viewSide[0]=m_viewSide[1]=m_viewSide[2]=m_viewSide[3]=0;
	m_maxIndex=0;
	m_move=0;//还没有移动物体
	m_drawChooseBox=0;//开始时不绘制选择框
	m_drawRotateCross=0;//开始时不绘制旋转十字架
	m_LBdown=0;//此时所需的左键没有按下
	m_BoxChoose=0;//框选关闭
	m_create=0;//创建状态,0不创建，1长方体，2柱体，3球体，4椎体，5圆环，6管状体
	m_numToCopy=0;
	m_numPointHits=0;//初始时没有点被选中
	m_cursorBox=0;//此时不绘制鼠标框
	
	
	/////////////////初始化快捷键设置////////////////////////
	FILE *fp=fopen("HotKeysSetting.txt","r");
	if (fp)//加载用户快捷键
	{
		fscanf(fp,"%d%d%d%d%d%d%d",&m_hotKeys.theta_plus,&m_hotKeys.theta_minus,&m_hotKeys.phi_plus,
			&m_hotKeys.phi_minus,&m_hotKeys.R_plus,&m_hotKeys.R_minus,&m_hotKeys.del_object);	
	}
	else //默认快捷键
	{
		m_hotKeys.theta_plus=83;
		m_hotKeys.theta_minus=87;
		m_hotKeys.phi_plus=65;
		m_hotKeys.phi_minus=68;
		m_hotKeys.R_plus=VK_PRIOR;
		m_hotKeys.R_minus=VK_NEXT;
		m_hotKeys.del_object=VK_DELETE;
	}
	//////////////////数据/////////////////
	m_cs_head=new CS;
	m_cs_head->next=NULL;
	pActiveCS=NULL; //尚未引入曲面
	////////////////////////设置按钮图标////////////////////////
	((CButton *)GetDlgItem(IDC_OPEN))->SetIcon(AfxGetApp()->LoadIcon(IDI_OPEN));
	((CButton *)GetDlgItem(IDC_NEWFILE))->SetIcon(AfxGetApp()->LoadIcon(IDI_NEW));
	((CButton *)GetDlgItem(IDC_SAVE))->SetIcon(AfxGetApp()->LoadIcon(IDI_SAVE));
	((CButton *)GetDlgItem(IDC_OUTPUT))->SetIcon(AfxGetApp()->LoadIcon(IDI_OUTPUT));
	((CButton *)GetDlgItem(IDC_UNDO))->SetIcon(AfxGetApp()->LoadIcon(IDI_UNDO));
	((CButton *)GetDlgItem(IDC_REDO))->SetIcon(AfxGetApp()->LoadIcon(IDI_REDO));
	((CButton *)GetDlgItem(IDC_SELECT))->SetIcon(AfxGetApp()->LoadIcon(IDI_SELECT));
	((CButton *)GetDlgItem(IDC_SELECT_MOVE))->SetIcon(AfxGetApp()->LoadIcon(IDI_MOVE));
	((CButton *)GetDlgItem(IDC_SELECT_SIZE))->SetIcon(AfxGetApp()->LoadIcon(IDI_SIZE));
	((CButton *)GetDlgItem(IDC_ROTATE))->SetIcon(AfxGetApp()->LoadIcon(IDI_ROTATE));
	////////////////////////////////////////////////////////////
	calculateRc();//计算4个绘图区的相对大区域的位置
	GetClientRect(&m_old_rect);
	////////////////////////控制面板初始化////////////////////////
	//插入标签
	m_tab.InsertItem(0, _T("创建"));
    m_tab.InsertItem(1, _T("修改"));
	//创建对话框
    m_page1.Create(IDD_PANEL_CREATE, &m_tab);
    m_page2.Create(IDD_PANEL_MODIFY, &m_tab);
	//设定在Tab内显示的范围
    CRect rc;
    m_tab.GetClientRect(rc);
    rc.top += 20;
    rc.bottom -= 8;
    //rc.left += 8;
    //rc.right -= 8; 
    m_page1.MoveWindow(&rc);
    m_page2.MoveWindow(&rc);
    //用数组把对话框对象指针保存起来
    pDlg[0] = &m_page1;
    pDlg[1] = &m_page2;
    //显示初始页面
    pDlg[0]->ShowWindow(SW_SHOW);
    pDlg[1]->ShowWindow(SW_HIDE);
	m_cur=0; 	
	////////////////////////状态面板初始化////////////////////////
	rc.left=INCREASE;
	rc.right=m_old_rect.right-INCREASE;
	CRect rcALL;
	GetDlgItem(IDC_ALL)->GetClientRect(&rcALL);
	rc.top=rcALL.bottom;
	rc.bottom=m_old_rect.bottom;
	m_statePanel.Create(IDD_PANEL_STATE);
	ClientToScreen(&rc);
	m_statePanel.MoveWindow(&rc);
	m_statePanel.ShowWindow(SW_SHOW);
	//////////////////////初始化颜色面板//////////////////////////
	fp=fopen("SavedColor.txt","r");
	if (fp)
	{
		COLORREF* color=m_pallete.GetSavedCustomColors();
		byte r,g,b;
		for (int i=0;i<16;i++)
		{
			fscanf(fp,"%d%d%d",&r,&g,&b);
			color[i]=RGB(r,g,b);
		}
		fscanf(fp,"%d%d%d",&r,&g,&b);
		m_activeColor=RGB(r,g,b);//读取活动颜色
		fclose(fp);
	}
	//读入64种基本色
	fp=fopen("BasicColors.txt","r"); 
	if (fp)
	{
		for (int i=0;i<64;i++)
		{
			fscanf(fp,"%d%d%d",&m_colors[i][0],&m_colors[i][1],&m_colors[i][2]);
		}
		fclose(fp);
	}
	else
	{
		AfxMessageBox("错误：BasicColors.txt丢失！\n  程序需要关闭...");
		exit(0);
	}
	getRandomColor();//随机指定一种颜色
	//////////////////系统时间产生随机数种子/////////////////////
	CTime tm=CTime::GetCurrentTime();
	short y,m,d,h,n,s;
	y=tm.GetYear();
	m=tm.GetMonth();
	d=tm.GetDay();
	h=tm.GetHour();
	n=tm.GetMinute();
	s=tm.GetSecond();
	m_In=y+70*(m+12*(d+31*(h+23*(n+59*s))));
	///////////////////////////获取程序当前运行路径,便于保存文件///////////////////
	TCHAR   exeFullPath[256]; 
	DWORD   path=GetCurrentDirectory(256,exeFullPath); 	
	m_directory=exeFullPath;
	m_directory+="\\";
	//m_directory.Replace("\\", "\\\\");
	///////////////////////////初始化视图模型矩阵/////////////////////////////////
	InitOpenGL();//初始化openGL
	for (int i=0;i<4;i++)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		lookAt(i);
		glScalef(50,50,50);
		glScalef(m_size[i],m_size[i],m_size[i]);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix[i]);
	}
	m_knife.init();//初始化切割面数据
	//////////////////////////////////////////////////////////////////
	SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE);//初始时窗口最大化
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMMDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;	
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	renderScene();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMMDlg::InitOpenGL()//初始化openGL
{
	//获得绘图DC
	m_pdc=GetDlgItem(IDC_ALL)->GetDC();
	if (!bSetupPixelFormat(m_pdc))	return;
	HGLRC hrc= wglCreateContext(m_pdc->GetSafeHdc());
	wglMakeCurrent(m_pdc->GetSafeHdc(), hrc);
	//////////////创建位图字体//////////////////
	char fontname[]="Arial"; //选择名字为Arial的字体
	HFONT hfont;
	LOGFONT logfont;
	logfont.lfHeight		= -20;
	logfont.lfWidth			= 0;
	logfont.lfEscapement	= 0;
	logfont.lfOrientation	= logfont.lfEscapement;
	logfont.lfWeight		= FW_NORMAL;
	logfont.lfItalic		= FALSE;
	logfont.lfUnderline		= FALSE;
	logfont.lfStrikeOut		= FALSE;
	logfont.lfCharSet		= ANSI_CHARSET;
	logfont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	logfont.lfQuality		= DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = FF_DONTCARE|DEFAULT_PITCH;
	lstrcpy ( logfont.lfFaceName, fontname );
	hfont=CreateFontIndirect( &logfont );
	SelectObject(m_pdc->m_hDC,hfont);
	m_id=glGenLists(256);
	wglUseFontBitmaps( m_pdc->m_hDC, 0, 256, m_id );
	DeleteObject(hfont);//删除旧字体	
	//////////////////////////////////
	//开启深度测试
	glEnable(GL_DEPTH_TEST);  
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);	
	//消除锯齿线
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	//只绘制逆时针正面
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//光照设置
	GLfloat ambientLight[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat diffuseLight[]={0.7f,0.7f,0.7f,1.0f};
	GLfloat lightPos[]={80.0f,80.0f,80.0f,0.0f};//光源位置
	GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);	
	//启用颜色追踪
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMateriali(GL_FRONT,GL_SHININESS,128);

	glShadeModel(GL_SMOOTH);
	glSelectBuffer(BUFFER_LENGTH,m_selectBuff);//设置选择缓冲区
	glPointSize(5.0);//设置选中点显示的大小

	glEnable(GL_RESCALE_NORMAL);//法向量自动标准化
	glClearColor(0.5, 0.5, 0.5, 0.0f);//设置清屏颜色
}

bool CMMDlg::bSetupPixelFormat(CDC * m_pdc)  //设置像素格式函数
{
	int n;
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;
	if ( (pixelformat = ChoosePixelFormat(m_pdc->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox("像素匹配失败！");
		return FALSE;
	}
	if (SetPixelFormat(m_pdc->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("像素设置失败！");
		return FALSE;
	}
	n = ::GetPixelFormat(m_pdc->GetSafeHdc());
	::DescribePixelFormat(m_pdc->GetSafeHdc(), n, sizeof(pfd), &pfd);

	return TRUE;
}

BOOL CMMDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	
	return CDialog::PreCreateWindow(cs);
}

void CMMDlg::OnClose() 
{
	if(m_num_model) //有模型载入
	{
		int choice=MessageBox("是否保存模型文件?","保存提示:",MB_YESNOCANCEL);
		if(choice!=IDCANCEL)
		{
			if (choice==IDYES&&!saveFile(0)) return;	
		}
		else return;
	}
	HGLRC hrc = ::wglGetCurrentContext();
	::wglMakeCurrent(NULL,  NULL);
	if (hrc) ::wglDeleteContext(hrc);
	//保存颜色板
	FILE *fp=fopen(m_directory+"SavedColors.txt","w");
	if (fp)
	{
		COLORREF* color=m_pallete.GetSavedCustomColors();
		byte r,g,b;
		for (int i=0;i<16;i++)
		{
			r=GetRValue(color[i]);
			g=GetGValue(color[i]);
			b=GetBValue(color[i]);
			fprintf(fp,"%d\t%d\t%d\n",r,g,b);
		}
		r=GetRValue(m_activeColor);
		g=GetGValue(m_activeColor);
		b=GetBValue(m_activeColor);
		fprintf(fp,"%d\t%d\t%d\n",r,g,b);//当前选中的颜色
		fclose(fp);
	}
	CDialog::OnClose();	
}


void CMMDlg::renderScene()
{
	int i=0;	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲区和深度缓冲区
	/*****************以下开始平面绘制*****************/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,rcALL.Width(),0,rcALL.Height(),-Lw,Lw);//更改投影空间与屏幕一致，便于平面绘图
	glViewport(0,0,rcALL.Width(),rcALL.Height());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDepthMask(GL_TRUE);
	if (m_drawChooseBox)//绘制选择框
	{		
		glColor3ub(255,255,255);
		glBegin(GL_LINE_LOOP);
			glVertex2d(m_pp[0][0],m_pp[0][1]);
			glVertex2d(m_pp[0][0],m_pp[2][1]);
			glVertex2d(m_pp[2][0],m_pp[2][1]);
			glVertex2d(m_pp[2][0],m_pp[0][1]);
		glEnd();
	}
	if (m_drawRotateCross)//绘制旋转十字架
	{	
		glColor3ub(0,255,0);
		glBegin(GL_LINES);
			glVertex3d(m_pp[0][0]-80,m_pp[0][1],Lw);
			glVertex3d(m_pp[0][0]+80,m_pp[0][1],Lw);
			glVertex3d(m_pp[0][0],m_pp[0][1]-80,Lw);
			glVertex3d(m_pp[0][0],m_pp[0][1]+80,Lw);
		glEnd();
		glBegin(GL_LINE_LOOP);
			for (int i=0;i<360;i++)
			{
				glVertex3d(m_pp[0][0]+80*cos(i*PI/180.0),m_pp[0][1]+80*sin(i*PI/180.0),Lw);
			}
		glEnd();
		glEnable(GL_LINE_STIPPLE);//开启点画线
		glLineStipple(1,0x01f1);
		glColor3ub(255,255,0);
		glBegin(GL_LINES);
			glVertex3d(m_pp[0][0]-80*cos(PI/4),m_pp[0][1]-80*cos(PI/4),Lw);
			glVertex3d(m_pp[0][0]+80*cos(PI/4),m_pp[0][1]+80*cos(PI/4),Lw);
			glVertex3d(m_pp[0][0]-80*cos(PI/4),m_pp[0][1]+80*cos(PI/4),Lw);
			glVertex3d(m_pp[0][0]+80*cos(PI/4),m_pp[0][1]-80*cos(PI/4),Lw);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
		glColor3ub(255,0,0);
		glBegin(GL_LINES);
			glVertex3d(m_pp[0][0],m_pp[0][1],Lw);
			glVertex3d(m_pp[2][0],m_pp[2][1],Lw);
		glEnd();
		int dx=m_pp[2][0]-m_pp[0][0],dy=m_pp[2][1]-m_pp[0][1];
		CString s;
		if(abs(dx)>abs(dy)) s.Format("angle=%d",abs(dx));
		else s.Format("angle=%d",abs(dy));
		GLTextOut(m_pp[0][0]-40,m_pp[0][1]+30,Lw,s.GetBuffer(15));
	}
	if (m_cursorBox&&(m_workMode==5||m_workMode==7.1))//绘制鼠标框
	{
		glColor3ub(255,0,0);
		glBegin(GL_LINE_LOOP);
		glVertex3d(m_pp[2][0]-m_page2.m_cursorW/2,m_pp[2][1]-m_page2.m_cursorH/2,Lw);
		glVertex3d(m_pp[2][0]+m_page2.m_cursorW/2,m_pp[2][1]-m_page2.m_cursorH/2,Lw);		
		glVertex3d(m_pp[2][0]+m_page2.m_cursorW/2,m_pp[2][1]+m_page2.m_cursorH/2,Lw);
		glVertex3d(m_pp[2][0]-m_page2.m_cursorW/2,m_pp[2][1]+m_page2.m_cursorH/2,Lw);
		glEnd();
	}
	glDepthMask(GL_FALSE);//关闭深度写入
	if(m_num_view==1)//单视图
	{
		int width=rcALL.Height(),length=rcALL.Width();
		if (m_activeView!=0)//非3D视图时绘制平面栅格
		{
			glColor3ub(155,155,155);//设置栅格颜色
			glBegin(GL_LINES);			
				//m_numOfLattice=20;
				int delta=width/m_numOfLattice;  //一个栅格的边长
				for(int y=0;y<=width;y+=delta)
				{			
					glVertex2f(0,y);
					glVertex2f(length,y);
				}
				for(int x=0;x<=length;x+=delta)
				{			
					glVertex2f(x,0);
					glVertex2f(x,width);
				}
			glEnd();
		}
		//绘制活动视图的边框
		glColor3ub(255,255,0);
		glBegin(GL_LINE_LOOP);
			glVertex2f(0,0);
			glVertex2f(length,0);
			glVertex2f(length,width);
			glVertex2f(0,width);
		glEnd();
		//绘制显示视区提示文本		
		if (m_activeView==0) GLTextOut(0,width-20,0,"3D");
		else switch(m_activeView+3*m_viewSide[m_activeView])
		{
		case 1:GLTextOut(0,width-20,0,"top");break;
		case 2:GLTextOut(0,width-20,0,"front");break;
		case 3:GLTextOut(0,width-20,0,"left");break;
		case 4:GLTextOut(0,width-20,0,"bottom");break;
		case 5:GLTextOut(0,width-20,0,"back");break;
		case 6:GLTextOut(0,width-20,0,"right");break;
		default: break;
		}
		//绘制返回按钮
		glBegin(GL_LINE_LOOP);
			glVertex2f(length-40,width-30);
			glVertex2f(length-6,width-30);
			glVertex2f(length-6,width-6);
			glVertex2f(length-40,width-6);
		glEnd();
		GLTextOut(length-39,width-28,0,"Min");
		
		/********************3D绘图*******************/
		glDepthMask(GL_TRUE);
		CS *pCS=m_cs_head;
		float aspectRatio=(float)(rcALL.Width())/rcALL.Height();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-Lw*aspectRatio,Lw*aspectRatio,-Lw,Lw,-10*Lw,10*Lw);//更改视见空间
		glMatrixMode(GL_MODELVIEW);
		lookAt(m_activeView+3*m_viewSide[m_activeView]);
		glScalef(m_size[m_activeView],m_size[m_activeView],m_size[m_activeView]);
		if(m_activeView==0) draw3DLattice();
		DrawModel();
		while (pCS->next)
		{
			pCS=pCS->next;
			if (pCS->m_visible) pCS->show(m_viewManner[m_activeView],m_workMode);
		}
		if (m_workMode==0) drawNewObject();//创建时绘制新物体
		if(m_workMode==6) drawKnife();//绘制切割刀
	}
	else //四视图
	{
		//绘制活动视图的边框
		glColor3ub(255,255,0);
		glBegin(GL_LINE_LOOP);	
			glVertex2f(rc[m_activeView].left,rc[m_activeView].bottom);
			glVertex2f(rc[m_activeView].right,rc[m_activeView].bottom);
			glVertex2f(rc[m_activeView].right,rc[m_activeView].top);
			glVertex2f(rc[m_activeView].left,rc[m_activeView].top);	
		glEnd();
		//绘制平面栅格
		glColor3ub(155,155,155);//设置栅格颜色
		glBegin(GL_LINES);
			int width,length,x,y,delta;
			for (i=1;i<=3;i++)
			{
				width=rc[i].top-rc[i].bottom;
				length=rc[i].right-rc[i].left;
				delta=width/m_numOfLattice;  //一个栅格的边长
				for(y=0;y<=width;y+=delta)
				{			
					glVertex2f(rc[i].left,rc[i].bottom+y);
					glVertex2f(rc[i].right,rc[i].bottom+y);
				}
				for(x=0;x<=length;x+=delta)
				{			
					glVertex2f(rc[i].left+x,rc[i].bottom);
					glVertex2f(rc[i].left+x,rc[i].top);
				}
			}			
		glEnd();
		glColor3ub(255,255,0);//字体为黄色
		//绘制提示文本
		GLTextOut(rc[0].left,rc[0].top-20,0,"3D");
		if(m_viewSide[1]) GLTextOut(rc[1].left,rc[1].top-20,0,"bottom");
		else GLTextOut(rc[1].left,rc[1].top-20,0,"top");
		if(m_viewSide[2]) GLTextOut(rc[2].left,rc[2].top-20,0,"back");
		else GLTextOut(rc[2].left,rc[2].top-20,0,"front");
		if(m_viewSide[3]) GLTextOut(rc[3].left,rc[3].top-20,0,"right");
		else GLTextOut(rc[3].left,rc[3].top-20,0,"left");
		//绘制最大化按钮
		for (i=0;i<4;i++)
		{
			glBegin(GL_LINE_LOOP);
				glVertex2f(rc[i].right-25,rc[i].top-25);
				glVertex2f(rc[i].right-2,rc[i].top-25);
				glVertex2f(rc[i].right-2,rc[i].top-2);
				glVertex2f(rc[i].right-25,rc[i].top-2);
			glEnd();
			GLTextOut(rc[i].right-23,rc[i].top-23,0,"M");
		}
		/**********************3D绘图**************************/
		glDepthMask(GL_TRUE);
		CS *pCS=m_cs_head;
		float ratio;//视图比例辅助变量		
		//绘制四个视图
		for (i=0;i<4;i++)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			ratio=(float)(rc[i].right-rc[i].left)/(rc[i].top-rc[i].bottom);
			if(ratio>1)	glOrtho(-Lw*ratio,Lw*ratio,-Lw,Lw,-10*Lw,10*Lw);
			else glOrtho(-Lw,Lw,-Lw/ratio,Lw/ratio,-10*Lw,10*Lw);
			glMatrixMode(GL_MODELVIEW);
			glViewport(rc[i].left,rc[i].bottom,rc[i].right-rc[i].left,rc[i].top-rc[i].bottom);
			lookAt(i+3*m_viewSide[i]);
			glScalef(m_size[i],m_size[i],m_size[i]);
			if(!i) draw3DLattice();//view视图要绘制三维网格
			DrawModel();
			pCS=m_cs_head;
			while (pCS->next)
			{
				pCS=pCS->next;
				if (pCS->m_visible) pCS->show(m_viewManner[i],m_workMode);
			}
			if (m_workMode==0) drawNewObject();//创建时绘制新物体
			if(m_workMode==6) drawKnife();//绘制切割刀
		}
	} 
	glFinish(); //强制绘图，无论缓冲区是否满了
	SwapBuffers(wglGetCurrentDC()); //交换视图，显示画面（含双缓存机制)	
}

void CMMDlg::lookAt(short i)//调整观察视角
{
	glLoadIdentity();
	switch(i)
	{
	case 0:		
		{
			double ks1=sin(m_theta*PI/180.0),ks2=sin(m_phi*PI/180.0),kc1=cos(m_theta*PI/180.0),kc2=cos(m_phi*PI/180.0);
			gluLookAt(m_R*ks1*kc2,m_R*ks1*ks2,m_R*kc1,0.0f,0.0f,0.0f,-kc1*kc2,-kc1*ks2,ks1); //确定视角
		}
		break;
	case 1: //top
		gluLookAt(0,0,m_R,0.0f,0.0f,0.0f,-1,0,0);
		break;
	case 2: //front
		gluLookAt(m_R,0,0,0.0f,0.0f,0.0f,0,0,1);
		break;
	case 3: //left
		gluLookAt(0,-m_R,0,0.0f,0.0f,0.0f,0,0,1);
		break;
	case 4: //bottom
		gluLookAt(0,0,-m_R,0.0f,0.0f,0.0f,-1,0,0);
		break;
	case 5: //back
		gluLookAt(-m_R,0,0,0.0f,0.0f,0.0f,0,0,1);
		break;
	case 6: //right
		gluLookAt(0,m_R,0,0.0f,0.0f,0.0f,0,0,1);
		break;
	default:
		break;
	}
}

void CMMDlg::DrawModel() //绘制模型部分
{
	/**************绘制坐标轴****************/
	glColor3f(1.0,1.0,1.0);//绘图颜色设为白色
	glBegin(GL_LINES);
	glVertex3f(-KAXIS*Lw,0.0f,0.0f);
	glVertex3f(KAXIS*Lw,0.0f,0.0f);
	glVertex3f(0.0f,-KAXIS*Lw,0.0f);
	glVertex3f(0.0f,KAXIS*Lw,0.0f);
	glVertex3f(0.0f,0.0f,-KAXIS*Lw);
	glVertex3f(0.0f,0.0f,KAXIS*Lw);
	glEnd();
	glBegin(GL_LINE_STRIP); //绘制x轴箭头
	glVertex3f(KAXIS*0.95*Lw,0.0f,0.02*Lw);
	glVertex3f(KAXIS*Lw,0.0f,0.0f);
	glVertex3f(KAXIS*0.95*Lw,0.0f,-0.02*Lw);
	glEnd();
	GLTextOut(KAXIS*Lw*1.1,0.0f,0,"x");
	glBegin(GL_LINE_STRIP); //绘制y轴箭头
	glVertex3f(0,KAXIS*0.95*Lw,0.02*Lw);// 
	glVertex3f(0.0f,KAXIS*Lw,0.0f);
	glVertex3f(0,KAXIS*0.95*Lw,-0.02*Lw);
	glEnd();
	GLTextOut(0,KAXIS*Lw*1.1,0,"y");
	glBegin(GL_LINE_STRIP); //绘制z轴箭头
	glVertex3f(0,0.02*Lw,KAXIS*0.95*Lw);
	glVertex3f(0,0.0f,KAXIS*Lw);
	glVertex3f(0,-0.02*Lw,KAXIS*0.95*Lw);
	glEnd();
	GLTextOut(0,0,KAXIS*Lw*1.1,"z");
	/****************绘制实体模型*******************/
	glScalef(50,50,50);
}

BOOL CMMDlg::PreTranslateMessage(MSG* pMsg) //拦截即将分发的消息，并预处理
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==m_hotKeys.theta_minus)
		{				
			m_theta-=5;
			if(m_theta<0) m_theta=0;
		}
		else if(pMsg->wParam==m_hotKeys.theta_plus)
		{
			m_theta+=5;
			if(m_theta>180) m_theta=180;
		}
		else if(pMsg->wParam==m_hotKeys.phi_minus)
		{
			m_phi-=5;
			if(m_phi<0) m_phi+=360;
		}
		else if(pMsg->wParam==m_hotKeys.phi_plus)
		{
			m_phi+=5;
			if(m_phi>360) m_phi-=360;
		}
		else if (pMsg->wParam==m_hotKeys.R_plus) m_R+=2;
		else if (pMsg->wParam==m_hotKeys.R_minus) 
		{	
			if (m_R>2)	m_R-=2;
		}
		else if (pMsg->wParam==m_hotKeys.del_object)
		{
			deleteObject();
		}
		else return CDialog::PreTranslateMessage(pMsg);	
		//更新0号窗口视图模型矩阵
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		lookAt(0);
		glScalef(50,50,50);
		glScalef(m_size[0],m_size[0],m_size[0]);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix[0]);
		renderScene();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMMDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) //选项卡切换响应
{
	pDlg[m_cur]->ShowWindow(SW_HIDE);
    m_cur = m_tab.GetCurSel();
    pDlg[m_cur]->ShowWindow(SW_SHOW);	

	*pResult = 0;
}

void CMMDlg::draw3DLattice()
{
	glDepthMask(GL_FALSE);//绘制栅格时禁止写入深度值
	glColor3ub(155,155,155);//设置栅格颜色
	static float lw=Lw;//*0.9;
	static float step=2*lw/NUM_LATTICE;//栅格步长
	int i;
	for (i=0;i<=NUM_LATTICE;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(-lw+i*step,-lw);
		glVertex2f(-lw+i*step,lw);
		glEnd();	
	}
	for (i=0;i<=NUM_LATTICE;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(-lw,-lw+i*step);
		glVertex2f(lw,-lw+i*step);
		glEnd();		
	}	
	glDepthMask(GL_TRUE);//恢复深度写入
}

void CMMDlg::drawPlaneLattice()//绘制栅格
{
	glLoadIdentity();
	//绘制边框
	glColor3ub(54.0,63.0,75.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-Lw,-Lw);
		glVertex2f(Lw,-Lw);
		glVertex2f(Lw,Lw);
		glVertex2f(-Lw,Lw);
	glEnd();
	glColor3ub(155,155,155);//设置栅格颜色
	float step=2*Lw/NUM_LATTICE;//栅格步长
	int i;
	for (i=1;i<NUM_LATTICE;i++)
	{
		glBegin(GL_LINES);
			glVertex2f(-Lw+i*step,-Lw);
			glVertex2f(-Lw+i*step,Lw);
		glEnd();	
	}
	for (i=0;i<NUM_LATTICE;i++)
	{
		glBegin(GL_LINES);
			glVertex2f(-Lw,-Lw+i*step);
			glVertex2f(Lw,-Lw+i*step);
		glEnd();		
	}
}

void CMMDlg::changeViewportStyle(short m)//改变视区，在1个或4个投影区进行切换
{
	if(m==1) //切换到单一视图
	{
		float aspectRatio=(float)(rcALL.right-rcALL.left)/(rcALL.bottom-rcALL.top);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(m_projectManner) glOrtho(-Lw*aspectRatio,Lw*aspectRatio,-Lw,Lw,-Lw,Lw);//更改投影空间
		else gluPerspective(45,aspectRatio,-100,100);
		glViewport(0,0,rcALL.right-rcALL.left,rcALL.bottom-rcALL.top);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		m_num_view=1;
	}
	else //切换到4个视图
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(m_projectManner) glOrtho(-Lw,Lw,-Lw,Lw,-Lw,Lw);//更改投影空间
		else gluPerspective(45,1,100,200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		m_num_view=4;
	}
}


bool CMMDlg::Openfile(byte type)//获取要打开文件的路径
{
	int structsize=0;//显示风格变量
	DWORD dwVersion= GetVersion();// 检测目前的操作系统 
	if (dwVersion < 0x80000000)	structsize =88; //操作系统是Windows NT/2000
	else structsize =76; //Windows 95/98系统
	CString filter,defEx;
	if (type==0)//打开mm和smm文件
	{
		filter="ModelMaker文件(*.mm)|*.mm|单个ModelMaker模型(*.smm)|*.smm||";
		defEx="mm";
	}
	else if (type==1)//mm文件
	{
		filter="ModelMaker文件(*.mm)|*.mm||";
		defEx="mm";
	}
	else if(type==2)//smm文件
	{
		filter="ModelMaker文件(*.smm)|*.smm||";
		defEx="smm";
	}
	CFileDialog openFile(TRUE, defEx, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
	openFile.m_ofn.lStructSize = structsize;
	CString s;	
	if (type==0||type==1) 
	{	
		s=m_directory+"SavedScene";
		openFile.m_ofn.lpstrInitialDir=s;
	}
	else if (type==2) 
	{	
		s=m_directory+"SavedModel";
		openFile.m_ofn.lpstrInitialDir=s;
	}
	if(openFile.DoModal()!=IDOK) return FALSE;
	if (openFile.GetFileExt()=="mm")//mm文件
	{
		CS *pCS,*q;
		if (m_num_model) //原来场景有文件
		{
			int choice=MessageBox("是否替换现在场景？\nyes:替换当前场景\nno:添加到当前场景\ncancel:取消打开","场景替换",MB_YESNOCANCEL);
			if (choice==IDCANCEL) return FALSE;
			else if (choice==IDYES) //需删除场景中的所有物体
			{
				//删除旧场景
				pCS=m_cs_head->next;
				m_cs_head->next=NULL;
				while (pCS)
				{
					q=pCS;
					pCS=pCS->next;
					delete q;
				}
				m_num_model=0;
			}
		}		
		//打开mm文件，载入模型
		pCS=m_cs_head;
		while(pCS->next) pCS=pCS->next;
		FILE *fp=fopen(openFile.GetPathName(),"r");
		if (!fp)
		{
			MessageBox("文件打开失败！");
			return FALSE;
		}
		CString id=readLine(fp);
		if (id!="This is a ModelMaker file")
		{
			MessageBox("这不是mm文件！");
			fclose(fp);
			return FALSE;
		}
		int num=0;
		fscanf(fp,"%d",&num);//模型的个数
		int j=0,temp;
		char name[40];
		Triangle *pt;
		for (int i=0;i<num;i++)
		{
			while(readLine(fp)!="*here is the beginning of a model*");
			pCS->next=new CS;
			pCS=pCS->next;
			fscanf(fp,"%lf",&pCS->m_size);
			for (j=0;j<16;j++)
			{
				fscanf(fp,"%lf",&pCS->m_mvMatrix[j]);
			}
			fscanf(fp,"%lf%lf%lf",&pCS->m_translate[0],&pCS->m_translate[1],&pCS->m_translate[2]);
			fscanf(fp,"%d",&pCS->m_color[0]);
			fscanf(fp,"%d",&pCS->m_color[1]);
			fscanf(fp,"%d",&pCS->m_color[2]);
			fscanf(fp,"%d",&temp);
			pCS->m_selected=temp;
			fscanf(fp,"%d",&temp);
			pCS->m_visible=temp;
			fscanf(fp,"%d",&temp);
			pCS->m_free=temp;
			fscanf(fp,"%d",&pCS->m_num_triangle);
			fscanf(fp,"%s",&name);
			pCS->m_name=name;
			//以下是三角形数据
			pt=pCS->it;
			for (j=0;j<pCS->m_num_triangle;j++)
			{
				pt->next=new Triangle;
				pt=pt->next;
				fscanf(fp,"%lf%lf%lf",&pt->pt[0].v[0],&pt->pt[0].v[1],&pt->pt[0].v[2]);
				fscanf(fp,"%lf%lf%lf",&pt->pt[1].v[0],&pt->pt[1].v[1],&pt->pt[1].v[2]);
				fscanf(fp,"%lf%lf%lf",&pt->pt[2].v[0],&pt->pt[2].v[1],&pt->pt[2].v[2]);
			}
			pt->next=NULL;
			//此时一个CS数据已载入
			m_num_model++;
			m_maxIndex++;
			pCS->m_index=m_maxIndex;//分配索引
			pCS->createVertexRef();
		}
		pCS->next=NULL;
		m_filePath=openFile.GetPathName();//记录打开的文件位置
	}
	else //smm文件,直接添加进原来场景
	{
		CS *p=m_cs_head;
		while (p->next) p=p->next;
		p->next=new CS;
		p=p->next;
		p->next=NULL;
		p->loadCS(openFile.GetPathName());//载入模型
		m_maxIndex++;
		p->m_index=m_maxIndex;//赋名称编号	
		m_workMode=1;//?
		m_num_model++;
	}
	renderScene();
	return TRUE;
	
}

void CMMDlg::OnMouseMove(UINT nFlags, CPoint point) //鼠标移动时的响应
{
	CDialog::OnMouseMove(nFlags, point);	
	bool redraw=0;
	toGLCoordinate(&point);
	m_pp[2][0]=point.x;
	m_pp[2][1]=point.y;//记录此时鼠标的位置，以便绘制选择框
	CS *pCS;		
    /**********************************************************/	
	if (m_workMode==0)//创建模式
	{
		byte aim;
		if (m_num_view==4) aim=m_activeView;
		else aim=4;
		if (m_create==1.1||m_create==1.2)//创建长方体
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[1][0]=m_vertex[1][0]-ax/az*m_vertex[1][2];
				m_vertex[1][1]=m_vertex[1][1]-ay/az*m_vertex[1][2];
				m_vertex[1][2]=0;
			}
			if (m_activeView==1)//在top视口创建
			{		
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				m_vertex[0][2]=0;
			}
			if (m_activeView==2)//在front视口创建
			{		
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				m_vertex[0][0]=0;
			}
			if (m_activeView==3)//在left视口创建
			{		
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				m_vertex[0][1]=0;
			}
			m_create=1.2;
		}
		if (m_create==1.3||m_create==1.4)//创建长方体第二次移动
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)m_pp[1][0],(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[2][2]=az/ax*(m_vertex[1][0]-m_vertex[2][0])+m_vertex[2][2];
				m_vertex[2][0]=m_vertex[1][0];
				m_vertex[2][1]=m_vertex[1][1];
			}
			if (m_activeView==1)//在视口1创建
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				m_vertex[2][2]=-m_vertex[2][0]+m_vertex[1][0];
				m_vertex[2][0]=m_vertex[1][0];
				m_vertex[2][1]=m_vertex[1][1];				
			}
			if (m_activeView==2)//在视口2创建
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				m_vertex[2][0]=-m_vertex[2][2]+m_vertex[1][2];
				m_vertex[2][1]=m_vertex[1][1];
				m_vertex[2][2]=m_vertex[1][2];				
			}
			if (m_activeView==3)//在视口3创建
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				m_vertex[2][1]=m_vertex[2][2]-m_vertex[1][2];
				m_vertex[2][0]=m_vertex[1][0];
				m_vertex[2][2]=m_vertex[1][2];				
			}
			m_create=1.4;
		}
		if (m_create==2.11||m_create==2.12)//创建柱体
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[1][0]=m_vertex[1][0]-ax/az*m_vertex[1][2];
				m_vertex[1][1]=m_vertex[1][1]-ay/az*m_vertex[1][2];
				m_vertex[1][2]=0;
				m_cylinder.R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+
							(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
			}
			m_create=2.12;
		}
		if (m_create==2.13||m_create==2.14)//创建柱体
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)m_pp[1][0],(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[2][2]=az/ax*(m_vertex[1][0]-m_vertex[2][0])+m_vertex[2][2];
				m_vertex[2][0]=m_vertex[0][0];
				m_vertex[2][1]=m_vertex[0][1];
			}
			m_create=2.14;
		}
		if(m_create==2.2)//点画线创建
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[1][0]=m_vertex[1][0]-ax/az*m_vertex[1][2];
				m_vertex[1][1]=m_vertex[1][1]-ay/az*m_vertex[1][2];
				m_vertex[1][2]=0;
				//修改尾部坐标
				m_cylinder.end->x2=m_vertex[1][0];
				m_cylinder.end->y2=m_vertex[1][1];
				m_cylinder.end->z2=0;
			}
			redraw=1;//需重绘
		}
		if (m_create==2.21)//柱体获取高度
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)m_pp[1][0],(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_cylinder.h=az/ax*(m_vertex[1][0]-m_vertex[2][0])+m_vertex[2][2];
			}
			redraw=1;
		}
		if (m_create==4.11||m_create==4.12)//创建锥体
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[1][0]=m_vertex[1][0]-ax/az*m_vertex[1][2];
				m_vertex[1][1]=m_vertex[1][1]-ay/az*m_vertex[1][2];
				m_vertex[1][2]=0;
				m_cylinder.R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+
					(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
			}
			m_create=4.12;
		}
		if (m_create==4.13||m_create==4.14)//创建锥体
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)m_pp[1][0],(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[2][2]=az/ax*(m_vertex[1][0]-m_vertex[2][0])+m_vertex[2][2];
				m_vertex[2][0]=m_vertex[0][0];
				m_vertex[2][1]=m_vertex[0][1];
			}
			m_create=4.14;
		}
		if (m_create==3.1||m_create==3.2)//创建球体
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[1][0]=m_vertex[1][0]-ax/az*m_vertex[1][2];
				m_vertex[1][1]=m_vertex[1][1]-ay/az*m_vertex[1][2];
				m_vertex[1][2]=0;
			}
			m_create=3.2;
		}
		if (m_create==5.1||m_create==5.2)//创建轮胎
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[1][0]=m_vertex[1][0]-ax/az*m_vertex[1][2];
				m_vertex[1][1]=m_vertex[1][1]-ay/az*m_vertex[1][2];
				m_vertex[1][2]=0;
			}
			m_create=5.2;
		}
		if (m_create==5.3||m_create==5.4)//创建torus
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[2][0]=m_vertex[2][0]-ax/az*m_vertex[2][2];
				m_vertex[2][1]=m_vertex[2][1]-ay/az*m_vertex[2][2];
				m_vertex[2][2]=0;
			}
			m_create=5.4;
		}
		if (m_create==6.1||m_create==6.2)//创建pipe
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[1][0]=m_vertex[1][0]-ax/az*m_vertex[1][2];
				m_vertex[1][1]=m_vertex[1][1]-ay/az*m_vertex[1][2];
				m_vertex[1][2]=0;
			}
			m_create=6.2;
		}
		if (m_create==6.3||m_create==6.4)//创建pipe
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[2][0]=m_vertex[2][0]-ax/az*m_vertex[2][2];
				m_vertex[2][1]=m_vertex[2][1]-ay/az*m_vertex[2][2];
				m_vertex[2][2]=0;
			}
			m_create=6.4;
		}
		if (m_create==6.6||m_create==6.7)//创建pipe
		{
			if (m_activeView==0)//在3D窗口创建，求得该点的3D坐标
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[3][0],&m_vertex[3][1],&m_vertex[3][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[3][2]=az/ax*(m_vertex[2][0]-m_vertex[3][0])+m_vertex[3][2];
				m_vertex[3][0]=m_vertex[2][0];
				m_vertex[3][1]=m_vertex[2][1];
			}
			m_create=6.7;
		}
		redraw=1;
	}

	/**********************************************************/
	if(m_LBdown)
	{
		if(m_workMode==1||m_workMode==3)//选择模式
		{
			m_drawChooseBox=1;//绘制选择框			
		}

		if (m_workMode==2)//移动模式
		{
			if (m_BoxChoose)	m_drawChooseBox=1;//绘制选择框
			if (m_move)//进入移动模式
			{
				GLdouble vertex[2][3];
				int aim;
				if (m_num_view==4) aim=m_activeView;
				else aim=4;
				gluUnProject((GLdouble)m_pp[0][0],(GLdouble)m_pp[0][1],0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&vertex[0][0],&vertex[0][1],&vertex[0][2]);
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&vertex[1][0],&vertex[1][1],&vertex[1][2]);

				pCS=m_cs_head;
				while (pCS->next)
				{
					pCS=pCS->next;
					if (pCS->m_selected&&pCS->m_free)
					{
						pCS->tx=vertex[1][0]-vertex[0][0];
						pCS->ty=vertex[1][1]-vertex[0][1];
						pCS->tz=vertex[1][2]-vertex[0][2];
					}
				}
			}
		}

		if (m_workMode==4)//旋转模式
		{
			if (m_BoxChoose)//开启框选
			{
				m_drawChooseBox=1;//绘制选择框
			}
			if (m_move)//进入旋转
			{
				m_drawRotateCross=1;//绘制旋转十字架
				GLint dx=point.x-m_pp[0][0],dy=point.y-m_pp[0][1];
				GLdouble angle,rotateAxis[3];
				bool axis=0;//0表示沿x轴，1表示沿y轴
				if (abs(dx)>abs(dy)) //沿y轴旋转
				{	
					angle=dx;
					axis=1;
				}
				else //沿x轴旋转
				{
					angle=-dy;
					axis=0;
				}
				switch(m_activeView+3*m_viewSide[m_activeView])
				{
				case 0: //3D
					{
						double ks1=sin(m_theta*PI/180.0),ks2=sin(m_phi*PI/180.0),kc1=cos(m_theta*PI/180.0),kc2=cos(m_phi*PI/180.0);
						if(axis) 
						{
							rotateAxis[0]=-kc1*kc2;
							rotateAxis[1]=-kc1*ks2;
							rotateAxis[2]=ks1;
						}
						else
						{
							rotateAxis[0]=-ks2;
							rotateAxis[1]=kc2;
							rotateAxis[2]=0;
						}
						break;
					}
				case 1: //top
					if(axis) 
					{
						rotateAxis[0]=-1;
						rotateAxis[1]=0;
						rotateAxis[2]=0;
					}
					else
					{
						rotateAxis[0]=0;
						rotateAxis[1]=1;
						rotateAxis[2]=0;
					}
					break;
				case 2: //front
					if(axis) 
					{
						rotateAxis[0]=0;
						rotateAxis[1]=0;
						rotateAxis[2]=1;
					}
					else
					{
						rotateAxis[0]=0;
						rotateAxis[1]=1;
						rotateAxis[2]=0;
					}
					break;
				case 3: //left
					if(axis) 
					{
						rotateAxis[0]=0;
						rotateAxis[1]=0;
						rotateAxis[2]=1;
					}
					else
					{
						rotateAxis[0]=1;
						rotateAxis[1]=0;
						rotateAxis[2]=0;
					}
					break;
				case 4: //bottom
					if(axis) 
					{
						rotateAxis[0]=-1;
						rotateAxis[1]=0;
						rotateAxis[2]=0;
					}
					else
					{
						rotateAxis[0]=0;
						rotateAxis[1]=-1;
						rotateAxis[2]=0;
					}
					break;
				case 5: //back
					if(axis) 
					{
						rotateAxis[0]=0;
						rotateAxis[1]=0;
						rotateAxis[2]=1;
					}
					else
					{
						rotateAxis[0]=0;
						rotateAxis[1]=-1;
						rotateAxis[2]=0;
					}
					break;
				case 6: //right
					if(axis) 
					{
						rotateAxis[0]=0;
						rotateAxis[1]=0;
						rotateAxis[2]=1;
					}
					else
					{
						rotateAxis[0]=-1;
						rotateAxis[1]=0;
						rotateAxis[2]=0;
					}
					break;
				default:
					break;
				}
				//所有的选中对象都进行旋转
				pCS=m_cs_head;
				while (pCS->next)
				{
					pCS=pCS->next;
					if (pCS->m_selected&&pCS->m_free)
					{
						pCS->m_rotateAxis[0]=rotateAxis[0];
						pCS->m_rotateAxis[1]=rotateAxis[1];
						pCS->m_rotateAxis[2]=rotateAxis[2];
						pCS->m_angle=angle;
					}
				}
				
			}
		}
		redraw=1;//必然重绘
	}	
	
	if (m_workMode==5)
	{
		if (m_drag==1.1||m_drag==1.2||m_drag==1.3)
		{
			if(m_drag==1.2) m_drag=1.3;
			GLdouble vertex[2][3];//临时顶点
			GLdouble matrix[16];
			int aim;
			if (m_num_view==4) aim=m_activeView;
			else aim=4;
			//获取视图模型矩阵
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glMultMatrixd(m_modelMatrix[m_activeView]);//加载视角
			//加载移动？
			glMultMatrixd(pActiveCS->m_mvMatrix);//加载旋转
			glScaled(pActiveCS->m_size,pActiveCS->m_size,pActiveCS->m_size);//加载放缩
			glGetDoublev(GL_MODELVIEW_MATRIX,matrix);
			glPopMatrix();
			gluUnProject((GLdouble)m_lastMovePP[0],(GLdouble)m_lastMovePP[1],0,matrix,
				m_projMatrix[aim],m_port[aim],&vertex[0][0],&vertex[0][1],&vertex[0][2]);
			gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,matrix,
				m_projMatrix[aim],m_port[aim],&vertex[1][0],&vertex[1][1],&vertex[1][2]);
			m_lastMovePP[0]=point.x;
			m_lastMovePP[1]=point.y;
			//遍历所有选中点，改变其坐标
			TriangleRef *pt;
			for (GLuint i=0;i<m_numPointHits;i++)
			{
				m_pointHits[i]->v[0]+=vertex[1][0]-vertex[0][0];
				m_pointHits[i]->v[1]+=vertex[1][1]-vertex[0][1];
				m_pointHits[i]->v[2]+=vertex[1][2]-vertex[0][2];
				pt=m_pointHits[i]->nextRef;
				while (pt)
				{
					pt->p->v[0]=m_pointHits[i]->v[0];
					pt->p->v[1]=m_pointHits[i]->v[1];
					pt->p->v[2]=m_pointHits[i]->v[2];
					pt=pt->nextRef;
				}
			}
			redraw=1;			
		}
		if (m_cursorBox&&m_drag<2) redraw=1;//点选时刻绘制光标
		if (m_drag==2.1||m_drag==2.11) //绘制选择框
		{
			if(m_drag==2.1)
			{
				m_drag=2.11;//指示发生了移动
				m_drawChooseBox=1;
			}
			redraw=1;
		}
		if (m_drag==2.2||m_drag==2.21) //拖动
		{
			if(m_drag==2.2) m_drag=2.21;//指示发生了移动
			GLdouble vertex[2][3];
			int aim;
			if (m_num_view==4) aim=m_activeView;
			else aim=4;
			gluUnProject((GLdouble)m_lastMovePP[0],(GLdouble)m_lastMovePP[1],0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&vertex[0][0],&vertex[0][1],&vertex[0][2]);
			gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&vertex[1][0],&vertex[1][1],&vertex[1][2]);
			m_lastMovePP[0]=point.x;
			m_lastMovePP[1]=point.y;
			//遍历所有选中点，改变其坐标
			TriangleRef *pt;
			for (GLuint i=0;i<m_numPointHits;i++)
			{
				m_pointHits[i]->v[0]+=vertex[1][0]-vertex[0][0];
				m_pointHits[i]->v[1]+=vertex[1][1]-vertex[0][1];
				m_pointHits[i]->v[2]+=vertex[1][2]-vertex[0][2];
				pt=m_pointHits[i]->nextRef;
				while (pt)
				{
					pt->p->v[0]=m_pointHits[i]->v[0];
					pt->p->v[1]=m_pointHits[i]->v[1];
					pt->p->v[2]=m_pointHits[i]->v[2];
					pt=pt->nextRef;
				}
			}
			redraw=1;
		}
	}
	/*******************************************************************/
	if (m_workMode==6)
	{
		if (m_knife.state==1.1||m_knife.state==1.2)
		{
			if(m_knife.state==1.1) m_knife.state=1.2;//表示进行了移动
			GLdouble vertex[2][3];
			int aim;
			if (m_num_view==4) aim=m_activeView;
			else aim=4;
			gluUnProject((GLdouble)m_pp[0][0],(GLdouble)m_pp[0][1],0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&vertex[0][0],&vertex[0][1],&vertex[0][2]);
			gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&vertex[1][0],&vertex[1][1],&vertex[1][2]);
			m_knife.tx=vertex[1][0]-vertex[0][0];
			m_knife.ty=vertex[1][1]-vertex[0][1];
			m_knife.tz=vertex[1][2]-vertex[0][2];
			redraw=1;
		}
		if (m_knife.state==2.1||m_knife.state==2.2)
		{
			if(m_knife.state==2.1) m_knife.state=2.2;//表示进行了旋转
			m_drawRotateCross=1;//绘制旋转十字架
			GLint dx=point.x-m_pp[0][0],dy=point.y-m_pp[0][1];
			GLdouble angle,rotateAxis[3];
			bool axis=0;//0表示沿x轴，1表示沿y轴
			if (abs(dx)>abs(dy)) //沿y轴旋转
			{	
				angle=dx;
				axis=1;
			}
			else //沿x轴旋转
			{
				angle=-dy;
				axis=0;
			}
			switch(m_activeView+3*m_viewSide[m_activeView])
			{
			case 0: //3D
				{
					double ks1=sin(m_theta*PI/180.0),ks2=sin(m_phi*PI/180.0),kc1=cos(m_theta*PI/180.0),kc2=cos(m_phi*PI/180.0);
					if(axis) 
					{
						rotateAxis[0]=-kc1*kc2;
						rotateAxis[1]=-kc1*ks2;
						rotateAxis[2]=ks1;
					}
					else
					{
						rotateAxis[0]=-ks2;
						rotateAxis[1]=kc2;
						rotateAxis[2]=0;
					}
					break;
				}
			case 1: //top
				if(axis) 
				{
					rotateAxis[0]=-1;
					rotateAxis[1]=0;
					rotateAxis[2]=0;
				}
				else
				{
					rotateAxis[0]=0;
					rotateAxis[1]=1;
					rotateAxis[2]=0;
				}
				break;
			case 2: //front
				if(axis) 
				{
					rotateAxis[0]=0;
					rotateAxis[1]=0;
					rotateAxis[2]=1;
				}
				else
				{
					rotateAxis[0]=0;
					rotateAxis[1]=1;
					rotateAxis[2]=0;
				}
				break;
			case 3: //left
				if(axis) 
				{
					rotateAxis[0]=0;
					rotateAxis[1]=0;
					rotateAxis[2]=1;
				}
				else
				{
					rotateAxis[0]=1;
					rotateAxis[1]=0;
					rotateAxis[2]=0;
				}
				break;
			case 4: //bottom
				if(axis) 
				{
					rotateAxis[0]=-1;
					rotateAxis[1]=0;
					rotateAxis[2]=0;
				}
				else
				{
					rotateAxis[0]=0;
					rotateAxis[1]=-1;
					rotateAxis[2]=0;
				}
				break;
			case 5: //back
				if(axis) 
				{
					rotateAxis[0]=0;
					rotateAxis[1]=0;
					rotateAxis[2]=1;
				}
				else
				{
					rotateAxis[0]=0;
					rotateAxis[1]=-1;
					rotateAxis[2]=0;
				}
				break;
			case 6: //right
				if(axis) 
				{
					rotateAxis[0]=0;
					rotateAxis[1]=0;
					rotateAxis[2]=1;
				}
				else
				{
					rotateAxis[0]=-1;
					rotateAxis[1]=0;
					rotateAxis[2]=0;
				}
				break;
			default:
				break;
			}
			//所有的选中对象都进行旋转
			m_knife.axis[0]=rotateAxis[0];
			m_knife.axis[1]=rotateAxis[1];
			m_knife.axis[2]=rotateAxis[2];
			m_knife.angle=angle;
			redraw=1;
		}
	}
	if (m_workMode==7.1)//选择性加密模式
	{
		if (m_chooseDensify==2)
		{
			m_drawChooseBox=1;//绘制选择框
			redraw=1;
		}
		if (m_cursorBox) redraw=1;
	}

	if (redraw) renderScene();
}

BOOL CMMDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) //鼠标滚轮响应
{
	if (m_workMode==3)//选中物体放缩模式
	{
		CS *pCS=m_cs_head;
		while (pCS->next)
		{
			pCS=pCS->next;
			if (pCS->m_selected&&pCS->m_free)
			{
				if (zDelta>0) pCS->m_size+=0.1;
				else pCS->m_size-=0.1;
			}
		}
	}
	else if(m_workMode==6&&m_knife.mouseWheelSize)
	{
		if (zDelta>0) m_knife.size+=0.1;
		else m_knife.size-=0.1;
	}
	else
	{
		if (zDelta>0) m_size[m_activeView]+=0.1;
		else m_size[m_activeView]-=0.1;
		//更新视图矩阵
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		lookAt(m_activeView+3*m_viewSide[m_activeView]);
		glScalef(50,50,50);
		glScalef(m_size[m_activeView],m_size[m_activeView],m_size[m_activeView]);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix[m_activeView]);
	}
	renderScene();
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CMMDlg::OnLButtonDblClk(UINT nFlags, CPoint point) //鼠标双击时的响应
{
	CDialog::OnLButtonDblClk(nFlags, point);

	toGLCoordinate(&point);
	bool needRenderScene=0;
	if (m_num_view==4) 
	{	
		if (m_view_style==1)//3大1小视图风格
		{
			for (short i=0;i<4;i++)
			{
				if(rc[i].right-AREA<point.x&&point.x<rc[i].right&&rc[i].top-AREA<point.y&&point.y<rc[i].top)
				{
					if(m_viewport==i) 
					{
						m_num_view=1;//进入单视图
						//m_update[i]=1;
					}
					else //与view交换视图
					{
						if(m_viewport!=0||i==0) exchangeRect(rc[0],rc[m_viewport]);
						exchangeRect(rc[0],rc[i]);
						m_viewport=i;
						//m_update[0]=m_update[1]=m_update[2]=m_update[3]=1;//四个窗口中点的平面坐标都要更新
					}
					needRenderScene=1;
					break;
				}
			}
			//更新4个小视图的视口矩阵和投影矩阵
			for(i=0;i<4;i++)
			{
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				float ratio=(float)(rc[i].right-rc[i].left)/(rc[i].top-rc[i].bottom);
				if(ratio>1)	glOrtho(-Lw*ratio,Lw*ratio,-Lw,Lw,-10*Lw,10*Lw);
				else glOrtho(-Lw,Lw,-Lw/ratio,Lw/ratio,-10*Lw,10*Lw);
				glViewport(rc[i].left,rc[i].bottom,rc[i].right-rc[i].left,rc[i].top-rc[i].bottom);
				glGetIntegerv(GL_VIEWPORT, m_port[i]);
				glGetDoublev(GL_PROJECTION_MATRIX, m_projMatrix[i]);
			}
		}
		if (m_view_style==0)//4均匀视图风格
		{
			for (short i=0;i<4;i++)
			{
				if(rc[i].right-AREA<point.x&&point.x<rc[i].right&&rc[i].top-AREA<point.y&&point.y<rc[i].top)
				{
					m_num_view=1;
					m_viewport=i;
					//m_update[i]=1;
					needRenderScene=1;
				}
			}
		}		
	}
	else
	{	//在单视图时双击，进入4视图
		int length=rcALL.Width(),width=rcALL.Height();
		if(length-40<=point.x&&point.x<=length&&width-30<=point.y&&point.y<=width)
		{
			m_num_view=4;
			//m_update[m_viewport]=1;
			needRenderScene=1;
		}
	}
	if(needRenderScene) renderScene();
}

void CMMDlg::toGLRect(CRect *rc)
{
	rc->left-=rcALL.left;
	rc->right-=rcALL.left;
	rc->top=rcALL.bottom-rc->top;
	rc->bottom=rcALL.bottom-rc->bottom;
}

void CMMDlg::toGLCoordinate(CPoint *point)//将屏幕坐标转化为绘图区openGL视口坐标
{
	point->x-=rcALL.left;
	point->y=rcALL.bottom-point->y;	
}

void CMMDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonUp(nFlags, point);	
	toGLCoordinate(&point);
	int needRenderScene=0;
	short ctrlState=GetKeyState(VK_CONTROL);//获取此时control键是否按下
	CS *pCS;

	if (m_workMode==0)//创建模式
	{
		if (m_create==1.2)//创建长方体时等待第一次左键弹起
		{		
			if(point.x!=m_pp[0][0]&&point.y!=m_pp[0][1])
			{
				m_create=1.3;
			}
			else 
			{
				m_create=1;
				needRenderScene=1;
			}
		}
		if (m_create==2.12)//创建柱体
		{
			if(point.x!=m_pp[0][0]||point.y!=m_pp[0][1])
			{
				m_create=2.13;
			}
			else 
			{
				m_create=2;
				needRenderScene=1;
			}
		}

		if (m_create==3.2)//创建球体
		{
			if(point.x!=m_pp[0][0]||point.y!=m_pp[0][1])
			{
				//到此获取了球体的信息
				m_sphere.cx=m_vertex[0][0];
				m_sphere.cy=m_vertex[0][1];
				m_sphere.cz=0;
				m_sphere.R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
				createSphere();			
			}	
			m_create=3;
			needRenderScene=1;
		}
		if (m_create==4.12)//创建锥体
		{
			if(point.x!=m_pp[0][0]||point.y!=m_pp[0][1])
			{
				m_create=4.13;
			}
			else 
			{
				m_create=4;
				needRenderScene=1;
			}
		}
		if (m_create==5.2)//创建锥体
		{
			if(point.x!=m_pp[0][0]||point.y!=m_pp[0][1])
			{
				m_create=5.3;
			}
			else 
			{
				m_create=5;
				needRenderScene=1;
			}
		}
		if (m_create==6.2)//创建pipe
		{
			if(point.x!=m_pp[0][0]||point.y!=m_pp[0][1])
			{
				m_create=6.3;
			}
			else 
			{
				m_create=6;
				needRenderScene=1;
			}
		}
		if (m_create==6.5)//创建pipe
		{
			m_create=6.6;
		}
		
	}
	/***********************************************************/
	if (m_workMode==5)
	{
		if (m_drag==1.1||m_drag==1.3)
		{
			m_drag=1;
			needRenderScene=1;
		}
		if (m_drag==1.2)
		{
			m_drag=1;
			needRenderScene=1;
			if (ctrlState>=0)//没有按下ctrl，依旧代新
			{
				for (GLuint i=0;i<m_numPointHits;i++) m_pointHits[i]->selected=0;//取消先前的选中态
				m_numPointHits=m_numBuffHits;//更新选中点数
				for (i=0;i<m_numBuffHits;i++)
				{
					m_pointHits[i]=m_pointsBuffer[i];//复制指针数据
					m_pointHits[i]->selected=1;
				}
			}
			else //按下了ctrl，重复的会被清除
			{
				GLuint i=0,j=0;
				for (i=0;i<m_numBuffHits;i++)
				{
					for (j=0;j<m_numPointHits;j++)
					{
						if(m_pointHits[j]==m_pointsBuffer[i]) 
						{
							break;
						}
					}
					if(j<m_numPointHits) //有重叠
					{
						m_pointHits[j]->selected=0;//清除重叠点的选中
						if(j!=m_numPointHits) m_pointHits[j]=m_pointHits[m_numPointHits-1];//不是最后一个点，那么将最后一点的指针移到刚才清空处
						m_numPointHits--;//点数会减少
					}
					else //没有重叠，插入到尾部
					{
						m_pointHits[m_numPointHits]=m_pointsBuffer[i];
						m_numPointHits++;//点数增加
					}
				}
			}

		}
		if(m_drag==2.1) //没点中也没有移动,清除任何已选中的点
		{
			if(ctrlState>=0&&m_numPointHits!=0)//没有按下ctrl，清除以前选中的
			{	
				for (GLuint i=0;i<m_numPointHits;i++)
				{
					m_pointHits[i]->selected=0;
				}
				m_numPointHits=0;
				needRenderScene=1;				
			}
			m_drag=2;
		}
		if(m_drag==2.11&&point.x!=m_pp[0][0]&&point.y!=m_pp[0][1])//发生框选拖动,进行框选
		{
			int cx=(point.x+m_pp[0][0])/2,cy=(point.y+m_pp[0][1])/2;
			int w=abs(point.x-m_pp[0][0]),h=abs(point.y-m_pp[0][1]);
			GLuint hits=pickPoints(cx,cy,w,h);
			if (hits>0)
			{
				if (ctrlState>=0)//没有按下ctrl，依旧代新
				{
					for (GLuint i=0;i<m_numPointHits;i++) m_pointHits[i]->selected=0;//取消先前的选中态
					m_numPointHits=hits;//更新选中点数
					for (i=0;i<hits;i++)
					{
						m_pointHits[i]=m_pointsBuffer[i];//复制指针数据
						m_pointHits[i]->selected=1;
					}
				}
				else //按下了ctrl，重复的会被清除
				{
					GLuint i=0,j=0;
					for (i=0;i<hits;i++)
					{
						for (j=0;j<hits;j++)
						{
							if(m_pointHits[j]==m_pointsBuffer[i]) 
							{
								break;
							}
						}
						if(j<m_numPointHits) //有重叠
						{
							m_pointHits[j]->selected=0;//清除重叠点的选中
							if(j!=m_numPointHits) m_pointHits[j]=m_pointHits[m_numPointHits-1];//不是最后一个点，那么将最后一点的指针移到刚才清空处
							m_numPointHits--;//点数会减少
						}
						else //没有重叠，插入到尾部
						{
							m_pointHits[m_numPointHits]=m_pointsBuffer[i];
							m_numPointHits++;//点数增加
						}
					}
				}
			}
			else //清除所有以前选中的
			if(ctrlState>=0&&m_numPointHits!=0)//没有按下ctrl，清除以前选中的
			{	
				for (GLuint i=0;i<m_numPointHits;i++)
				{
					m_pointHits[i]->selected=0;
				}
				m_numPointHits=0;
				needRenderScene=1;				
			}			
			m_drag=2;
			needRenderScene=1;
		}
		if (m_drag==2.2)//点到了但是没有拖动
		{
			m_drag=2;
		}
		if (m_drag==2.21) //点到了且完成了拖动
		{
			m_drag=2;			
		}

		m_drawChooseBox=0;//关闭选择框绘图
	}

	/***************************************************************/
	if (m_LBdown)//必须在有效左键按下时执行
	{
		if (m_workMode==1||m_workMode==3)//选择模式，或缩放模式
		{
			GLuint hits;
			
			if (point.x==m_pp[0][0]&&point.y==m_pp[0][1])//没有移动，点选
			{
				hits=pickObjects(point.x-1,point.y-1,point.x+1,point.y+1);
				if (hits==0) //未选中
				{
					if (ctrlState>=0)//没有按下control,所有物体不选中
					{
						pCS=m_cs_head;
						while (pCS->next)
						{
							pCS=pCS->next;
							if (pCS->m_selected)
							{
								pCS->m_selected=0;
								needRenderScene=1;//改变选择，需重绘
							}
						}
					}
				}
				else //有物体选中
				{
					if (ctrlState<0)//多选，只考虑最近的物体
					{
						pCS=m_cs_head;
						while (pCS->next)
						{
							pCS=pCS->next;
							if (pCS->m_index==m_hits[0])
							{
								pCS->m_selected=!pCS->m_selected;
								needRenderScene=1;//改变选择，需重绘
							}
						}
					}
					else //单选,使更远的物体被选中
					{
						GLuint sel,i;
						if (hits==1) sel=m_hits[0];
						else //同时不只一个物体选中
						{
							//由近向远扫描，找到最远的选中者
							for (i=hits;i>0;i--)
							{
								if (m_hits[2*i-1]) break;
							}
							if (i==hits||i==0) sel=m_hits[0];//最远的选中,或均未选中，返回最近的
							else sel=m_hits[2*i];
						}
						//这样得到了要选的项
						pCS=m_cs_head;
						while (pCS->next)
						{
							pCS=pCS->next;
							if (pCS->m_index==sel)
							{
								if(pCS->m_selected!=1)
								{
									pCS->m_selected=1;
									needRenderScene=1;
								}
							}
							else
							{
								if(pCS->m_selected==1)
								{
									pCS->m_selected=0;
									needRenderScene=1;
								}
							}
						}
					}
				}
			}
			else //有移动，框选
			{
				needRenderScene=1;
				if(point.x!=m_pp[0][0]&&point.y!=m_pp[0][1])
				{
					hits=pickObjects(m_pp[0][0],m_pp[0][1],point.x,point.y);
					if (hits==0)
					{
						pCS=m_cs_head;
						while (pCS->next)
						{
							pCS=pCS->next;
							if (pCS->m_selected)
							{
								pCS->m_selected=0;
								needRenderScene=1;//改变选择，需重绘
							}
						}
					}
					else //有选中
					{
						GLuint i;
						pCS=m_cs_head;
						while (pCS->next)
						{
							pCS=pCS->next;
							for (i=0;i<hits;i++)
							{
								if (pCS->m_index==m_hits[2*i])
								{	
									if (pCS->m_selected==0)//尚未被选中
									{
										pCS->m_selected=1;
										needRenderScene=1;//改变选择，需重绘
									}	
									break;			
								}
							}
							if(i==hits&&pCS->m_selected)//没有符合的，使其不选中
							{
								pCS->m_selected=0;
								needRenderScene=1;
							}				
						}
					}
				}
			}
		}
		/************************************************************************************/
		if (m_workMode==2||m_workMode==4) //移动模式,或旋转模式
		{
			if (point.x==m_pp[0][0]&&point.y==m_pp[0][1])//没有移动
			{
				if (m_BoxChoose)//没有点到任何物体，取消框选，所有物体均不选中
				{
					pCS=m_cs_head;
					while (pCS->next) 
					{
						pCS=pCS->next;
						if (pCS->m_selected)
						{
							pCS->m_selected=0;
							needRenderScene=1;
						}
					}
				}
				////////////////////////////////////
				if (m_move>1)//调整重叠选择
				{
					GLuint sel,i,hits=m_move-1;
					for (i=hits;i>0;i--)
					{
						if (m_hits[2*i-1]) break;
					}
					if (i==hits||i==0) sel=m_hits[0];//最远的选中,或均未选中，返回最近的
					else sel=m_hits[2*i];

					pCS=m_cs_head;
					while (pCS->next)
					{
						pCS=pCS->next;
						if (pCS->m_index==sel)
						{
							if(pCS->m_selected!=1)
							{
								pCS->m_selected=1;
								needRenderScene=1;
							}
						}
						else
						{
							if(pCS->m_selected==1)
							{
								pCS->m_selected=0;
								needRenderScene=1;
							}
						}
					}
				}
			}
			else//移动了
			{
				if (m_BoxChoose)//进行框选
				{
					needRenderScene=1;
					if(point.x!=m_pp[0][0]&&point.y!=m_pp[0][1])
					{
						GLuint hits=pickObjects(m_pp[0][0],m_pp[0][1],point.x,point.y);
						if (hits==0) //全部未选中
						{
							pCS=m_cs_head;
							while (pCS->next)
							{
								pCS=pCS->next;
								if (pCS->m_selected)
								{
									pCS->m_selected=0;
									needRenderScene=1;//改变选择，需重绘
								}
							}
						}
						else //有选中
						{
							GLuint i;
							pCS=m_cs_head;
							while (pCS->next)
							{
								pCS=pCS->next;
								for (i=0;i<hits;i++)
								{
									if (pCS->m_index==m_hits[2*i])
									{	
										if (pCS->m_selected==0)//尚未被选中
										{
											pCS->m_selected=1;
											needRenderScene=1;//改变选择，需重绘
										}	
										break;			
									}
								}
								if(i==hits&&pCS->m_selected)//没有符合的，使其不选中
								{
									pCS->m_selected=0;
									needRenderScene=1;
								}				
							}
						}
					}
				}
				if (m_move)//要记录移动
				{
					pCS=m_cs_head;
					while (pCS->next) //所有选中物体储存模型视图矩阵
					{
						pCS=pCS->next;
						if (pCS->m_selected) pCS->storeMVMatrix();		
					}
				}
			}
			if (m_drawRotateCross)//不绘制旋转十字架
			{
				m_drawRotateCross=0;
				needRenderScene=1;
			}
			m_move=0;//关闭移动标识
		}
		
		checkSelected();
		m_LBdown=0;
		m_BoxChoose=0;//关闭选择标识
		m_drawChooseBox=0;//不绘制选择框
	}
	
	/**************************************************************************/
	if (m_workMode==6)
	{
		if(point.x!=m_pp[0][0]||point.y!=m_pp[0][1]) m_knife.storeMatrix();
		
		if (m_knife.state==1.1||m_knife.state==1.2)	
		{
			m_knife.state=1;
		}
		
		if (m_knife.state==2.1||m_knife.state==2.2)
		{
			m_knife.state=2;
			m_drawRotateCross=0;
		}
		needRenderScene=1;
	}
	/**************************************************************************/
	if(m_workMode==7.1)
	{
		if(point.x!=m_pp[0][0]&&point.y!=m_pp[0][1]&&m_chooseDensify==2)//有效移动
		{
			//进行框选
			GLuint hits=pickTriangles((point.x+m_pp[0][0])/2,(point.y+m_pp[0][1])/2,abs(point.x-m_pp[0][0]),abs(point.y-m_pp[0][1]));
		}
		m_drawChooseBox=0;//关闭选择框
		needRenderScene=1;
		m_chooseDensify=1;
	}

	m_pp[1][0]=point.x;
	m_pp[1][1]=point.y;//记录第二平面点
	if(needRenderScene==1) renderScene();
}

///////////////////////界面响应函数////////////////////////////
void CMMDlg::OnUndo() 
{	
	pickPoints(800,400,200,200);//选点
}

void CMMDlg::OnRedo() 
{
	
}

void CMMDlg::OnSelect() //进入选择模式
{
	m_workMode=1;
	renderScene();
	m_page2.unCheck();
}

void CMMDlg::OnSelectMove() //移动模型
{
	m_workMode=2;
	renderScene();
	m_page2.unCheck();
}

void CMMDlg::OnSelectSize() //选择并均匀缩放
{
	m_workMode=3;
	renderScene();
	m_page2.unCheck();
}

void CMMDlg::OnRotate() //选择并旋转物体
{
	m_workMode=4;
	renderScene();
	m_page2.unCheck();
}

void CMMDlg::OnNewfile() //新建场景，可能要保存旧场景
{
	CS *pCS=m_cs_head,*q;
	if (m_num_model)
	{
		int ans=MessageBox("模型文件已经过修改，是否保存?\nyes：保存\nno:不保存\ncancel:放弃新建","保存提示:",MB_YESNOCANCEL);
		if(ans==IDCANCEL) return;
		else if(ans==IDYES) //保存旧场景
		{
			
		}
	}
	//删除旧场景
	pCS=m_cs_head->next;
	m_cs_head->next=NULL;
	while (pCS)
	{
		q=pCS;
		pCS=pCS->next;
		delete q;
	}
	m_num_model=0;
	renderScene();
}
///////////////////////////////////////////////////////////////

void CMMDlg::GLTextOut(float x,float y,float z,char * const textstring) //输出字符串
{
	glRasterPos3f(x,y,z);
	glListBase(m_id);
	glCallLists(strlen( textstring ), GL_UNSIGNED_BYTE, (const GLvoid*)textstring ); 
}

void CMMDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);	
	toGLCoordinate(&point);	
	short i=0;
	bool needRenderScene=0;
	short ctrlState=GetKeyState(VK_CONTROL);//获取此时control键是否按下
	/*****************************************************************/
	if (m_num_view==4)//四视图时切换活动区域
	{
		for (i=0;i<4;i++)
		{
			if(inRect(rc[i],point)) //落在第i区域内
			{
				//如果点击左上角文字，切换渲染方式
				if (rc[i].left<point.x&&point.x<rc[i].left+AREA&&rc[i].top-AREA<point.y&&point.y<rc[i].top)
				{
					m_viewManner[i]=!m_viewManner[i];
					needRenderScene=1;
				}
				//假如被点的区域不是活动的，将其变为活动的
				if(m_activeView!=i) 
				{
					m_activeView=i;
					needRenderScene=1;
				}				
				break;
			}
		}
	}
	else //单视图
	//如果点击左上角文字，切换渲染方式
	if (0<point.x&&point.x<AREA&&rcALL.Height()-AREA<point.y&&point.y<rcALL.Height())
	{
		m_viewManner[m_activeView]=!m_viewManner[m_activeView];
		needRenderScene=1;
	}
	/************************以下与工作模式有关*******************************/
	if (inDrawArea(point))
	{
		
		//点击到绘图区，先保存名字更新	
		if (m_page1.GetDlgItem(IDC_NAME)->IsWindowEnabled()!=0) updateName();	
		CS *pCS;
		if (m_workMode==0)//创建模式
		{
			byte aim;
			if (m_num_view==4) aim=m_activeView;
			else aim=4;
			if (m_create==1)//创建长方体时等待第一次左键按下
			{
				unSelectALL();
				//获取各边分段
				m_cuboid.nx=10-m_page1.m_cuboidPanel.m_nx.GetTopIndex();
				m_cuboid.ny=10-m_page1.m_cuboidPanel.m_ny.GetTopIndex();
				m_cuboid.nz=10-m_page1.m_cuboidPanel.m_nz.GetTopIndex();
				if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//在3D窗口创建，求得该点的3D坐标
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
						   ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
						   az=cos(m_theta*PI/180.0);
					m_vertex[0][0]=m_vertex[0][0]-ax/az*m_vertex[0][2];
					m_vertex[0][1]=m_vertex[0][1]-ay/az*m_vertex[0][2];
					m_vertex[0][2]=0;
				}
				if (m_activeView==1)//在视口1创建
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					m_vertex[0][2]=0;
				}
				if (m_activeView==2)//在视口2创建
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					m_vertex[0][0]=0;
				}
				if (m_activeView==3)//在视口3创建
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					m_vertex[0][1]=0;
				}
				m_create=1.1;//进入移动，但尚未移动
			}
			if (m_create==1.4)//创建长方体时等待第二次左键按下
			{
				if (point.y!=m_pp[1][1])
				{
					//获取中心坐标和边长
					if (m_activeView==0||m_activeView==1)
					{
						m_cuboid.cx=(m_vertex[0][0]+m_vertex[1][0])/2;
						m_cuboid.cy=(m_vertex[0][1]+m_vertex[1][1])/2;
						m_cuboid.cz=m_vertex[2][2]/2;
						m_cuboid.lx=fabs(m_vertex[0][0]-m_vertex[1][0]);
						m_cuboid.ly=fabs(m_vertex[0][1]-m_vertex[1][1]);
						m_cuboid.lz=fabs(m_vertex[2][2]);
					}
					if (m_activeView==2)
					{
						m_cuboid.cx=m_vertex[2][0]/2;
						m_cuboid.cz=(m_vertex[0][2]+m_vertex[1][2])/2;
						m_cuboid.cy=(m_vertex[0][1]+m_vertex[1][1])/2;	
						m_cuboid.lx=fabs(m_vertex[2][0]);
						m_cuboid.lz=fabs(m_vertex[0][2]-m_vertex[1][2]);
						m_cuboid.ly=fabs(m_vertex[0][1]-m_vertex[1][1]);
						
					}
					if (m_activeView==3)
					{
						m_cuboid.cx=(m_vertex[0][0]+m_vertex[1][0])/2;
						m_cuboid.cy=m_vertex[2][1]/2;
						m_cuboid.cz=(m_vertex[0][2]+m_vertex[1][2])/2;
						m_cuboid.lx=fabs(m_vertex[0][0]-m_vertex[1][0]);
						m_cuboid.ly=fabs(m_vertex[2][1]);
						m_cuboid.lz=fabs(m_vertex[0][2]-m_vertex[1][2]);
					}
					
					createCuboid();

				}
				m_create=1;
				needRenderScene=1;				
			}

			if (m_create==2)//创建柱体
			{
				//读取设置
				m_cylinder.n=ND_MAX-m_page1.m_cylinderPanel.m_DN.GetTopIndex();
				if(((CButton *)m_page1.m_cylinderPanel.GetDlgItem(IDC_REGULAR_POLYGON))->GetCheck()==1)
					m_cylinder.type=0;
				else m_cylinder.type=1;
				m_cylinder.nr=20-m_page1.m_cylinderPanel.m_NR.GetTopIndex();
				m_cylinder.nh=20-m_page1.m_cylinderPanel.m_NH.GetTopIndex();//获得分段数
				unSelectALL();
				if (m_cylinder.type==0)//正底面是正多边形，获取中心点
				{					
					if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//在3D窗口创建，求得该点的3D坐标
					{
						gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
							m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
						double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
							ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
							az=cos(m_theta*PI/180.0);
						m_vertex[0][0]=m_vertex[0][0]-ax/az*m_vertex[0][2];
						m_vertex[0][1]=m_vertex[0][1]-ay/az*m_vertex[0][2];
						m_vertex[0][2]=0;
					}
					m_create=2.11;//指示进入移动状态
				}
				else //底边是任意多边形
				{
					if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//在3D窗口创建，求得该点的3D坐标
					{
						gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
							m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
						double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
							ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
							az=cos(m_theta*PI/180.0);
						//获取初始点
						m_vertex[0][0]=m_vertex[0][0]-ax/az*m_vertex[0][2];
						m_vertex[0][1]=m_vertex[0][1]-ay/az*m_vertex[0][2];
						m_vertex[0][2]=0;
						//创建第一条边					
						m_cylinder.ehead=new Edge;
						Edge *p=new Edge;
						m_cylinder.ehead->next=p;
						m_cylinder.end=p;//移动时只会修改尾部，所以很有用
						m_cylinder.numEdge=0;//边数为0
						p->x1=p->x2=m_vertex[0][0];
						p->y1=p->y2=m_vertex[0][1];
						p->z1=0;
						p->next=NULL;
					}
					m_create=2.2;//指示进入点画边的状态,LBdown创建线段头
				}
			}
			else if (m_create==2.14)
			{
				if (point.y!=m_pp[1][1]) //数据采集完成
				{
					//获取中心坐标和R,H
					if (m_activeView==0||m_activeView==1)
					{
						m_cylinder.cx=m_vertex[0][0];
						m_cylinder.cy=m_vertex[0][1];
						m_cylinder.cz=0;
						m_cylinder.R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
						m_cylinder.axis[0]=1;
						m_cylinder.axis[1]=0;
						m_cylinder.axis[2]=0;
						if(m_vertex[2][2]>0) 
						{
							m_cylinder.angle=0;
							m_cylinder.h=m_vertex[2][2];
						}
						else
						{
							m_cylinder.angle=180;
							m_cylinder.h=-m_vertex[2][2];
						}		
					}					
					createCylinder();					
				}
				m_create=2;
				needRenderScene=1;	
			}			
			else if (m_create==2.2&&(point.x!=m_pp[0][0]||point.y!=m_pp[0][1]))
			{				
				if (m_cylinder.numEdge==0)//第一条边，不检查
				{
					Edge *p;
					m_cylinder.end->next=p=new Edge;//新建节点
					p->next=NULL;
					m_cylinder.numEdge++;//新建一条边
					p->x1=m_cylinder.end->x2;
					p->y1=m_cylinder.end->y2;
					p->z1=0;
					p->x2=m_cylinder.end->x2;
					p->y2=m_cylinder.end->y2;
					p->z2=0;
					m_cylinder.end=p;//指向链尾
				}
				else if(m_cylinder.numEdge==1) //第二条边，只检查末点是否在第一条边上
				{
					byte r=m_p2t.pointOnEdge(m_cylinder.ehead->next,m_cylinder.end);
					if(r==0)
					{
						Edge *p;
						m_cylinder.end->next=p=new Edge;//新建节点
						p->next=NULL;
						m_cylinder.numEdge++;//新建一条边
						p->x1=m_cylinder.end->x2;
						p->y1=m_cylinder.end->y2;
						p->z1=0;
						p->x2=m_cylinder.end->x2;
						p->y2=m_cylinder.end->y2;
						p->z2=0;
						m_cylinder.end=p;//指向链尾
					}
					else MessageBox("第一条边和第二条边不满足条件");
				}
				else if(m_cylinder.numEdge>=2) //3~N条边，正常检查
				{
					byte r=m_p2t.checkEdge(m_cylinder.ehead);//检查最后一条边是否符合
					if(r)//符合
					{
						if(r==1)
						{
							Edge *p;
							m_cylinder.end->next=p=new Edge;//新建节点
							p->next=NULL;
							m_cylinder.numEdge++;//新建一条边
							p->x1=m_cylinder.end->x2;
							p->y1=m_cylinder.end->y2;
							p->z1=0;
							p->x2=m_cylinder.end->x2;
							p->y2=m_cylinder.end->y2;
							p->z2=0;
							m_cylinder.end=p;//指向链尾
						}
						else //边数据采集结束
						{
							m_cylinder.numEdge++;//此边符合
							m_create=2.21;
						}
					}
					else //边不符合条件
					{
						MessageBox("边不可以与不相邻的边相交！");
					}						
				}				
			}
			else if (m_create==2.21) //得到高度
			{
				m_create=2;
				createCylinder();//由采集的数据产生剖分多边形	
				needRenderScene=1;
			}

			if (m_create==3)//创建球体
			{
				m_sphere.nj=ND_MAX-m_page1.m_spherePanel.m_NJ.GetTopIndex();
				m_sphere.nw=ND_MAX-m_page1.m_spherePanel.m_NW.GetTopIndex();//获得分段数	
				unSelectALL();
				if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//在3D窗口创建，求得该点的3D坐标
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
						m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
						ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
						az=cos(m_theta*PI/180.0);
					m_vertex[0][0]=m_vertex[0][0]-ax/az*m_vertex[0][2];
					m_vertex[0][1]=m_vertex[0][1]-ay/az*m_vertex[0][2];
					m_vertex[0][2]=0;
				}
				m_create=3.1;//指示进入移动状态
			}
			if (m_create==4)//创建锥体
			{
				//读取设置
				m_cylinder.n=ND_MAX-m_page1.m_conePanel.m_DN.GetTopIndex();
				if(((CButton *)m_page1.m_conePanel.GetDlgItem(IDC_REGULAR_POLYGON))->GetCheck()==1)
					m_cylinder.type=0;
				else m_cylinder.type=1;
				m_cylinder.nr=20-m_page1.m_conePanel.m_NR.GetTopIndex();
				m_cylinder.nh=20-m_page1.m_conePanel.m_NH.GetTopIndex();//获得分段数
				if (m_cylinder.type==0)//正底面是正多边形，获取中心点
				{
					unSelectALL();
					if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//在3D窗口创建，求得该点的3D坐标
					{
						gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
							m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
						double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
							ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
							az=cos(m_theta*PI/180.0);
						m_vertex[0][0]=m_vertex[0][0]-ax/az*m_vertex[0][2];
						m_vertex[0][1]=m_vertex[0][1]-ay/az*m_vertex[0][2];
						m_vertex[0][2]=0;
					}
					m_create=4.11;//指示进入移动状态
				}
				else
				{
					
				}
			}
			if (m_create==4.14) //创建锥体
			{
				if (point.y!=m_pp[1][1]) //数据采集完成
				{
					//获取中心坐标和R,H
					if (m_activeView==0||m_activeView==1)
					{
						m_cylinder.cx=m_vertex[0][0];
						m_cylinder.cy=m_vertex[0][1];
						m_cylinder.cz=0;
						m_cylinder.R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
						m_cylinder.axis[0]=1;
						m_cylinder.axis[1]=0;
						m_cylinder.axis[2]=0;
						if(m_vertex[2][2]>0) 
						{
							m_cylinder.angle=0;
							m_cylinder.h=m_vertex[2][2];
						}
						else
						{
							m_cylinder.angle=180;
							m_cylinder.h=-m_vertex[2][2];
						}		
					}					
					createCone();					
				}
				m_create=4;
				needRenderScene=1;	
			}

			if (m_create==5)//创建轮胎时等待第一次左键按下
			{
				unSelectALL();
				//读取分段
				m_torus.nj=ND_MAX-m_page1.m_torusPanel.m_nj.GetTopIndex();
				m_torus.nw=ND_MAX-m_page1.m_torusPanel.m_nw.GetTopIndex();
				if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//在3D窗口创建，求得该点的3D坐标
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
						m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
						ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
						az=cos(m_theta*PI/180.0);
					m_vertex[0][0]=m_vertex[0][0]-ax/az*m_vertex[0][2];
					m_vertex[0][1]=m_vertex[0][1]-ay/az*m_vertex[0][2];
					m_vertex[0][2]=0;
				}
				m_create=5.1;//进入移动，但尚未移动
			}
			if (m_create==5.4)//创建轮胎时等待第二次左键按下
			{
				if (point.y!=m_pp[1][1])
				{
					//获取数据
					if (m_activeView==0||m_activeView==1)
					{
						m_torus.cx=m_vertex[0][0];
						m_torus.cy=m_vertex[0][1];
						m_torus.cz=0;
						double r1=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
						double r2=sqrt((m_vertex[0][0]-m_vertex[2][0])*(m_vertex[0][0]-m_vertex[2][0])+(m_vertex[0][1]-m_vertex[2][1])*(m_vertex[0][1]-m_vertex[2][1]));
						m_torus.R=(r1+r2)/2;
						m_torus.r=fabs(r1-r2)/2;
						createTorus();
					}
				}
				m_create=5;
				needRenderScene=1;				
			}
			if (m_create==6)//创建管子等待第一次左键按下
			{
				unSelectALL();
				//读取分段
				m_pipe.n=ND_MAX-m_page1.m_pipePanel.m_N.GetTopIndex();
				m_pipe.nh=ND_MAX-m_page1.m_pipePanel.m_NH.GetTopIndex();
				m_pipe.nr=ND_MAX-m_page1.m_pipePanel.m_NR.GetTopIndex();
				if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//在3D窗口创建，求得该点的3D坐标
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
						m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
						ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
						az=cos(m_theta*PI/180.0);
					m_vertex[0][0]=m_vertex[0][0]-ax/az*m_vertex[0][2];
					m_vertex[0][1]=m_vertex[0][1]-ay/az*m_vertex[0][2];
					m_vertex[0][2]=0;
				}
				m_create=6.1;//进入移动，但尚未移动
			}
			if (m_create==6.4)//创建管子等待第二次左键按下
			{
				if (point.x!=m_pp[1][0]||point.y!=m_pp[1][1])
				{
					m_create=6.5;
				}
				else
				{
					m_create=6;
					needRenderScene=1;	
				}
			}
			if (m_create==6.7)//创建pipe
			{
				if(point.y!=m_pp[1][1])
				{
					if (m_activeView==0)
					{
						//获取全部数据
						m_pipe.cx=m_vertex[0][0];
						m_pipe.cy=m_vertex[0][1];
						m_pipe.cz=0;
						m_pipe.h=m_vertex[3][2];
						double r1=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
						double r2=sqrt((m_vertex[0][0]-m_vertex[2][0])*(m_vertex[0][0]-m_vertex[2][0])+(m_vertex[0][1]-m_vertex[2][1])*(m_vertex[0][1]-m_vertex[2][1]));
						if (r1<r2) 
						{
							m_pipe.R=r2;
							m_pipe.r=r1;
						}
						else
						{
							m_pipe.R=r1;
							m_pipe.r=r2;
						}
						createPipe();
					}
				}
				m_create=6;
				needRenderScene=1;
			}
		}
		/***********************************************************/
		if (m_workMode==1||m_workMode==3)//选择模式，或均匀放缩
		{	
			m_LBdown=1;
		}
		/***********************************************************/
		if (m_workMode==2||m_workMode==4)//移动模式，或旋转模式
		{
			
			GLuint hits=pickObjects(point.x-1,point.y-1,point.x+1,point.y+1);
			if (hits==0)
			{
				m_BoxChoose=1;//开启框选
			}
			else
			{
				GLuint i=0;
				for (i=0;i<hits;i++)
				{
					if(m_hits[2*i+1]) break;
				}
				
				if(i!=hits)//有的已经选中
				{
					if(ctrlState<0) 
					{
						pCS=m_cs_head;
						while(pCS->next)
						{
							pCS=pCS->next;
							if (pCS->m_index==m_hits[2*i])
							{
								pCS->m_selected=0;
								needRenderScene=1;
								break;
							}
						}
					}
					else m_move=hits+1;//m_move至少为2，和其他情况区别
				}
				else //点击的没有被选中,将最近的一个选中
				{
					
					pCS=m_cs_head;
					while(pCS->next)
					{
						pCS=pCS->next;
						if(pCS->m_index==m_hits[0]) pCS->m_selected=1;				
						else if(ctrlState>=0) pCS->m_selected=0;//区分ctrl是否按下
					}
					needRenderScene=1;
					m_move=1;
				}
			}
			m_LBdown=1;
		}
		/***********************************************************/
		if (m_workMode==5)//点拖动模式
		{
			m_lastMovePP[0]=point.x;
			m_lastMovePP[1]=point.y;
			if(m_drag==1) //点选
			{
				byte curseW=m_page2.m_cursorW,curseH=m_page2.m_cursorH;
				GLuint hits=pickPoints(point.x,point.y,curseW,curseH);
				if (hits==0)//未点中
				{
					if(ctrlState>=0)//没有按下ctrl，清除以前选中的
					{
						if(m_numPointHits!=0)
						{
							for (GLuint i=0;i<m_numPointHits;i++)
							{
								m_pointHits[i]->selected=0;
							}
							m_numPointHits=0;
							needRenderScene=1;
						}
					}
					else if(m_numPointHits!=0) m_drag=1.1;
				}
				else //有点中
				{
					bool used=0;
					GLuint i=0,j=0,ok=0;
					if (m_numPointHits!=0)//先前有选中
					{
						for (GLuint i=0;i<hits;i++) //检查是否点到先前选中的
						{
							for (j=0;j<m_numPointHits;j++)
							{
								if(m_pointHits[j]==m_pointsBuffer[i]) 
								{
									used=1;
									ok=1;
									break;
								}
							}
							if(ok) break;
						}
					}
					if (used)//点中以前的
					{
						m_drag=1.2;//表明LButtonUp时可能再选
						m_numBuffHits=hits;//记载新选中的点数
					}
					else //现在选中的不包含以前选中的
					{
						m_drag=1.1;//up时不会再选
						needRenderScene=1;
						if(ctrlState>=0)//没有按下ctrl，以新代旧
						{
							for (GLuint i=0;i<m_numPointHits;i++)	m_pointHits[i]->selected=0;//取消以前的选中态
							m_numPointHits=hits;//更新选中点数
							for (i=0;i<hits;i++)
							{
								m_pointHits[i]=m_pointsBuffer[i];//复制指针数据
								m_pointHits[i]->selected=1;//使新的数据选中
							}
						}
						else //按下了ctrl，插入到后面
						{
							for (i=0;i<hits;i++)
							{
								m_pointHits[i+m_numPointHits]=m_pointsBuffer[i];//复制指针数据
								m_pointsBuffer[i]->selected=1;//使新的数据选中
							}
							m_numPointHits+=hits;
						}
					}					
				}
			}
			if(m_drag==2) //框选
			{
				GLuint hits=pickPoints(point.x,point.y,8,8);
				if (hits==0) m_drag=2.1;//拉框
				else m_drag=2.2;//移动点
			}
		}
		/***********************************************************/
		if (m_workMode==6) //切割模式: 移动和旋转刀面
		{
			if (m_knife.state==1)//移动
			{
				//检测是否点击到knife
				if (pickKnife(point.x,point.y))	m_knife.state=1.1;
			}
			if (m_knife.state==2)//旋转
			{
				if (pickKnife(point.x,point.y))	m_knife.state=2.1;	
			}
		}
		/***********************************************************/
		if (m_workMode==7.1)//选择性加密模式
		{
			byte curseW=m_page2.m_cursorW,curseH=m_page2.m_cursorH;
			GLuint hits=pickTriangles(point.x,point.y,curseW,curseH);
			if(hits>0) 
			{
				m_chooseDensify=1.1;
				needRenderScene=1;
			}
			else 
			{
				m_chooseDensify=2;
			}
		}

		checkSelected();//更新名字和颜色
		m_pp[0][0]=point.x;
		m_pp[0][1]=point.y;//记录点击的平面位置
	}
	if (needRenderScene) renderScene();	
}

void CMMDlg::calculateRc()//计算显示区域的位置
{
	GetDlgItem(IDC_ALL)->GetWindowRect(&rcALL);// 获取绘图区的位置
	ScreenToClient(rcALL);
	int length=rcALL.right-rcALL.left,width=rcALL.bottom-rcALL.top;
	if (m_view_style==0)//4个均等的窗口
	{
		int x=(length-3*INCREASE)/2.0,y=(width-3*INCREASE)/2.0;
		rc[3].left=INCREASE;
		rc[3].right=x+INCREASE;
		rc[3].bottom=INCREASE;
		rc[3].top=y+INCREASE;

		rc[2].left=2*INCREASE+x;
		rc[2].right=length-INCREASE;
		rc[2].bottom=2*INCREASE+y;
		rc[2].top=width-INCREASE;
		
		rc[1].left=INCREASE;
		rc[1].right=x+INCREASE;
		rc[1].bottom=2*INCREASE+y;
		rc[1].top=width-INCREASE;
		
		rc[0].left=x+2*INCREASE;
		rc[0].right=length-INCREASE;
		rc[0].bottom=INCREASE;
		rc[0].top=y+INCREASE;

		m_viewport=0;
	}
	else if (m_view_style==1)//3大1小
	{
		
		int x,y;
		if ((float)length/width>4.0/3)
		{
			x=(length-3*INCREASE)/4.0;
			y=(width-4*INCREASE)/3.0;
		}	
		else x=y=(width-4*INCREASE)/3.0;
		rc[3].left=INCREASE;
		rc[3].right=x+INCREASE;
		rc[3].bottom=INCREASE;
		rc[3].top=y+INCREASE;
		
		rc[2].left=INCREASE;
		rc[2].right=x+INCREASE;
		rc[2].bottom=2*INCREASE+y;
		rc[2].top=2*INCREASE+2*y;
		
		rc[1].left=INCREASE;
		rc[1].right=x+INCREASE;
		rc[1].bottom=3*INCREASE+2*y;
		rc[1].top=3*INCREASE+3*y;
		
		rc[0].left=x+2*INCREASE;
		rc[0].right=length-INCREASE;
		rc[0].bottom=INCREASE;
		rc[0].top=width-INCREASE;
	}
	if(m_viewport!=0) exchangeRect(rc[0],rc[m_viewport]);
	//更新4个小视图的视口矩阵和投影矩阵
	for(short i=0;i<4;i++)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float ratio=(float)(rc[i].right-rc[i].left)/(rc[i].top-rc[i].bottom);
		if(ratio>1)	glOrtho(-Lw*ratio,Lw*ratio,-Lw,Lw,-10*Lw,10*Lw);
		else glOrtho(-Lw,Lw,-Lw/ratio,Lw/ratio,-10*Lw,10*Lw);
		glViewport(rc[i].left,rc[i].bottom,rc[i].right-rc[i].left,rc[i].top-rc[i].bottom);
		glGetIntegerv(GL_VIEWPORT, m_port[i]);
		glGetDoublev(GL_PROJECTION_MATRIX, m_projMatrix[i]);
	}
	//更新单视图的视口矩阵和投影矩阵
	glLoadIdentity();
	float ratio=(float)rcALL.Width()/rcALL.Height();
	if(ratio>1)	glOrtho(-Lw*ratio,Lw*ratio,-Lw,Lw,-10*Lw,10*Lw);
	else glOrtho(-Lw,Lw,-Lw/ratio,Lw/ratio,-10*Lw,10*Lw);
	glViewport(0,0,rcALL.Width(),rcALL.Height());
	glGetIntegerv(GL_VIEWPORT, m_port[4]);
	glGetDoublev(GL_PROJECTION_MATRIX, m_projMatrix[4]);
	//m_update[0]=m_update[1]=m_update[2]=m_update[3]=1;//一旦calculateRc(),四个窗口中点的平面坐标都要更新
}

void CMMDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	CWnd *pALL= GetDlgItem(IDC_ALL);     
	CWnd *pTab= GetDlgItem(IDC_TAB1); 
	CWnd *pState=GetDlgItem(IDD_PANEL_STATE);//获取控件句柄
    if(nType==SIZE_MINIMIZED) return;   //如果是窗体最小化则什么都不做
    if(pALL)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
    {
		int dx=cx-m_old_rect.Width(),dy=cy-m_old_rect.Height();
		CRect rectALL,rectTab,rectState;   //获取控件变化前大小
		pALL->GetWindowRect(&rectALL);
		pTab->GetWindowRect(&rectTab);
		m_statePanel.GetWindowRect(&rectState);
		ScreenToClient(&rectALL);
		ScreenToClient(&rectTab);
		ScreenToClient(&rectState);//将控件大小转换为在对话框中的区域坐标
		rectALL.right+=dx;
		rectALL.bottom+=dy;
		rectTab.left+=dx;
		rectTab.right+=dx;
		rectTab.bottom+=dy;
		rectState.right+=dx;
		rectState.top+=dy;
		rectState.bottom+=dy;
		pALL->MoveWindow(rectALL);
		pTab->MoveWindow(rectTab);//设置控件大小
		m_statePanel.MoveWindow(rectState);
		
		calculateRc();
		m_updateProjMatrix=1;//标识需要更新投影矩阵
    }
    GetClientRect(&m_old_rect);//将变化后的对话框大小设为旧大小
}

bool CMMDlg::inRect(Rect &rc,CPoint &p)//判断是否在区域中
{
	if(rc.left<=p.x&&p.x<=rc.right&&rc.bottom<=p.y&&p.y<=rc.top)
	{
		return TRUE;
	}
	else return FALSE;
} 

int CMMDlg::howInRect(Rect &rc,CPoint &p)
{
	if(rc.left<=p.x&&p.x<=rc.right&&rc.bottom<=p.y&&p.y<=rc.top)
	{
		if (rc.left<p.x&&p.x<rc.left+AREA&&rc.top-AREA<p.y&&p.y<rc.top) return 2;//左上角
		else if (rc.right-AREA<p.x&&p.x<rc.right&&rc.top-AREA<p.y&&p.y<rc.top) return 3;//右上角
		else return 1;//绘图区
	}
	else return 0; //不在任何区域
}

void CMMDlg::exchangeRect(Rect &a,Rect &b) //交换视图
{
	int top,bottom,left,right;
	top=a.top;
	left=a.left;
	bottom=a.bottom;
	right=a.right;

	a.top=b.top;
	a.left=b.left;
	a.bottom=b.bottom;
	a.right=b.right;

	b.top=top;
	b.left=left;
	b.bottom=bottom;
	b.right=right;
}

void CMMDlg::changeViewStyle()//改变视图分布的风格
{
	if(m_view_style==0) m_view_style=1;
	else m_view_style=0;
	calculateRc();
	renderScene();
}

void CMMDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{	
	CDialog::OnRButtonDown(nFlags, point);
	short ctrlState=GetKeyState(VK_CONTROL);//获取此时control键是否按下
	//右键菜单准备
	CPoint ps;
	ps.x=point.x;
	ps.y=point.y;
	CMenu myMenu ;
	myMenu.LoadMenu(IDR_MENU_POPUP) ;
	CMenu *pMenu = myMenu.GetSubMenu(0);//只显示第一个子菜单
	ClientToScreen( &ps ) ;
	/***************************活动视图切换******************************/
	toGLCoordinate(&point);
	m_rpp[1][0]=point.x;
	m_rpp[1][1]=point.y;//储存右键坐标		
	short i=0;
	bool needRenderScene=0;
	if (m_num_view==4)//四视图时切换活动区域
	{
		for (i=0;i<4;i++)
		{
			if(inRect(rc[i],point)&&m_activeView!=i) //落在第i区域内
			{
				m_activeView=i;
				needRenderScene=1;	
			}
			break;
		}
	}
	GLuint hits;
	if (inDrawArea(point)&&m_workMode!=5)//编辑模式不进行选择
	{
		hits=pickObjects(point.x-1,point.y-1,point.x+1,point.y+1);
		if (hits>0&&m_hits[1]==0) //选中了点，且这个点之前未被选中
		{			
			CS *pCS=m_cs_head;
			while (pCS->next)
			{
				pCS=pCS->next;
				if(pCS->m_index==m_hits[0]) pCS->m_selected=1;
				else if(ctrlState>=0) pCS->m_selected=0;//单选时其他物体将不选中
			}
			needRenderScene=1;
		}
	}
	if(needRenderScene) renderScene();
	if(inDrawArea(point)) pMenu->TrackPopupMenu(TPM_LEFTALIGN,ps.x, ps.y,this);
}

void CMMDlg::OnRButtonDblClk(UINT nFlags, CPoint point) //右键双击，切换视角
{
	CDialog::OnRButtonDblClk(nFlags, point);
	toGLCoordinate(&point);
	bool needRenderScene=0;
	if (m_num_view==4)//四视图时
	for(short i=1;i<4;i++)
	{
		if (rc[i].left<point.x&&point.x<rc[i].left+AREA&&rc[i].top-AREA<point.y&&point.y<rc[i].top)
		{
			m_viewSide[i]=!m_viewSide[i];
			//更新i号窗口视图模型矩阵
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			lookAt(i+3*m_viewSide[i]);
			glScalef(50,50,50);
			glScalef(m_size[i],m_size[i],m_size[i]);
			glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix[i]);
			needRenderScene=1;
			break;
		}
	}
	else //单视图
	//如果点击左上角文字，切换渲染方式
	if (0<point.x&&point.x<AREA&&rcALL.Height()-AREA<point.y&&point.y<rcALL.Height()&&m_activeView!=0)
	{
		m_viewSide[m_activeView]=!m_viewSide[m_activeView];
		//更新m_activeView号窗口视图模型矩阵
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		lookAt(m_activeView+3*m_viewSide[m_activeView]);
		glScalef(50,50,50);
		glScalef(m_size[m_activeView],m_size[m_activeView],m_size[m_activeView]);		
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix[m_activeView]);
		needRenderScene=1;
	}
	if(needRenderScene) renderScene();	
}

double CMMDlg::random()//随机数函数
{
	m_In=16807*(m_In%127773)-2836*(m_In/127773);
	if(m_In<0) m_In=m_In+2147483647;
	return (double)m_In/2147483647;
}

void CMMDlg::changeColor()
{
	m_pallete.m_cc.rgbResult=m_activeColor;
	m_pallete.m_cc.Flags|=CC_RGBINIT;
	m_pallete.DoModal();
	m_activeColor=m_pallete.GetColor();
	m_page1.GetDlgItem(IDC_COLOR)->RedrawWindow();
	CS *p=m_cs_head;
	while (p->next)
	{
		p=p->next;
		if (p->m_selected)//选中者改变颜色
		{
			p->setColor(m_activeColor);
		}
	}
	renderScene();//重绘
}

GLint CMMDlg::pickObjects(GLint x1,GLint y1,GLint x2,GLint y2)//在指定的区域内选择物体
{	
	short aim;//从哪个面观察
	GLuint hits;
	float size; //选择视图的放缩
	GLint viewport[4];
	CPoint point;
	point.x=x1;
	point.y=y1;
	g_renderMode=0;//切换到挑选模式，使一些东西不绘制
	if (m_num_view==4)//四视图
	{
		for (short i=0;i<4;i++)
		{
			if(inRect(rc[i],point)) //落在第i区域内
			{
				viewport[0]=rc[i].left;
				viewport[1]=rc[i].bottom;
				viewport[2]=rc[i].right-rc[i].left;
				viewport[3]=rc[i].top-rc[i].bottom;
				aim=i+3*m_viewSide[i];
				size=m_size[i];
			}			
		}
	}
	else //单视图
	{
		viewport[0]=viewport[1]=0;
		viewport[2]=rcALL.Width();
		viewport[3]=rcALL.Height();												
		aim=m_activeView+3*m_viewSide[m_activeView];		
		size=m_size[m_activeView];
	}			
	glRenderMode(GL_SELECT);//切换为选择模式
	glInitNames();
	glPushName(0);//初始化名字堆栈
	glMatrixMode(GL_PROJECTION);//切换到投影矩阵
	glLoadIdentity();
	gluPickMatrix((x1+x2)/2,(y1+y2)/2,abs(x1-x2),abs(y1-y2),viewport); 
	float ratio=(float)viewport[2]/viewport[3];
	if(ratio>1)	glOrtho(-Lw*ratio,Lw*ratio,-Lw,Lw,-10*Lw,10*Lw);
	else glOrtho(-Lw,Lw,-Lw/ratio,Lw/ratio,-10*Lw,10*Lw);
	glMatrixMode(GL_MODELVIEW);
	lookAt(aim);
	glScalef(size,size,size); 
	glScalef(50,50,50);		
	CS *pCS=m_cs_head;
	/**************被选中的物体在测试时绘制的是曲面*************/
	while(pCS->next)
	{
		pCS=pCS->next;
		if (pCS->m_visible) pCS->show(m_viewManner[m_activeView],m_workMode);
	}
	hits=glRenderMode(GL_RENDER);//切换回渲染模式
	g_renderMode=1;
	if (hits==1) m_hits[0]=m_selectBuff[3];
	if (hits>1)
	{
		GLuint ta,tb;
		m_hits[0]=m_selectBuff[3];
		m_hits[1]=m_selectBuff[1];//先压入一个值
		for (GLuint i=1;i<hits;i++) //压入一个值，然后比较深度大小进行排序
		{
			m_hits[2*i]=m_selectBuff[3+i*4];
			m_hits[2*i+1]=m_selectBuff[1+i*4];
			for (short j=i;j>1;j--)
			{
				if(m_hits[2*j+1]<m_hits[2*j-1]) //需对换
				{
					ta=m_hits[2*j];
					tb=m_hits[2*j+1];
					m_hits[2*j]=m_hits[2*j-2];
					m_hits[2*j+1]=m_hits[2*j-1];
					m_hits[2*j-2]=ta;
					m_hits[2*j-1]=tb;
				}
				else break;
			}
		}
	}
	//判断每个物体是否被选中
	pCS=m_cs_head;
	GLuint count=0;
	while (pCS->next)
	{
		pCS=pCS->next;
		for (GLuint i=0;i<hits;i++)
		{
			if (m_hits[2*i]==pCS->m_index)
			{
				m_hits[2*i+1]=pCS->m_selected;
				count++;
				break;
			}
		}
		if (count==hits) break;//所有选中物体信息都已提取
	}
	return hits;
}

bool CMMDlg::inDrawArea(CPoint &p)//判断是否点击在绘图区
{
	if (m_num_view==4)
	{
		for (short i=0;i<4;i++)
		{
			if (howInRect(rc[i],p)==1) 	return TRUE;
		}
	}
	else //单视图
	{
		Rect rt;
		rt.left=0;
		rt.right=rcALL.Width();
		rt.bottom=0;
		rt.top=rcALL.Height();
		if (howInRect(rt,p)==1) return TRUE;
	}
	return FALSE;
}

void CMMDlg::checkSelected()//检查选择情况，更新名字和颜色
{
	int count=0;
	CS *pCS=m_cs_head,*pSel;
	while(pCS->next)
	{
		pCS=pCS->next;
		if(pCS->m_selected) 
		{
			count++;
			pSel=pCS;
		}
	}
	CWnd *pName=m_page1.GetDlgItem(IDC_NAME);
	if(count==0)
	{
		pName->SetWindowText("");
		pName->EnableWindow(FALSE);
	}
	if(count==1) 
	{
		m_activeColor=RGB(pSel->m_color[0],pSel->m_color[1],pSel->m_color[2]);
		m_page1.GetDlgItem(IDC_COLOR)->RedrawWindow();
		pName->SetWindowText(pSel->m_name);
		pName->EnableWindow(TRUE);
	}
	if (count>1)
	{
		CString s;
		s.Format("选择了%d个对象",count);
		pName->SetWindowText(s);
		pName->EnableWindow(FALSE);
	}
}

void CMMDlg::updateName()//更新对象的颜色
{	
	CS *pCS=m_cs_head;
	while (pCS->next)//查找被选择的项
	{
		pCS=pCS->next;
		if (pCS->m_selected) break;
	}
	CString s;
	m_page1.GetDlgItem(IDC_NAME)->GetWindowText(s);
	if(s!="") pCS->m_name=s;
}

void CMMDlg::createCuboid()//创建长方体
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//移动到最后
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createCuboid(m_cuboid);
	pCS->setColor(m_randomColor);
	getRandomColor();//设定新的随机色
	m_maxIndex++;
	pCS->m_index=m_maxIndex;
	m_num_model++;	
	renderScene();
}

void CMMDlg::createCone()//创建椎体
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//移动到最后
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createCone(m_cylinder);
	pCS->setColor(m_randomColor);
	getRandomColor();//设定新的随机色
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::createSphere()//创建球体
{

	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//移动到最后
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createSphere(m_sphere);
	pCS->setColor(m_randomColor);
	getRandomColor();//设定新的随机色
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::createCylinder()//创建柱体
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//移动到最后
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createCylinder(m_cylinder);
	pCS->setColor(m_randomColor);
	getRandomColor();//设定新的随机色
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::createTorus()//创建环形
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//移动到最后
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createTorus(m_torus);
	pCS->setColor(m_randomColor);
	getRandomColor();//设定新的随机色
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::createPipe()//创建管状体
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//移动到最后
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createPipe(m_pipe);
	pCS->setColor(m_randomColor);
	getRandomColor();//设定新的随机色
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::drawNewObject()//创建过程绘制物体
{
	glColor3ub(GetRValue(m_randomColor),GetGValue(m_randomColor),GetBValue(m_randomColor));
	if (m_create==1.2)
	{
		if (m_activeView==0||m_activeView==1)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3d(m_vertex[0][0],m_vertex[0][1],m_vertex[0][2]);
			glVertex3d(m_vertex[0][0],m_vertex[1][1],m_vertex[0][2]);
			glVertex3d(m_vertex[1][0],m_vertex[1][1],m_vertex[0][2]);
			glVertex3d(m_vertex[1][0],m_vertex[0][1],m_vertex[0][2]);
			glEnd();
		}	
		if (m_activeView==2)
		{
			glBegin(GL_LINE_LOOP);
				glVertex3d(0,m_vertex[0][1],m_vertex[0][2]);
				glVertex3d(0,m_vertex[0][1],m_vertex[1][2]);
				glVertex3d(0,m_vertex[1][1],m_vertex[1][2]);
				glVertex3d(0,m_vertex[1][1],m_vertex[0][2]);
			glEnd();
		}
		if (m_activeView==3)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3d(m_vertex[0][0],0,m_vertex[0][2]);
			glVertex3d(m_vertex[0][0],0,m_vertex[1][2]);
			glVertex3d(m_vertex[1][0],0,m_vertex[1][2]);
			glVertex3d(m_vertex[1][0],0,m_vertex[0][2]);
			glEnd();
		}
	}
	if (m_create==1.4)
	{
		if (m_activeView==0||m_activeView==1)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3d(m_vertex[0][0],m_vertex[0][1],0);
			glVertex3d(m_vertex[0][0],m_vertex[1][1],0);
			glVertex3d(m_vertex[1][0],m_vertex[1][1],0);
			glVertex3d(m_vertex[1][0],m_vertex[0][1],0);
			glEnd();
			glBegin(GL_LINE_LOOP);
			glVertex3d(m_vertex[0][0],m_vertex[0][1],m_vertex[2][2]);
			glVertex3d(m_vertex[0][0],m_vertex[1][1],m_vertex[2][2]);
			glVertex3d(m_vertex[1][0],m_vertex[1][1],m_vertex[2][2]);
			glVertex3d(m_vertex[1][0],m_vertex[0][1],m_vertex[2][2]);
			glEnd();
			glBegin(GL_LINES);
			glVertex3d(m_vertex[0][0],m_vertex[0][1],0);
			glVertex3d(m_vertex[0][0],m_vertex[0][1],m_vertex[2][2]);
			glVertex3d(m_vertex[0][0],m_vertex[1][1],0);
			glVertex3d(m_vertex[0][0],m_vertex[1][1],m_vertex[2][2]);
			glVertex3d(m_vertex[1][0],m_vertex[1][1],0);
			glVertex3d(m_vertex[1][0],m_vertex[1][1],m_vertex[2][2]);
			glVertex3d(m_vertex[1][0],m_vertex[0][1],0);
			glVertex3d(m_vertex[1][0],m_vertex[0][1],m_vertex[2][2]);
			glEnd();
		}	
		if (m_activeView==2)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3d(0,m_vertex[0][1],m_vertex[0][2]);
			glVertex3d(0,m_vertex[0][1],m_vertex[1][2]);
			glVertex3d(0,m_vertex[1][1],m_vertex[1][2]);
			glVertex3d(0,m_vertex[1][1],m_vertex[0][2]);
			glEnd();
			glBegin(GL_LINE_LOOP);
			glVertex3d(m_vertex[2][0],m_vertex[0][1],m_vertex[0][2]);
			glVertex3d(m_vertex[2][0],m_vertex[0][1],m_vertex[1][2]);
			glVertex3d(m_vertex[2][0],m_vertex[1][1],m_vertex[1][2]);
			glVertex3d(m_vertex[2][0],m_vertex[1][1],m_vertex[0][2]);
			glEnd();
			glBegin(GL_LINES);
			glVertex3d(0,m_vertex[0][1],m_vertex[0][2]);
			glVertex3d(m_vertex[2][0],m_vertex[0][1],m_vertex[0][2]);
			glVertex3d(0,m_vertex[0][1],m_vertex[1][2]);
			glVertex3d(m_vertex[2][0],m_vertex[0][1],m_vertex[1][2]);
			glVertex3d(0,m_vertex[1][1],m_vertex[1][2]);
			glVertex3d(m_vertex[2][0],m_vertex[1][1],m_vertex[1][2]);
			glVertex3d(0,m_vertex[1][1],m_vertex[0][2]);
			glVertex3d(m_vertex[2][0],m_vertex[1][1],m_vertex[0][2]);
			glEnd();
		}
		if (m_activeView==3)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3d(m_vertex[0][0],0,m_vertex[0][2]);
			glVertex3d(m_vertex[0][0],0,m_vertex[1][2]);
			glVertex3d(m_vertex[1][0],0,m_vertex[1][2]);
			glVertex3d(m_vertex[1][0],0,m_vertex[0][2]);
			glEnd();
			glBegin(GL_LINE_LOOP);
			glVertex3d(m_vertex[0][0],m_vertex[2][1],m_vertex[0][2]);
			glVertex3d(m_vertex[0][0],m_vertex[2][1],m_vertex[1][2]);
			glVertex3d(m_vertex[1][0],m_vertex[2][1],m_vertex[1][2]);
			glVertex3d(m_vertex[1][0],m_vertex[2][1],m_vertex[0][2]);
			glEnd();
			glBegin(GL_LINES);
			glVertex3d(m_vertex[0][0],0,m_vertex[0][2]);
			glVertex3d(m_vertex[0][0],m_vertex[2][1],m_vertex[0][2]);
			glVertex3d(m_vertex[0][0],0,m_vertex[1][2]);
			glVertex3d(m_vertex[0][0],m_vertex[2][1],m_vertex[1][2]);
			glVertex3d(m_vertex[1][0],0,m_vertex[1][2]);
			glVertex3d(m_vertex[1][0],m_vertex[2][1],m_vertex[1][2]);
			glVertex3d(m_vertex[1][0],0,m_vertex[0][2]);
			glVertex3d(m_vertex[1][0],m_vertex[2][1],m_vertex[0][2]);
			glEnd();
		}
	}
	if (m_create==2.12||m_create==4.12)
	{
		if (m_activeView==0||m_activeView==1)
		{
			double ks=sin(PI/m_cylinder.n),kc=cos(PI/m_cylinder.n);
			double R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
			glPushMatrix();
			glTranslated(m_vertex[0][0],m_vertex[0][1],0);
			for(int i=0;i<m_cylinder.n;i++)
			{
				glPushMatrix();
				glRotated(i*360/m_cylinder.n,0,0,1);
				glBegin(GL_LINE_LOOP);
				glVertex3d(0,0,0);
				glVertex3d(R*kc,R*ks,0);
				glVertex3d(R*kc,-R*ks,0);
				glEnd();
				glPopMatrix();
			}
			glPopMatrix();			
		}
	}
	if (m_create==2.14)
	{
		if (m_activeView==0||m_activeView==1)
		{
			double ks=sin(PI/m_cylinder.n),kc=cos(PI/m_cylinder.n);
			double R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
			glPushMatrix();
			glTranslated(m_vertex[0][0],m_vertex[0][1],0);
			for(int i=0;i<m_cylinder.n;i++)
			{
				glPushMatrix();
				glRotated(i*360/m_cylinder.n,0,0,1);
				glBegin(GL_LINE_LOOP);
				glVertex3d(0,0,0);
				glVertex3d(R*kc,R*ks,0);
				glVertex3d(R*kc,-R*ks,0);
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3d(0,0,m_vertex[2][2]);
				glVertex3d(R*kc,R*ks,m_vertex[2][2]);
				glVertex3d(R*kc,-R*ks,m_vertex[2][2]);
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3d(R*kc,-R*ks,0);
				glVertex3d(R*kc,R*ks,0);
				glVertex3d(R*kc,R*ks,m_vertex[2][2]);
				glVertex3d(R*kc,-R*ks,m_vertex[2][2]);
				glEnd();
				glPopMatrix();
			}
			glPopMatrix();			
		}
	}
	if(m_create==2.2)//点划线构建模式
	{
		Edge *pe=m_cylinder.ehead;
		glPushAttrib(GL_CURRENT_BIT);
		glColor3ub(255,0,0);
		glBegin(GL_POINTS);
		glVertex2d(pe->next->x1,pe->next->y1);
		glEnd();
		glPopAttrib();
		glBegin(GL_LINES);		
		while (pe->next)
		{
			pe=pe->next;
			glVertex2d(pe->x1,pe->y1);
			glVertex2d(pe->x2,pe->y2);
		}
		glEnd();
	}
	if(m_create==2.21)//点划线+高
	{
		Edge *pe=m_cylinder.ehead;			
		while (pe->next)
		{
			pe=pe->next;
			glBegin(GL_LINE_LOOP);	
			glVertex3d(pe->x1,pe->y1,0);
			glVertex3d(pe->x2,pe->y2,0);
			glVertex3d(pe->x2,pe->y2,m_cylinder.h);
			glVertex3d(pe->x1,pe->y1,m_cylinder.h);
			glEnd();
		}		
	}
	if (m_create==3.2)
	{
		if (m_activeView==0||m_activeView==1)
		{
			double R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
			glPushMatrix();
			glTranslated(m_vertex[0][0],m_vertex[0][1],0);
			glutWireSphere(R,m_sphere.nj,m_sphere.nw);
			glPopMatrix();
		}
	}
	if (m_create==4.14)
	{
		if (m_activeView==0||m_activeView==1)
		{
			double ks=sin(PI/m_cylinder.n),kc=cos(PI/m_cylinder.n);
			double R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
			glPushMatrix();
			glTranslated(m_vertex[0][0],m_vertex[0][1],0);
			for(int i=0;i<m_cylinder.n;i++)
			{
				glPushMatrix();
				glRotated(i*360/m_cylinder.n,0,0,1);
				glBegin(GL_LINE_LOOP);
				glVertex3d(0,0,0);
				glVertex3d(R*kc,R*ks,0);
				glVertex3d(R*kc,-R*ks,0);
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3d(0,0,m_vertex[2][2]);
				glVertex3d(R*kc,R*ks,0);
				glVertex3d(R*kc,-R*ks,0);
				glEnd();
				glPopMatrix();
			}
			glPopMatrix();			
		}
	}
	if (m_create==5.2)
	{
		if (m_activeView==0||m_activeView==1)
		{
			double R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
			glPushMatrix();
			glTranslated(m_vertex[0][0],m_vertex[0][1],0);
			glutWireTorus(0.01,R,m_torus.nj,m_torus.nw);
			glPopMatrix();
		}
	}
	if (m_create==5.4)
	{
		if (m_activeView==0||m_activeView==1)
		{
			double r1=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
			double r2=sqrt((m_vertex[0][0]-m_vertex[2][0])*(m_vertex[0][0]-m_vertex[2][0])+(m_vertex[0][1]-m_vertex[2][1])*(m_vertex[0][1]-m_vertex[2][1]));
			glPushMatrix();
			glTranslated(m_vertex[0][0],m_vertex[0][1],0);
			glutWireTorus(fabs(r1-r2)/2,(r1+r2)/2,m_torus.nj,m_torus.nw);
			glPopMatrix();
		}
	}
	if (m_create==6.2)
	{
		double r=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
		double y=r*tan(PI/m_pipe.n);
		glPushMatrix();
		glTranslated(m_vertex[0][0],m_vertex[0][1],0);
		for(int i=0;i<m_pipe.n;i++)
		{
			glPushMatrix();
			glRotated(i*360/m_pipe.n,0,0,1);
			glBegin(GL_LINES);
			glVertex3d(r,-y,0);
			glVertex3d(r,y,0);
			glEnd();
			glPopMatrix();
		}
		glPopMatrix();
	}
	if (m_create==6.4)
	{
		double r1=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
		double r2=sqrt((m_vertex[0][0]-m_vertex[2][0])*(m_vertex[0][0]-m_vertex[2][0])+(m_vertex[0][1]-m_vertex[2][1])*(m_vertex[0][1]-m_vertex[2][1]));
		double y1=r1*tan(PI/m_pipe.n),y2=r2*tan(PI/m_pipe.n);
		glPushMatrix();
		glTranslated(m_vertex[0][0],m_vertex[0][1],0);
		for(int i=0;i<m_pipe.n;i++)
		{
			glPushMatrix();
			glRotated(i*360/m_pipe.n,0,0,1);
			glBegin(GL_LINE_LOOP);
			glVertex3d(r1,-y1,0);
			glVertex3d(r1,y1,0);
			glVertex3d(r2,y2,0);
			glVertex3d(r2,-y2,0);
			glEnd();
			glPopMatrix();
		}
		glPopMatrix();
	}
	if (m_create==6.7)
	{
		double r1=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
		double r2=sqrt((m_vertex[0][0]-m_vertex[2][0])*(m_vertex[0][0]-m_vertex[2][0])+(m_vertex[0][1]-m_vertex[2][1])*(m_vertex[0][1]-m_vertex[2][1]));
		double y1=r1*tan(PI/m_pipe.n),y2=r2*tan(PI/m_pipe.n);
		double h=m_vertex[3][2];
		glPushMatrix();
		glTranslated(m_vertex[0][0],m_vertex[0][1],0);
		for(int i=0;i<m_pipe.n;i++)
		{
			glPushMatrix();
			glRotated(i*360/m_pipe.n,0,0,1);
			glBegin(GL_LINE_LOOP);
			glVertex3d(r1,-y1,0);
			glVertex3d(r1,y1,0);
			glVertex3d(r2,y2,0);
			glVertex3d(r2,-y2,0);
			glEnd();
			glBegin(GL_LINE_LOOP);
			glVertex3d(r1,-y1,h);
			glVertex3d(r1,y1,h);
			glVertex3d(r2,y2,h);
			glVertex3d(r2,-y2,h);
			glEnd();
			glBegin(GL_LINES);
			glVertex3d(r1,-y1,0);
			glVertex3d(r1,-y1,h);
			glVertex3d(r1,y1,0);
			glVertex3d(r1,y1,h);
			glVertex3d(r2,y2,0);
			glVertex3d(r2,y2,h);
			glVertex3d(r2,-y2,0);
			glVertex3d(r2,-y2,h);
			glEnd();
			glPopMatrix();
		}
		glPopMatrix();
	}
}

bool CMMDlg::isHotKey(WPARAM VirKey)//判断是否属于快捷键
{
	if (VirKey==m_hotKeys.del_object||
		VirKey==m_hotKeys.phi_minus||
		VirKey==m_hotKeys.phi_plus||
		VirKey==m_hotKeys.R_minus||
		VirKey==m_hotKeys.R_plus||
		VirKey==m_hotKeys.theta_minus||
		VirKey==m_hotKeys.theta_plus)
	return TRUE;
	return FALSE;
}

COLORREF CMMDlg::getRandomColor()//随机颜色
{
	byte i=63*random();
	return m_randomColor=RGB(m_colors[i][0],m_colors[i][1],m_colors[i][2]);
}

void CMMDlg::unSelectALL()//使所有物体全不选中
{
	CS *pCS=m_cs_head;
	while (pCS->next)
	{
		pCS=pCS->next;
		pCS->m_selected=0;
	}
}

BOOL CMMDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CS *pCS=m_cs_head;
	bool needRenderScene=0;
	switch(LOWORD(wParam))   
	{   
	case IDR_MENU_FILE_EXIT:   //加速键ID   
		//exit(0);
		OnClose();
		break; 
	case ID_HIDE:
		pCS=m_cs_head;
		while (pCS->next)
		{
			pCS=pCS->next;
			if (pCS->m_selected)
			{
				pCS->m_selected=0;//不选中
				pCS->m_visible=0;//不可见
			}				
		}
		renderScene();	
		break;
	case ID_SHOW_ALL:
		pCS=m_cs_head;
		while (pCS->next)
		{
			pCS=pCS->next;
			if (pCS->m_visible==0)
			{
				pCS->m_visible=1;//使其可见
				needRenderScene=1;
			}				
		}
		if(needRenderScene) renderScene();
		break;
	case ID_FREEZE:
		pCS=m_cs_head;
		while (pCS->next)
		{
			pCS=pCS->next;
			if (pCS->m_selected)
			{
				pCS->m_free=0;//使其冻结，不可编辑
			}				
		}
		renderScene();
		break;
	case ID_FREE:
		pCS=m_cs_head;
		while (pCS->next)
		{
			pCS=pCS->next;
			if (pCS->m_selected)
			{
				pCS->m_free=1;//解除冻结，使可编辑
			}				
		}
		renderScene();
		break;
	case ID_DELETE_OBJECT:
		deleteObject();
		renderScene();
		break;
	case ID_COPY:
		//将要复制的模型指针写入缓冲区
		m_numToCopy=0;
		pCS=m_cs_head;
		while(pCS->next)
		{
			pCS=pCS->next;
			if(pCS->m_selected)
			{
				if(m_numToCopy==MAX_COPY_INDEX) MessageBox("要复制的个数超出最大值!");
				m_copyPointer[m_numToCopy]=pCS;
				m_numToCopy++;
			}
		}
		m_rpp[0][0]=m_rpp[1][0];
		m_rpp[0][1]=m_rpp[1][1];//记下点击复制时右键坐标
		break;
	case ID_PASTE:
		{
			byte aim;
			if (m_num_view==4) aim=m_activeView;
			else aim=4;
			double x1,y1,z1,x2,y2,z2,x3,y3,x4,y4;
			gluUnProject((GLdouble)m_rpp[0][0],(GLdouble)m_rpp[0][1],0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&x1,&y1,&z1);
			gluUnProject((GLdouble)m_rpp[1][0],(GLdouble)m_rpp[1][1],0,m_modelMatrix[m_activeView],
						m_projMatrix[aim],m_port[aim],&x2,&y2,&z2);
			double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
				ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
				az=cos(m_theta*PI/180.0);
			x3=x1-ax/az*z1;
			y3=y1-ay/az*z1;			
			x4=x2-ax/az*z2;
			y4=y2-ay/az*z2;

			pCS=m_cs_head;
			while(pCS->next) pCS=pCS->next;
			for (GLuint i=0;i<m_numToCopy;i++)
			{
				pCS->next=new CS;
				pCS=pCS->next;
				pCS->copy(m_copyPointer[i]);
				m_copyPointer[i]->m_selected=0;//取消原来的选中
				m_num_model++;//模型数增加
				m_maxIndex++;				
				pCS->m_index=m_maxIndex;//赋编号
				pCS->m_name+="_复件";
				pCS->m_selected=1;//选中
				if (m_activeView==0&&m_theta!=90||m_activeView==1)//在3D窗口和窗口1
				{										
					pCS->m_translate[0]+=x4-x3; 
					pCS->m_translate[1]+=y4-y3; 
				}
				else if (m_activeView==2)
				{
					pCS->m_translate[2]+=z2-z1; 
					pCS->m_translate[1]+=y2-y1; 
				}
				else if (m_activeView==3)
				{
					pCS->m_translate[2]+=z2-z1; 
					pCS->m_translate[0]+=x2-x1; 
				}
			}
			pCS->next=NULL;
			needRenderScene=1;
		}		
		break;
	case ID_EDIT_THIS:
		{
			//右键单选
			GLuint	hits=pickObjects(m_rpp[1][0]-1,m_rpp[1][1]-1,m_rpp[1][0]+1,m_rpp[1][1]+1);
			if (hits>0) //选中了点
			{			
				CS *pCS=m_cs_head;
				while (pCS->next)
				{
					pCS=pCS->next;
					if(pCS->m_index==m_hits[0])
					{
						pCS->m_selected=1;
						pActiveCS=pCS;//改变正在编辑的对象
					}
					else 
					{
						pCS->m_selected=0;//单选时其他物体将不选中
						pCS->m_visible=0;//隐藏其他物体
					}
				}
				pDlg[0]->ShowWindow(SW_HIDE);
				m_tab.SetCurSel(1);
				pDlg[1]->ShowWindow(SW_SHOW);
				m_cur=1;
				m_page2.m_startDrag.SetCheck(TRUE); 
				m_page2.OnStartDrag();
				needRenderScene=1; 
			}			
		}		
		break;
	case ID_EXPORT_CS://导出为smm文件
		{
			CS *p=m_cs_head;
			int num=0;
			while (p->next)
			{
				p=p->next;
				if(p->m_selected)
				{
					pActiveCS=p;
					num++;
				}
			}
			if(num>1) MessageBox("抱歉：一次只能导出一个对象！");
			else if(num==1) saveFile(2);
		}
		break;
	default:
		break;   		
	}
	CDialog::OnCommand(wParam, lParam);
	if(needRenderScene) renderScene();
	return TRUE;
}

void CMMDlg::deleteObject()
{
	CS *pCS=m_cs_head,*pTemp;
	while (pCS->next)
	{
		pTemp=pCS->next;
		if(pTemp->m_selected)
		{
			pCS->next=pTemp->next;
			delete pTemp;
			m_num_model--;
			if (m_num_model==0)//自动进入创建等待
			{
				m_workMode=0;
				m_create=0;
			}
		}
		else pCS=pTemp;
	}
}

GLint CMMDlg::pickPoints(GLint x,GLint y,GLint w,GLint h)//选点
{
	GLuint hits;
	GLdouble modelMatrix[16];
	byte aim;
	if (m_num_view==4) aim=m_activeView;
	else aim=4;		
	glRenderMode(GL_SELECT);//切换为选择模式
	glInitNames();
	glPushName(0);//初始化名字堆栈
	glMatrixMode(GL_PROJECTION);//切换到投影矩阵
	glLoadIdentity();
	gluPickMatrix(x,y,w,h,m_port[aim]); 
	glMultMatrixd(m_projMatrix[aim]);//先乘上已储存的视图矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(m_modelMatrix[m_activeView]); //加载视角		
	/*******************************************/
	CS *pCS=m_cs_head;
	GLuint index=0;	
	while(pCS->next)
	{
		pCS=pCS->next;
		if (pCS->m_visible) 
		{
			//加载移动和旋转
			glPushMatrix();
			glTranslated(pCS->m_translate[0],pCS->m_translate[1],pCS->m_translate[2]);//最后平移
			glMultMatrixd(pCS->m_mvMatrix);//再乘上已储存的旋转矩阵		
			glScaled(pCS->m_size,pCS->m_size,pCS->m_size);//先进行放缩
			
			VertexRef *p=pCS->iv;			
			while (p->next)
			{
				p=p->next;
				//设置点的大小
				index++;
				glLoadName(index);//替换名称
				glBegin(GL_POINTS);
				glVertex3d(p->v[0],p->v[1],p->v[2]);
				glEnd();
			}		
			glPopMatrix();
		} 
	}	
	glFinish();
	hits=glRenderMode(GL_RENDER);//切换回渲染模式
	if(hits==0) return hits;
	if(hits>1024) 
	{
		MessageBox("选择溢出！");
		return 0;
	}
	GLuint buffCur=1;//指向记录缓冲区的序号
	GLuint pointBuffCur=0;//结果坐标的序号
	GLdouble wx,wy,wz;//平面坐标
	float depth;//实际显示的深度
	index=0;
	pCS=m_cs_head;
	while(pCS->next)
	{
		pCS=pCS->next;
		if (pCS->m_visible) 
		{
			glPushMatrix();
			//加载移动和旋转
			glTranslated(pCS->m_translate[0],pCS->m_translate[1],pCS->m_translate[2]);
			glMultMatrixd(pCS->m_mvMatrix);
			glScaled(pCS->m_size,pCS->m_size,pCS->m_size);
			VertexRef *p=pCS->iv;
			while (p->next)
			{
				p=p->next;
				index++;
				if(index==m_selectBuff[4*buffCur-1])//等于被记录的索引,p指向选中者
				{	
					glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);//获取视图模型矩阵，便于反投影
					//检测是否可见，因为可能被曲面挡住
					gluProject(p->v[0],p->v[1],p->v[2],modelMatrix,
						m_projMatrix[aim],m_port[aim],&wx,&wy,&wz);
					glReadPixels((int)wx,(int)wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
					if(fabs(wz-depth)<=0.005) //若能看到p点,压入记录指针数组//深度值大小尚需讨论
					{
						m_pointsBuffer[pointBuffCur]=p;
						pointBuffCur++;
					}		
					if(buffCur==hits) return pointBuffCur;//选择的点已经扫描完毕，直接跳出
					buffCur++;
				}
			}
			glPopMatrix();
		}
	}
	return pointBuffCur;
}

GLint CMMDlg::pickTriangles(GLint x,GLint y,GLint w,GLint h)//挑选三角形,进入此模式先清空m_hits
{
	byte aim;
	if (m_num_view==4) aim=m_activeView;
	else aim=4;		
	glRenderMode(GL_SELECT);//切换为选择模式
	glInitNames();
	glPushName(0);//初始化名字堆栈
	glMatrixMode(GL_PROJECTION);//切换到投影矩阵
	glLoadIdentity();
	gluPickMatrix(x,y,w,h,m_port[aim]); 
	glMultMatrixd(m_projMatrix[aim]);//先乘上已储存的视图矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(m_modelMatrix[m_activeView]); //加载视角		
	/*******************************************/
	if(!pActiveCS) return 0;
	GLuint index=0;	
	if (pActiveCS->m_visible)//可见是才进行选择 
	{
		//加载移动和旋转
		glPushMatrix();
		glTranslated(pActiveCS->m_translate[0],pActiveCS->m_translate[1],pActiveCS->m_translate[2]);//最后平移
		glMultMatrixd(pActiveCS->m_mvMatrix);//再乘上已储存的旋转矩阵		
		glScaled(pActiveCS->m_size,pActiveCS->m_size,pActiveCS->m_size);//先进行放缩
		Triangle *p=pActiveCS->it;
		if (m_viewManner[m_activeView])//先绘制三角面
		{
			glBegin(GL_TRIANGLES);
			while (p->next)
			{
				p=p->next;
				glVertex3d(p->pt[0].v[0],p->pt[0].v[1],p->pt[0].v[2]);
				glVertex3d(p->pt[1].v[0],p->pt[1].v[1],p->pt[1].v[2]);
				glVertex3d(p->pt[2].v[0],p->pt[2].v[1],p->pt[2].v[2]);
			}
			glEnd();
			p=pActiveCS->it;
		}	
		while (p->next)
		{
			p=p->next;
			//设置点的大小
			index++;
			glLoadName(index);//替换名称
			glBegin(GL_POINTS);
			glVertex3d((p->pt[0].v[0]+p->pt[1].v[0]+p->pt[2].v[0])/3,(p->pt[0].v[1]+p->pt[1].v[1]+p->pt[2].v[1])/3,(p->pt[0].v[2]+p->pt[1].v[2]+p->pt[2].v[2])/3);
			glEnd();
		}		
		glPopMatrix();
	}
	else return 0;
	glFinish();
	GLuint hits=glRenderMode(GL_RENDER);//切换回渲染模式
	short ctrlState=GetKeyState(VK_CONTROL);//获取此时control键是否按下	
	if(ctrlState>=0)//没有按下control键
	{
		if (hits)
		{
			for(GLuint i=1;i<=hits;i++) m_hits[i]=m_selectBuff[4*i-1];
		}
		m_hits[0]=hits;		
	}
	else 
	{
		//选中的添加到其中
		if (m_hits[0])//原来有选中
		{
			GLuint cur=m_hits[0]+1;
			for (GLuint i=1;i<=hits;i++)
			{
				for (GLuint j=1;j<=m_hits[0];j++)
				{
					if (m_hits[j]==m_selectBuff[4*i-1]) break;//有重的，不管
				}
				if (j>m_hits[0])//没有重的,按大小插入
				{
					m_hits[cur]=m_selectBuff[4*i-1];//放在最后
					GLuint k=cur,temp;
					while (m_hits[k]<m_hits[k-1])
					{
						temp=m_hits[k];
						m_hits[k]=m_hits[k-1];
						m_hits[k-1]=temp;
						k--;
					}
					cur++;
				}
			}
			m_hits[0]=cur-1;
		}
		else //原来没有选中，直接加入
		{
			for(GLuint i=1;i<=hits;i++) m_hits[i]=m_selectBuff[4*i-1];
			m_hits[0]=hits;
		}
	}
	return hits;
}

int CMMDlg::chooseOneObject()//使选中的物体只有一个
{
	CS* p=m_cs_head;
	int num=0;
	while (p->next)
	{
		p=p->next;
		if (p->m_selected)
		{
			if(num==0)	pActiveCS=p;
			else	p->m_selected=0;
			num++;
		}
	}
	return num;
}

void CMMDlg::hideUnselected()//隐藏未被选中的
{
	CS* p=m_cs_head;
	while (p->next)
	{
		p=p->next;
		if (!p->m_selected)	p->m_visible=0;
	}
}

bool CMMDlg::pickKnife(GLint x,GLint y)//判断是否点击到刀面
{
	GLuint hits;
	byte aim;
	if (m_num_view==4) aim=m_activeView;
	else aim=4;		
	glRenderMode(GL_SELECT);//切换为选择模式
	glInitNames();
	glPushName(0);//初始化名字堆栈
	glMatrixMode(GL_PROJECTION);//切换到投影矩阵
	glLoadIdentity();
	gluPickMatrix(x,y,2,2,m_port[aim]); 
	glMultMatrixd(m_projMatrix[aim]);//先乘上已储存的视图矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(m_modelMatrix[m_activeView]); //加载视角
	//以下绘制knife	
	glTranslated(m_knife.t[0],m_knife.t[1],m_knife.t[2]);
	glMultMatrixd(m_knife.rotateMatrix);
	glScaled(m_knife.size,m_knife.size,m_knife.size);
	glDisable(GL_CULL_FACE);
	glBegin(GL_QUADS);
	glLoadName(1);
	glVertex3d(-m_knife.dx/2,-m_knife.dy/2,0);
	glVertex3d(m_knife.dx/2,-m_knife.dy/2,0);
	glVertex3d(m_knife.dx/2,m_knife.dy/2,0);
	glVertex3d(-m_knife.dx/2,m_knife.dy/2,0);
	glEnd();
	glEnable(GL_CULL_FACE);
	hits=glRenderMode(GL_RENDER);//切换回渲染模式
	if (hits==0) return FALSE;
	else return TRUE;
}

void CMMDlg::drawKnife()//绘制切割刀
{	
	glPushMatrix();
	glTranslated(m_knife.t[0]+m_knife.tx,m_knife.t[1]+m_knife.ty,m_knife.t[2]+m_knife.tz);//移动
	glRotated(m_knife.angle,m_knife.axis[0],m_knife.axis[1],m_knife.axis[2]);//临时转动
	glMultMatrixd(m_knife.rotateMatrix);//先前的转动
	glScaled(m_knife.size,m_knife.size,m_knife.size);//放缩
	glBegin(GL_QUADS);
	glColor3ub(255,0,0);
	glVertex2d(-m_knife.dx/2,-m_knife.dy/2);
	glVertex2d(m_knife.dx/2,-m_knife.dy/2);
	glVertex2d(m_knife.dx/2,m_knife.dy/2);
	glVertex2d(-m_knife.dx/2,m_knife.dy/2);
	glEnd();
	glCullFace(GL_FRONT);
	glBegin(GL_QUADS);	
	glColor3ub(255,255,0);
	glVertex2d(-m_knife.dx/2,-m_knife.dy/2);
	glVertex2d(m_knife.dx/2,-m_knife.dy/2);
	glVertex2d(m_knife.dx/2,m_knife.dy/2);
	glVertex2d(-m_knife.dx/2,m_knife.dy/2);
	glEnd();
	glCullFace(GL_BACK);
	glPopMatrix();
	glEnable(GL_CULL_FACE);
}

void CMMDlg::deleteTriangle(Triangle *q,CS *pCS,CSearch &s1,CSearch &s2)//删除三角形和相应节点
{
	Triangle *p=q->next;
	for (byte i=0;i<3;i++)
	{
		VertexRef *vr=p->pt[i].vr;
		TriangleRef *tr=p->pt[i].tr,*qtr;
		if (vr->nextRef->nextRef==NULL)//需删除vr和tr
		{
			delete vr->nextRef;
			VertexRef *pvr=pCS->iv;
			while (pvr->next!=vr) pvr=pvr->next;
			pvr->next=vr->next;
			delete vr;
			s1.del(vr);//尝试删除vr索引
			s2.del(vr);//尝试删除vr索引
		}
		else//只删除tr
		{
			if (vr->nextRef==tr) //tr紧邻vr
			{
				vr->nextRef=tr->nextRef;
				delete tr;
			}
			else
			{
				qtr=vr->nextRef;
				while (qtr->nextRef!=tr) qtr=qtr->nextRef;
				qtr->nextRef=tr->nextRef;
				delete tr;
			}
		}
	}	
	//删除三角形
	q->next=p->next;
	delete p;
}

void CMMDlg::cutCS()//将一个CS分割为两个CS
{
	if (!pActiveCS)
	{
		MessageBox("请选择一个物体！");
	}
	else
	{
		//计算knife的信息
		GLdouble mvMatrix[16];
		GLdouble wx1,wy1,wz1,wx2,wy2,wz2,wx3,wy3,wz3,wx4,wy4,wz4;
		GLdouble vo[3],vn[3],vx[3],vy[3];//vo是中心点的坐标，vn是法向量（不一定是单位向量）
		GLdouble dx=m_knife.dx*m_knife.size/pActiveCS->m_size;
		GLdouble dy=m_knife.dy*m_knife.size/pActiveCS->m_size;//大小
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixd(m_modelMatrix[0]);//加载视角
		glPushMatrix();
		glTranslated(m_knife.t[0],m_knife.t[1],m_knife.t[2]);
		glMultMatrixd(m_knife.rotateMatrix);
		glScaled(m_knife.size,m_knife.size,m_knife.size);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);//取得knife的视图模型矩阵
		gluProject(0,0,0,mvMatrix,m_projMatrix[0],m_port[0],&wx1,&wy1,&wz1);
		gluProject(0,0,1,mvMatrix,m_projMatrix[0],m_port[0],&wx2,&wy2,&wz2);
		gluProject(m_knife.dx,0,0,mvMatrix,m_projMatrix[0],m_port[0],&wx3,&wy3,&wz3);
		gluProject(0,m_knife.dy,0,mvMatrix,m_projMatrix[0],m_port[0],&wx4,&wy4,&wz4);
		glPopMatrix();
		glTranslated(pActiveCS->m_translate[0],pActiveCS->m_translate[1],pActiveCS->m_translate[2]);
		glMultMatrixd(pActiveCS->m_mvMatrix);
		glScaled(pActiveCS->m_size,pActiveCS->m_size,pActiveCS->m_size);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);//取得activeCS的视图模型矩阵
		gluUnProject(wx1,wy1,wz1,mvMatrix,m_projMatrix[0],m_port[0],&vo[0],&vo[1],&vo[2]);
		gluUnProject(wx2,wy2,wz2,mvMatrix,m_projMatrix[0],m_port[0],&vn[0],&vn[1],&vn[2]);
		gluUnProject(wx3,wy3,wz3,mvMatrix,m_projMatrix[0],m_port[0],&vx[0],&vx[1],&vx[2]);
		gluUnProject(wx4,wy4,wz4,mvMatrix,m_projMatrix[0],m_port[0],&vy[0],&vy[1],&vy[2]);
		vn[0]=vn[0]-vo[0];
		vn[1]=vn[1]-vo[1];
		vn[2]=vn[2]-vo[2];
		vx[0]=vx[0]-vo[0];
		vx[1]=vx[1]-vo[1];
		vx[2]=vx[2]-vo[2];
		vy[0]=vy[0]-vo[0];
		vy[1]=vy[1]-vo[1];
		vy[2]=vy[2]-vo[2];
		glPopMatrix();
		//初始化边链
		Edge *ehead=new Edge;
		ehead->next=NULL;
		Edge *pe=ehead;
		//新建两个CS
		CS *pCS1=new CS;
		CS *pCS2=new CS;
		Triangle *pt1=pCS1->it;
		Triangle *pt2=pCS2->it;
		CSearch search1,search2;//储存索引顶点
		//遍历pActiveCS所指的三角网，判断三角形和knife相交情况
		bool deleted=0;
		GLdouble r1,r2,r3,type=0;
		GLdouble v[3][3];//三角形三个顶点
		Triangle *q=pActiveCS->it,*p;
		while(q->next)
		{
			p=q->next;
			deleted=0;
			r1=(p->pt[0].v[0]-vo[0])*vn[0]+(p->pt[0].v[1]-vo[1])*vn[1]+(p->pt[0].v[2]-vo[2])*vn[2];
			r2=(p->pt[1].v[0]-vo[0])*vn[0]+(p->pt[1].v[1]-vo[1])*vn[1]+(p->pt[1].v[2]-vo[2])*vn[2];
			r3=(p->pt[2].v[0]-vo[0])*vn[0]+(p->pt[2].v[1]-vo[1])*vn[1]+(p->pt[2].v[2]-vo[2])*vn[2];
			//相交情况分类,但尚未判断交点是否落在knife上
			if (r1>0&&r2>0&&r3>0||r1<0&&r2<0&&r3<0) type=0;
			else if(r1*r2*r3!=0) //没有顶点在knife上
			{
				if (r1>0&&r2>0&&r3<0)
				{
					v[0][0]=p->pt[0].v[0];
					v[0][1]=p->pt[0].v[1];
					v[0][2]=p->pt[0].v[2];
					v[1][0]=p->pt[1].v[0];
					v[1][1]=p->pt[1].v[1];
					v[1][2]=p->pt[1].v[2];
					v[2][0]=p->pt[2].v[0];
					v[2][1]=p->pt[2].v[1];
					v[2][2]=p->pt[2].v[2];
					search1.push(p->pt[2].vr);
					search2.push(p->pt[0].vr);
					search2.push(p->pt[1].vr);
					type=1;
				}
				else if (r1>0&&r2<0&&r3>0)
				{
					v[0][0]=p->pt[2].v[0];
					v[0][1]=p->pt[2].v[1];
					v[0][2]=p->pt[2].v[2];
					v[1][0]=p->pt[0].v[0];
					v[1][1]=p->pt[0].v[1];
					v[1][2]=p->pt[0].v[2];
					v[2][0]=p->pt[1].v[0];
					v[2][1]=p->pt[1].v[1];
					v[2][2]=p->pt[1].v[2];
					search1.push(p->pt[1].vr);
					search2.push(p->pt[0].vr);
					search2.push(p->pt[2].vr);
					type=1;
				}
				else if (r1<0&&r2>0&&r3>0)
				{
					v[0][0]=p->pt[1].v[0];
					v[0][1]=p->pt[1].v[1];
					v[0][2]=p->pt[1].v[2];
					v[1][0]=p->pt[2].v[0];
					v[1][1]=p->pt[2].v[1];
					v[1][2]=p->pt[2].v[2];
					v[2][0]=p->pt[0].v[0];
					v[2][1]=p->pt[0].v[1];
					v[2][2]=p->pt[0].v[2];
					search1.push(p->pt[0].vr);
					search2.push(p->pt[1].vr);
					search2.push(p->pt[2].vr);
					type=1;
				}
				else if (r1>0&&r2<0&&r3<0)
				{
					v[0][0]=p->pt[0].v[0];
					v[0][1]=p->pt[0].v[1];
					v[0][2]=p->pt[0].v[2];
					v[1][0]=p->pt[1].v[0];
					v[1][1]=p->pt[1].v[1];
					v[1][2]=p->pt[1].v[2];
					v[2][0]=p->pt[2].v[0];
					v[2][1]=p->pt[2].v[1];
					v[2][2]=p->pt[2].v[2];
					search1.push(p->pt[1].vr);
					search1.push(p->pt[2].vr);
					search2.push(p->pt[0].vr);
					type=2;
				}
				else if (r1<0&&r2>0&&r3<0)
				{
					v[0][0]=p->pt[1].v[0];
					v[0][1]=p->pt[1].v[1];
					v[0][2]=p->pt[1].v[2];
					v[1][0]=p->pt[2].v[0];
					v[1][1]=p->pt[2].v[1];
					v[1][2]=p->pt[2].v[2];
					v[2][0]=p->pt[0].v[0];
					v[2][1]=p->pt[0].v[1];
					v[2][2]=p->pt[0].v[2];
					search1.push(p->pt[0].vr);
					search1.push(p->pt[2].vr);
					search2.push(p->pt[1].vr);
					type=2;
				}
				else if (r1<0&&r2<0&&r3>0)
				{
					v[0][0]=p->pt[2].v[0];
					v[0][1]=p->pt[2].v[1];
					v[0][2]=p->pt[2].v[2];
					v[1][0]=p->pt[0].v[0];
					v[1][1]=p->pt[0].v[1];
					v[1][2]=p->pt[0].v[2];
					v[2][0]=p->pt[1].v[0];
					v[2][1]=p->pt[1].v[1];
					v[2][2]=p->pt[1].v[2];
					search1.push(p->pt[0].vr);
					search1.push(p->pt[1].vr);
					search2.push(p->pt[2].vr);
					type=2;
				}
			}
			else //有顶点在knife上，概率较小
			if (r1>0&&r2==0&&r3<0)
			{
				v[0][0]=p->pt[0].v[0];
				v[0][1]=p->pt[0].v[1];
				v[0][2]=p->pt[0].v[2];
				v[1][0]=p->pt[1].v[0];
				v[1][1]=p->pt[1].v[1];
				v[1][2]=p->pt[1].v[2];
				v[2][0]=p->pt[2].v[0];
				v[2][1]=p->pt[2].v[1];
				v[2][2]=p->pt[2].v[2];
				search1.push(p->pt[2].vr);
				search2.push(p->pt[0].vr);
				type=3;
			}
			else if (r1==0&&r2<0&&r3>0)
			{
				v[0][0]=p->pt[2].v[0];
				v[0][1]=p->pt[2].v[1];
				v[0][2]=p->pt[2].v[2];
				v[1][0]=p->pt[0].v[0];
				v[1][1]=p->pt[0].v[1];
				v[1][2]=p->pt[0].v[2];
				v[2][0]=p->pt[1].v[0];
				v[2][1]=p->pt[1].v[1];
				v[2][2]=p->pt[1].v[2];
				search1.push(p->pt[1].vr);
				search2.push(p->pt[2].vr);
				type=3;
			}
			else if (r1<0&&r2>0&&r3==0)
			{
				v[0][0]=p->pt[1].v[0];
				v[0][1]=p->pt[1].v[1];
				v[0][2]=p->pt[1].v[2];
				v[1][0]=p->pt[2].v[0];
				v[1][1]=p->pt[2].v[1];
				v[1][2]=p->pt[2].v[2];
				v[2][0]=p->pt[0].v[0];
				v[2][1]=p->pt[0].v[1];
				v[2][2]=p->pt[0].v[2];
				search1.push(p->pt[0].vr);
				search2.push(p->pt[1].vr);
				type=3;
			}
			else if (r1<0&&r2==0&&r3>0)
			{
				v[0][0]=p->pt[2].v[0];
				v[0][1]=p->pt[2].v[1];
				v[0][2]=p->pt[2].v[2];
				v[1][0]=p->pt[0].v[0];
				v[1][1]=p->pt[0].v[1];
				v[1][2]=p->pt[0].v[2];
				v[2][0]=p->pt[1].v[0];
				v[2][1]=p->pt[1].v[1];
				v[2][2]=p->pt[1].v[2];
				search1.push(p->pt[0].vr);
				search2.push(p->pt[2].vr);
				type=4;
			}
			else if (r1>0&&r2<0&&r3==0)
			{
				v[0][0]=p->pt[0].v[0];
				v[0][1]=p->pt[0].v[1];
				v[0][2]=p->pt[0].v[2];
				v[1][0]=p->pt[1].v[0];
				v[1][1]=p->pt[1].v[1];
				v[1][2]=p->pt[1].v[2];
				v[2][0]=p->pt[2].v[0];
				v[2][1]=p->pt[2].v[1];
				v[2][2]=p->pt[2].v[2];
				search1.push(p->pt[1].vr);
				search2.push(p->pt[0].vr);
				type=4;
			}
			else if (r1==0&&r2>0&&r3<0)
			{
				v[0][0]=p->pt[1].v[0];
				v[0][1]=p->pt[1].v[1];
				v[0][2]=p->pt[1].v[2];
				v[1][0]=p->pt[2].v[0];
				v[1][1]=p->pt[2].v[1];
				v[1][2]=p->pt[2].v[2];
				v[2][0]=p->pt[0].v[0];
				v[2][1]=p->pt[0].v[1];
				v[2][2]=p->pt[0].v[2];
				search1.push(p->pt[2].vr);
				search2.push(p->pt[1].vr);
				type=4;
			}
			else if (r1>0&&r2>0&&r3==0)
			{
				v[0][0]=p->pt[1].v[0];
				v[0][1]=p->pt[1].v[1];
				v[0][2]=p->pt[1].v[2];
				v[1][0]=p->pt[2].v[0];
				v[1][1]=p->pt[2].v[1];
				v[1][2]=p->pt[2].v[2];
				v[2][0]=p->pt[0].v[0];
				v[2][1]=p->pt[0].v[1];
				v[2][2]=p->pt[0].v[2];
				search2.push(p->pt[0].vr);
				search2.push(p->pt[1].vr);
				type=5;
			}
			else if (r1>0&&r2==0&&r3>0)
			{
				v[0][0]=p->pt[0].v[0];
				v[0][1]=p->pt[0].v[1];
				v[0][2]=p->pt[0].v[2];
				v[1][0]=p->pt[1].v[0];
				v[1][1]=p->pt[1].v[1];
				v[1][2]=p->pt[1].v[2];
				v[2][0]=p->pt[2].v[0];
				v[2][1]=p->pt[2].v[1];
				v[2][2]=p->pt[2].v[2];
				search2.push(p->pt[0].vr);
				search2.push(p->pt[2].vr);
				type=5;
			}
			else if (r1==0&&r2>0&&r3>0)
			{
				v[0][0]=p->pt[2].v[0];
				v[0][1]=p->pt[2].v[1];
				v[0][2]=p->pt[2].v[2];
				v[1][0]=p->pt[0].v[0];
				v[1][1]=p->pt[0].v[1];
				v[1][2]=p->pt[0].v[2];
				v[2][0]=p->pt[1].v[0];
				v[2][1]=p->pt[1].v[1];
				v[2][2]=p->pt[1].v[2];
				search2.push(p->pt[1].vr);
				search2.push(p->pt[2].vr);
				type=5;
			}
			else if (r1==0&&r2<0&&r3<0)
			{
				v[0][0]=p->pt[0].v[0];
				v[0][1]=p->pt[0].v[1];
				v[0][2]=p->pt[0].v[2];
				v[1][0]=p->pt[1].v[0];
				v[1][1]=p->pt[1].v[1];
				v[1][2]=p->pt[1].v[2];
				v[2][0]=p->pt[2].v[0];
				v[2][1]=p->pt[2].v[1];
				v[2][2]=p->pt[2].v[2];
				search1.push(p->pt[1].vr);
				search1.push(p->pt[2].vr);
				type=6;
			}
			else if (r1<0&&r2==0&&r3<0)
			{
				v[0][0]=p->pt[1].v[0];
				v[0][1]=p->pt[1].v[1];
				v[0][2]=p->pt[1].v[2];
				v[1][0]=p->pt[2].v[0];
				v[1][1]=p->pt[2].v[1];
				v[1][2]=p->pt[2].v[2];
				v[2][0]=p->pt[0].v[0];
				v[2][1]=p->pt[0].v[1];
				v[2][2]=p->pt[0].v[2];
				search1.push(p->pt[0].vr);
				search1.push(p->pt[2].vr);
				type=6;
			}
			else if (r1<0&&r2<0&&r3==0)
			{
				v[0][0]=p->pt[2].v[0];
				v[0][1]=p->pt[2].v[1];
				v[0][2]=p->pt[2].v[2];
				v[1][0]=p->pt[0].v[0];
				v[1][1]=p->pt[0].v[1];
				v[1][2]=p->pt[0].v[2];
				v[2][0]=p->pt[1].v[0];
				v[2][1]=p->pt[1].v[1];
				v[2][2]=p->pt[1].v[2];
				search1.push(p->pt[0].vr);
				search1.push(p->pt[1].vr);
				type=6;
			}
			else if (r1>0&&r2==0&&r3==0)
			{
				v[0][0]=p->pt[0].v[0];
				v[0][1]=p->pt[0].v[1];
				v[0][2]=p->pt[0].v[2];
				v[1][0]=p->pt[1].v[0];
				v[1][1]=p->pt[1].v[1];
				v[1][2]=p->pt[1].v[2];
				v[2][0]=p->pt[2].v[0];
				v[2][1]=p->pt[2].v[1];
				v[2][2]=p->pt[2].v[2];
				search2.push(p->pt[0].vr);
				type=7;
			}
			else if (r1==0&&r2>0&&r3==0)
			{
				v[0][0]=p->pt[1].v[0];
				v[0][1]=p->pt[1].v[1];
				v[0][2]=p->pt[1].v[2];
				v[1][0]=p->pt[2].v[0];
				v[1][1]=p->pt[2].v[1];
				v[1][2]=p->pt[2].v[2];
				v[2][0]=p->pt[0].v[0];
				v[2][1]=p->pt[0].v[1];
				v[2][2]=p->pt[0].v[2];
				search2.push(p->pt[1].vr);
				type=7;
			}
			else if (r1==0&&r2==0&&r3>0)
			{
				v[0][0]=p->pt[2].v[0];
				v[0][1]=p->pt[2].v[1];
				v[0][2]=p->pt[2].v[2];
				v[1][0]=p->pt[0].v[0];
				v[1][1]=p->pt[0].v[1];
				v[1][2]=p->pt[0].v[2];
				v[2][0]=p->pt[1].v[0];
				v[2][1]=p->pt[1].v[1];
				v[2][2]=p->pt[1].v[2];
				search2.push(p->pt[2].vr);
				type=7;
			}
			else if (r1==0&&r2<0&&r3==0)
			{
				v[0][0]=p->pt[0].v[0];
				v[0][1]=p->pt[0].v[1];
				v[0][2]=p->pt[0].v[2];
				v[1][0]=p->pt[1].v[0];
				v[1][1]=p->pt[1].v[1];
				v[1][2]=p->pt[1].v[2];
				v[2][0]=p->pt[2].v[0];
				v[2][1]=p->pt[2].v[1];
				v[2][2]=p->pt[2].v[2];
				search1.push(p->pt[1].vr);
				type=8;
			}
			else if (r1==0&&r2==0&&r3<0)
			{
				v[0][0]=p->pt[1].v[0];
				v[0][1]=p->pt[1].v[1];
				v[0][2]=p->pt[1].v[2];
				v[1][0]=p->pt[2].v[0];
				v[1][1]=p->pt[2].v[1];
				v[1][2]=p->pt[2].v[2];
				v[2][0]=p->pt[0].v[0];
				v[2][1]=p->pt[0].v[1];
				v[2][2]=p->pt[0].v[2];
				search1.push(p->pt[2].vr);
				type=8;
			}
			else if (r1<0&&r2==0&&r3==0)
			{
				v[0][0]=p->pt[2].v[0];
				v[0][1]=p->pt[2].v[1];
				v[0][2]=p->pt[2].v[2];
				v[1][0]=p->pt[0].v[0];
				v[1][1]=p->pt[0].v[1];
				v[1][2]=p->pt[0].v[2];
				v[2][0]=p->pt[1].v[0];
				v[2][1]=p->pt[1].v[1];
				v[2][2]=p->pt[1].v[2];
				search1.push(p->pt[0].vr);
				type=8;
			}
			else 
			{
				MessageBox("切割出错！\n原因：有三角形全部顶点都在刀面上！");
				return;
			}
			//到此分类结束，分别进行处理
			GLdouble k;
			GLdouble ve[2][3];//边的两个端点
			GLdouble kx,ky;
			if (type==1)
			{
				k=((v[1][0]-vo[0])*vn[0]+(v[1][1]-vo[1])*vn[1]+(v[1][2]-vo[2])*vn[2])/((v[1][0]-v[2][0])*vn[0]+(v[1][1]-v[2][1])*vn[1]+(v[1][2]-v[2][2])*vn[2]);
				ve[0][0]=(1-k)*v[1][0]+k*v[2][0]-vo[0];	
				ve[0][1]=(1-k)*v[1][1]+k*v[2][1]-vo[1];
				ve[0][2]=(1-k)*v[1][2]+k*v[2][2]-vo[2];//OD	
				kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
				ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)//在刀面上才进行下一步
				{
					k=((v[0][0]-vo[0])*vn[0]+(v[0][1]-vo[1])*vn[1]+(v[0][2]-vo[2])*vn[2])/((v[0][0]-v[2][0])*vn[0]+(v[0][1]-v[2][1])*vn[1]+(v[0][2]-v[2][2])*vn[2]);
					ve[1][0]=(1-k)*v[0][0]+k*v[2][0]-vo[0];	
					ve[1][1]=(1-k)*v[0][1]+k*v[2][1]-vo[1];
					ve[1][2]=(1-k)*v[0][2]+k*v[2][2]-vo[2];//OE
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //两点都在刀面上,压入边，去掉和写入三角形
					{
						ve[0][0]+=vo[0];
						ve[0][1]+=vo[1];
						ve[0][2]+=vo[2];
						ve[1][0]+=vo[0];
						ve[1][1]+=vo[1];
						ve[1][2]+=vo[2];
						//写入DE边
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=ve[0][0];
						pe->y1=ve[0][1];
						pe->z1=ve[0][2];
						pe->x2=ve[1][0];
						pe->y2=ve[1][1];
						pe->z2=ve[1][2];
						//压入新三角形
						pt1->next=new Triangle;
						pt1=pt1->next;
						pt1->next=NULL;
						pt1->pt[0].v[0]=v[2][0];
						pt1->pt[0].v[1]=v[2][1];
						pt1->pt[0].v[2]=v[2][2];
						pt1->pt[1].v[0]=ve[1][0];
						pt1->pt[1].v[1]=ve[1][1];
						pt1->pt[1].v[2]=ve[1][2];
						pt1->pt[2].v[0]=ve[0][0];
						pt1->pt[2].v[1]=ve[0][1];
						pt1->pt[2].v[2]=ve[0][2];

						pt2->next=new Triangle;
						pt2=pt2->next;
						pt2->pt[0].v[0]=ve[1][0];
						pt2->pt[0].v[1]=ve[1][1];
						pt2->pt[0].v[2]=ve[1][2];
						pt2->pt[1].v[0]=(v[0][0]+v[1][0])/2;
						pt2->pt[1].v[1]=(v[0][1]+v[1][1])/2;
						pt2->pt[1].v[2]=(v[0][2]+v[1][2])/2;
						pt2->pt[2].v[0]=ve[0][0];
						pt2->pt[2].v[1]=ve[0][1];
						pt2->pt[2].v[2]=ve[0][2];

						pt2->next=new Triangle;
						pt2=pt2->next;
						pt2->pt[0].v[0]=v[0][0];
						pt2->pt[0].v[1]=v[0][1];
						pt2->pt[0].v[2]=v[0][2];
						pt2->pt[1].v[0]=(v[0][0]+v[1][0])/2;
						pt2->pt[1].v[1]=(v[0][1]+v[1][1])/2;
						pt2->pt[1].v[2]=(v[0][2]+v[1][2])/2;
						pt2->pt[2].v[0]=ve[1][0];
						pt2->pt[2].v[1]=ve[1][1];
						pt2->pt[2].v[2]=ve[1][2];

						pt2->next=new Triangle;
						pt2=pt2->next;
						pt2->pt[0].v[0]=ve[0][0];
						pt2->pt[0].v[1]=ve[0][1];
						pt2->pt[0].v[2]=ve[0][2];
						pt2->pt[1].v[0]=(v[0][0]+v[1][0])/2;
						pt2->pt[1].v[1]=(v[0][1]+v[1][1])/2;
						pt2->pt[1].v[2]=(v[0][2]+v[1][2])/2;
						pt2->pt[2].v[0]=v[1][0];
						pt2->pt[2].v[1]=v[1][1];
						pt2->pt[2].v[2]=v[1][2];
						pt2->next=NULL;
						//删掉p=q->next所指向的三角形及辅助顶点
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("切割出错！\n原因：三角形不完全切割！");
						return;
					}
				}
			}
			else if (type==2)
			{
				k=((v[0][0]-vo[0])*vn[0]+(v[0][1]-vo[1])*vn[1]+(v[0][2]-vo[2])*vn[2])/((v[0][0]-v[1][0])*vn[0]+(v[0][1]-v[1][1])*vn[1]+(v[0][2]-v[1][2])*vn[2]);
				ve[0][0]=(1-k)*v[0][0]+k*v[1][0]-vo[0];	
				ve[0][1]=(1-k)*v[0][1]+k*v[1][1]-vo[1];
				ve[0][2]=(1-k)*v[0][2]+k*v[1][2]-vo[2];//OD	
				kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
				ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)//在刀面上才进行下一步
				{
					k=((v[0][0]-vo[0])*vn[0]+(v[0][1]-vo[1])*vn[1]+(v[0][2]-vo[2])*vn[2])/((v[0][0]-v[2][0])*vn[0]+(v[0][1]-v[2][1])*vn[1]+(v[0][2]-v[2][2])*vn[2]);
					ve[1][0]=(1-k)*v[0][0]+k*v[2][0]-vo[0];	
					ve[1][1]=(1-k)*v[0][1]+k*v[2][1]-vo[1];
					ve[1][2]=(1-k)*v[0][2]+k*v[2][2]-vo[2];//OE
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //两点都在刀面上,压入边，去掉和写入三角形
					{
						ve[0][0]+=vo[0];
						ve[0][1]+=vo[1];
						ve[0][2]+=vo[2];
						ve[1][0]+=vo[0];
						ve[1][1]+=vo[1];
						ve[1][2]+=vo[2];
						//写入DE边
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=ve[0][0];
						pe->y1=ve[0][1];
						pe->z1=ve[0][2];
						pe->x2=ve[1][0];
						pe->y2=ve[1][1];
						pe->z2=ve[1][2];
						//压入新三角形
						pt1->next=new Triangle;
						pt1=pt1->next;
						pt1->pt[0].v[0]=(v[1][0]+v[2][0])/2;
						pt1->pt[0].v[1]=(v[1][1]+v[2][1])/2;
						pt1->pt[0].v[2]=(v[1][2]+v[2][2])/2;
						pt1->pt[1].v[0]=ve[1][0];
						pt1->pt[1].v[1]=ve[1][1];
						pt1->pt[1].v[2]=ve[1][2];
						pt1->pt[2].v[0]=ve[0][0];
						pt1->pt[2].v[1]=ve[0][1];
						pt1->pt[2].v[2]=ve[0][2];

						pt1->next=new Triangle;
						pt1=pt1->next;
						pt1->pt[0].v[0]=(v[1][0]+v[2][0])/2;
						pt1->pt[0].v[1]=(v[1][1]+v[2][1])/2;
						pt1->pt[0].v[2]=(v[1][2]+v[2][2])/2;
						pt1->pt[1].v[0]=ve[0][0];
						pt1->pt[1].v[1]=ve[0][1];
						pt1->pt[1].v[2]=ve[0][2];
						pt1->pt[2].v[0]=v[1][0];
						pt1->pt[2].v[1]=v[1][1];
						pt1->pt[2].v[2]=v[1][2];
						
						pt1->next=new Triangle;
						pt1=pt1->next;
						pt1->pt[0].v[0]=(v[1][0]+v[2][0])/2;
						pt1->pt[0].v[1]=(v[1][1]+v[2][1])/2;
						pt1->pt[0].v[2]=(v[1][2]+v[2][2])/2;
						pt1->pt[1].v[0]=v[2][0];
						pt1->pt[1].v[1]=v[2][1];
						pt1->pt[1].v[2]=v[2][2];
						pt1->pt[2].v[0]=ve[1][0];
						pt1->pt[2].v[1]=ve[1][1];
						pt1->pt[2].v[2]=ve[1][2];
						pt1->next=NULL;

						pt2->next=new Triangle;
						pt2=pt2->next;
						pt2->pt[0].v[0]=ve[1][0];
						pt2->pt[0].v[1]=ve[1][1];
						pt2->pt[0].v[2]=ve[1][2];
						pt2->pt[1].v[0]=v[0][0];
						pt2->pt[1].v[1]=v[0][1];
						pt2->pt[1].v[2]=v[0][2];
						pt2->pt[2].v[0]=ve[0][0];
						pt2->pt[2].v[1]=ve[0][1];
						pt2->pt[2].v[2]=ve[0][2];
						pt2->next=NULL;
						//删掉p=q->next所指向的三角形及辅助顶点
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("切割出错！\n原因：三角形不完全切割！");
						return;
					}
				}
			}
			else if (type==3)
			{
				ve[0][0]=v[1][0]-vo[0];
				ve[0][1]=v[1][1]-vo[1];
				ve[0][2]=v[1][2]-vo[2];
				kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
				ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)//在刀面上才进行下一步
				{
					k=((v[0][0]-vo[0])*vn[0]+(v[0][1]-vo[1])*vn[1]+(v[0][2]-vo[2])*vn[2])/((v[0][0]-v[2][0])*vn[0]+(v[0][1]-v[2][1])*vn[1]+(v[0][2]-v[2][2])*vn[2]);
					ve[1][0]=(1-k)*v[0][0]+k*v[2][0]-vo[0];	
					ve[1][1]=(1-k)*v[0][1]+k*v[2][1]-vo[1];
					ve[1][2]=(1-k)*v[0][2]+k*v[2][2]-vo[2];//OE
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //两点都在刀面上,压入边，去掉和写入三角形
					{
						ve[0][0]+=vo[0];
						ve[0][1]+=vo[1];
						ve[0][2]+=vo[2];
						ve[1][0]+=vo[0];
						ve[1][1]+=vo[1];
						ve[1][2]+=vo[2];
						//写入DE边
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=ve[0][0];
						pe->y1=ve[0][1];
						pe->z1=ve[0][2];
						pe->x2=ve[1][0];
						pe->y2=ve[1][1];
						pe->z2=ve[1][2];
						//压入新三角形
						pt1->next=new Triangle;
						pt1=pt1->next;
						pt1->pt[0].v[0]=v[2][0];
						pt1->pt[0].v[1]=v[2][1];
						pt1->pt[0].v[2]=v[2][2];
						pt1->pt[1].v[0]=ve[1][0];
						pt1->pt[1].v[1]=ve[1][1];
						pt1->pt[1].v[2]=ve[1][2];
						pt1->pt[2].v[0]=ve[0][0];
						pt1->pt[2].v[1]=ve[0][1];
						pt1->pt[2].v[2]=ve[0][2];
						pt1->next=NULL;

						pt2->next=new Triangle;
						pt2=pt2->next;
						pt2->pt[0].v[0]=ve[1][0];
						pt2->pt[0].v[1]=ve[1][1];
						pt2->pt[0].v[2]=ve[1][2];
						pt2->pt[1].v[0]=v[0][0];
						pt2->pt[1].v[1]=v[0][1];
						pt2->pt[1].v[2]=v[0][2];
						pt2->pt[2].v[0]=ve[0][0];
						pt2->pt[2].v[1]=ve[0][1];
						pt2->pt[2].v[2]=ve[0][2];
						pt2->next=NULL;
						//删掉p=q->next所指向的三角形及辅助顶点
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("切割出错！\n原因：三角形不完全切割！");
						return;
					}
				}
			}
			else if (type==4)
			{
				ve[1][0]=v[2][0]-vo[0];
				ve[1][1]=v[2][1]-vo[1];
				ve[1][2]=v[2][2]-vo[2];
				kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
				ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)//在刀面上才进行下一步
				{
					k=((v[0][0]-vo[0])*vn[0]+(v[0][1]-vo[1])*vn[1]+(v[0][2]-vo[2])*vn[2])/((v[0][0]-v[1][0])*vn[0]+(v[0][1]-v[1][1])*vn[1]+(v[0][2]-v[1][2])*vn[2]);
					ve[0][0]=(1-k)*v[0][0]+k*v[1][0]-vo[0];	
					ve[0][1]=(1-k)*v[0][1]+k*v[1][1]-vo[1];
					ve[0][2]=(1-k)*v[0][2]+k*v[1][2]-vo[2];//OE
					kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //两点都在刀面上,压入边，去掉和写入三角形
					{
						ve[0][0]+=vo[0];
						ve[0][1]+=vo[1];
						ve[0][2]+=vo[2];
						ve[1][0]+=vo[0];
						ve[1][1]+=vo[1];
						ve[1][2]+=vo[2];
						//写入DE边
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=ve[0][0];
						pe->y1=ve[0][1];
						pe->z1=ve[0][2];
						pe->x2=ve[1][0];
						pe->y2=ve[1][1];
						pe->z2=ve[1][2];
						//压入新三角形
						pt1->next=new Triangle;
						pt1=pt1->next;
						pt1->pt[0].v[0]=v[1][0];
						pt1->pt[0].v[1]=v[1][1];
						pt1->pt[0].v[2]=v[1][2];
						pt1->pt[1].v[0]=ve[1][0];
						pt1->pt[1].v[1]=ve[1][1];
						pt1->pt[1].v[2]=ve[1][2];
						pt1->pt[2].v[0]=ve[0][0];
						pt1->pt[2].v[1]=ve[0][1];
						pt1->pt[2].v[2]=ve[0][2];
						pt1->next=NULL;
						
						pt2->next=new Triangle;
						pt2=pt2->next;
						pt2->pt[0].v[0]=ve[1][0];
						pt2->pt[0].v[1]=ve[1][1];
						pt2->pt[0].v[2]=ve[1][2];
						pt2->pt[1].v[0]=v[0][0];
						pt2->pt[1].v[1]=v[0][1];
						pt2->pt[1].v[2]=v[0][2];
						pt2->pt[2].v[0]=ve[0][0];
						pt2->pt[2].v[1]=ve[0][1];
						pt2->pt[2].v[2]=ve[0][2];
						pt2->next=NULL;
						//删掉p=q->next所指向的三角形及辅助顶点
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("切割出错！\n原因：三角形不完全切割！");
						return;
					}
				}
			}
			else if (type==5)
			{
				ve[0][0]=v[1][0]-vo[0];
				ve[0][1]=v[1][1]-vo[1];
				ve[0][2]=v[1][2]-vo[2];
				kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
				ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) 
				{
					pt2->next=new Triangle;
					pt2=pt2->next;
					pt2->pt[0].v[0]=v[0][0];
					pt2->pt[0].v[1]=v[0][1];
					pt2->pt[0].v[2]=v[0][2];
					pt2->pt[1].v[0]=v[1][0];
					pt2->pt[1].v[1]=v[1][1];
					pt2->pt[1].v[2]=v[1][2];
					pt2->pt[2].v[0]=v[2][0];
					pt2->pt[2].v[1]=v[2][1];
					pt2->pt[2].v[2]=v[2][2];
					pt2->next=NULL;
					//删掉p=q->next所指向的三角形及辅助顶点
					deleteTriangle(q,pActiveCS,search1,search2);
					deleted=1;
				}			
			}
			else if(type==6)
			{
				ve[0][0]=v[0][0]-vo[0];
				ve[0][1]=v[0][1]-vo[1];
				ve[0][2]=v[0][2]-vo[2];
				kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
				ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) 
				{
					pt1->next=new Triangle;
					pt1=pt1->next;
					pt1->pt[0].v[0]=v[0][0];
					pt1->pt[0].v[1]=v[0][1];
					pt1->pt[0].v[2]=v[0][2];
					pt1->pt[1].v[0]=v[1][0];
					pt1->pt[1].v[1]=v[1][1];
					pt1->pt[1].v[2]=v[1][2];
					pt1->pt[2].v[0]=v[2][0];
					pt1->pt[2].v[1]=v[2][1];
					pt1->pt[2].v[2]=v[2][2];
					pt1->next=NULL;
					//删掉p=q->next所指向的三角形及辅助顶点
					deleteTriangle(q,pActiveCS,search1,search2);
					deleted=1;
				}
			}
			else if(type==6)
			{
				ve[0][0]=v[1][0]-vo[0];
				ve[0][1]=v[1][1]-vo[1];
				ve[0][2]=v[1][2]-vo[2];
				kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
				ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //在刀面上才进行下一步
				{
					ve[1][0]=v[2][0]-vo[0];
					ve[1][1]=v[2][1]-vo[1];
					ve[1][2]=v[2][2]-vo[2];
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)
					{
						//写入DE边
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=v[1][0];
						pe->y1=v[1][1];
						pe->z1=v[1][2];
						pe->x2=v[2][0];
						pe->y2=v[2][1];
						pe->z2=v[2][2];
						//写入新三角形
						pt2->next=new Triangle;
						pt2=pt2->next;
						pt2->pt[0].v[0]=v[0][0];
						pt2->pt[0].v[1]=v[0][1];
						pt2->pt[0].v[2]=v[0][2];
						pt2->pt[1].v[0]=v[1][0];
						pt2->pt[1].v[1]=v[1][1];
						pt2->pt[1].v[2]=v[1][2];
						pt2->pt[2].v[0]=v[2][0];
						pt2->pt[2].v[1]=v[2][1];
						pt2->pt[2].v[2]=v[2][2];
						pt2->next=NULL;
						//删掉p=q->next所指向的三角形及辅助顶点
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("切割出错！\n原因：三角形不完全切割！");
						return;
					}
				}
			}
			else if (type==8)
			{
				ve[0][0]=v[0][0]-vo[0];
				ve[0][1]=v[0][1]-vo[1];
				ve[0][2]=v[0][2]-vo[2];
				kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
				ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //在刀面上才进行下一步
				{
					ve[1][0]=v[2][0]-vo[0];
					ve[1][1]=v[2][1]-vo[1];
					ve[1][2]=v[2][2]-vo[2];
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)
					{
						//写入DE边
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=v[0][0];
						pe->y1=v[0][1];
						pe->z1=v[0][2];
						pe->x2=v[2][0];
						pe->y2=v[2][1];
						pe->z2=v[2][2];
						//写入新三角形
						pt1->next=new Triangle;
						pt1=pt1->next;
						pt1->pt[0].v[0]=v[0][0];
						pt1->pt[0].v[1]=v[0][1];
						pt1->pt[0].v[2]=v[0][2];
						pt1->pt[1].v[0]=v[1][0];
						pt1->pt[1].v[1]=v[1][1];
						pt1->pt[1].v[2]=v[1][2];
						pt1->pt[2].v[0]=v[2][0];
						pt1->pt[2].v[1]=v[2][1];
						pt1->pt[2].v[2]=v[2][2];
						pt1->next=NULL;
						//删掉p=q->next所指向的三角形及辅助顶点
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("切割出错！\n原因：三角形不完全切割！");
						return;
					}
				}
			}
			//未删除p,则q向后移动
			if(!deleted) q=q->next;
		}
		//将pActiveCS余下的三角形的访问标记置0
		Triangle *pt=pActiveCS->it;
		while(pt->next)
		{
			pt=pt->next;
			pt->visit=0;
		}
		VertexRef *vr=pActiveCS->iv;
		while(vr->next)
		{
			vr=vr->next;
			vr->visit=0;
		}
		//开始节点搜索pActiveCS余下的三角形，分为pCS1和pCS2
		TriangleRef *tr=NULL;
		CQueue queue;//开始队列
		if (search1.shead->next)//有可用索引才进行搜索
		{
			queue.enQueue(search1.shead->next->pv);
			search1.shead->next->pv->visit=1;//第一个节点被访问了
			while (queue.num) //队列不空时
			{
				vr=queue.deQueue();
				tr=vr->nextRef;
				while(tr)
				{
					if(!tr->t->visit)//未被访问，则将其复制到pCS1所指的三角形中
					{
						tr->t->visit=1;
						pt1->next=new Triangle;
						pt1=pt1->next;
						pt1->pt[0].v[0]=tr->t->pt[0].v[0];
						pt1->pt[0].v[1]=tr->t->pt[0].v[1];
						pt1->pt[0].v[2]=tr->t->pt[0].v[2];
						pt1->pt[1].v[0]=tr->t->pt[1].v[0];
						pt1->pt[1].v[1]=tr->t->pt[1].v[1];
						pt1->pt[1].v[2]=tr->t->pt[1].v[2];
						pt1->pt[2].v[0]=tr->t->pt[2].v[0];
						pt1->pt[2].v[1]=tr->t->pt[2].v[1];
						pt1->pt[2].v[2]=tr->t->pt[2].v[2];
						pt1->next=NULL;
						//把三角形的另外两点放到队列中
						if (tr->p!=&tr->t->pt[0])
						{
							if (!tr->t->pt[0].vr->visit)
							{
								tr->t->pt[0].vr->visit=1;
								queue.enQueue(tr->t->pt[0].vr);
							}
						}
						if (tr->p!=&tr->t->pt[1])
						{
							if (!tr->t->pt[1].vr->visit)
							{
								tr->t->pt[1].vr->visit=1;
								queue.enQueue(tr->t->pt[1].vr);
							}
						}
						if (tr->p!=&tr->t->pt[2])
						{
							if (!tr->t->pt[2].vr->visit)
							{
								tr->t->pt[2].vr->visit=1;
								queue.enQueue(tr->t->pt[2].vr);
							}
						}
					}
					tr=tr->nextRef;
				}
			}
		}	
		
		if (search2.shead->next)//有可用索引才进行搜索
		{
			queue.enQueue(search2.shead->next->pv);
			search2.shead->next->pv->visit=1;//第一个节点被访问了
			while (queue.num) //队列不空时
			{
				vr=queue.deQueue();
				tr=vr->nextRef;
				while(tr)
				{
					if(!tr->t->visit)//未被访问，则将其复制到pCS1所指的三角形中
					{
						tr->t->visit=1;
						pt2->next=new Triangle;
						pt2=pt2->next;
						pt2->pt[0].v[0]=tr->t->pt[0].v[0];
						pt2->pt[0].v[1]=tr->t->pt[0].v[1];
						pt2->pt[0].v[2]=tr->t->pt[0].v[2];
						pt2->pt[1].v[0]=tr->t->pt[1].v[0];
						pt2->pt[1].v[1]=tr->t->pt[1].v[1];
						pt2->pt[1].v[2]=tr->t->pt[1].v[2];
						pt2->pt[2].v[0]=tr->t->pt[2].v[0];
						pt2->pt[2].v[1]=tr->t->pt[2].v[1];
						pt2->pt[2].v[2]=tr->t->pt[2].v[2];
						pt2->next=NULL;
						//把三角形的另外两点放到队列中
						if (tr->p!=&tr->t->pt[0])
						{
							if (!tr->t->pt[0].vr->visit)
							{
								tr->t->pt[0].vr->visit=1;
								queue.enQueue(tr->t->pt[0].vr);
							}
						}
						else if (tr->p!=&tr->t->pt[1])
						{
							if (!tr->t->pt[1].vr->visit)
							{
								tr->t->pt[1].vr->visit=1;
								queue.enQueue(tr->t->pt[1].vr);
							}
						}
						else if (tr->p!=&tr->t->pt[2])
						{
							if (!tr->t->pt[2].vr->visit)
							{
								tr->t->pt[2].vr->visit=1;
								queue.enQueue(tr->t->pt[2].vr);
							}
						}
					}
					tr=tr->nextRef;
				}
			}
		}	
		//梳理边，使其连接成环
		Edge *qe,*te;
		bool just=0;
		pe=ehead->next;
		while(pe->next)
		{
			just=0;
			qe=pe;
			while (qe->next)
			{
				//if(fabs(qe->next->x1-pe->x2)==0&&(qe->next->y1-pe->y2)==0&&(qe->next->z1-pe->z2)==0)
				if(fabs(qe->next->x1-pe->x2)<DELTA&&fabs(qe->next->y1-pe->y2)<DELTA&&fabs(qe->next->z1-pe->z2)<DELTA)
				{
					te=qe->next;
					//修正误差
					te->x1=pe->x2;
					te->y1=pe->y2;
					te->z1=pe->z2;
					qe->next=te->next;
					just=1;
					break;
				}
				else qe=qe->next;
			}
			if(just==0) MessageBox("错误：没有找到正确的边！");
			te->next=pe->next;
			pe->next=te;
			pe=pe->next;
		}
		if(!(fabs(ehead->next->x1-pe->x2)<DELTA&&fabs(ehead->next->y1-pe->y2)<DELTA&&fabs(ehead->next->z1-pe->z2)<DELTA)) MessageBox("错误：切割出的边不能封闭");
		pe->x2=ehead->next->x1;
		pe->y2=ehead->next->y1;
		pe->z2=ehead->next->z1;
		//对此环进行剖分,并将剖分后的三角形接到pcs
		Triangle *t1=m_p2t.polygonToTriangles(ehead,TRUE,vn);
		Triangle *t2=m_p2t.copyAnotherSide(t1);
		pt1->next=t1->next;
		delete t1;
		pt2->next=t2->next;
		delete t2;
		//pCS1和pCS2需复制pActiveCS中的旋转缩放移动
		pCS1->m_size=pCS2->m_size=pActiveCS->m_size;
		pCS1->m_translate[0]=pCS2->m_translate[0]=pActiveCS->m_translate[0];
		pCS1->m_translate[1]=pCS2->m_translate[1]=pActiveCS->m_translate[1];
		pCS1->m_translate[2]=pCS2->m_translate[2]=pActiveCS->m_translate[2];
		for (int i=0;i<16;i++) 
		{
			pCS1->m_mvMatrix[i]=pCS2->m_mvMatrix[i]=pActiveCS->m_mvMatrix[i];
		}
		//删除pActiveCS,挂载pCS1和pCS2,workmode=1
		CS *pCS=m_cs_head;
		while(pCS->next)
		{
			if (pCS->next==pActiveCS)
			{
				pCS->next=pActiveCS->next;
				delete pActiveCS;
				m_num_model--;
				pActiveCS=NULL;
				break;
			}
			else pCS=pCS->next;
		}
		pCS1->m_name="剖分体1";
		pCS1->m_visible=1;
		pCS1->m_selected=1;
		m_maxIndex++;
		pCS1->m_index=m_maxIndex;
		pCS1->setColor(getRandomColor());	
		m_num_model++;	
		pCS2->m_name="剖分体2";
		pCS2->m_visible=1;
		pCS2->m_selected=1;
		m_maxIndex++;
		pCS2->m_index=m_maxIndex;
		pCS->setColor(getRandomColor());	
		m_num_model++;	
		pCS1->createVertexRef();//由Triangle链获取VertexRef链
		pCS2->createVertexRef();//由Triangle链获取VertexRef链
		pCS1->next=pCS2;
		pCS2->next=m_cs_head->next;
		m_cs_head->next=pCS1;
		m_workMode=2;//移动模式
		renderScene();
	}
}

bool CMMDlg::saveFile(byte type)//存盘对话
{
	CString filter,defEx;
	int structsize=0; //显示风格变量
	DWORD dwVersion;
	dwVersion = GetVersion();// 检测目前的操作系统 
	if (dwVersion < 0x80000000)	structsize =88; //操作系统是Windows NT/2000
	else structsize =76; //Windows 95/98系统	
	if (type==0)//会显示txt和mm文件选择
	{
		filter="ModelMaker文件(*.mm)|*.mm|实验数据文件(*.txt)|*.txt||";
		defEx="mm";
	}
	else if (type==1)//mm文件
	{
		filter="ModelMaker文件(*.mm)|*.mm||";
		defEx="mm";
	}
	else if (type==2)//smm文件
	{
		filter="单个ModelMaker模型文件(*.smm)|*.smm||";
		defEx="smm";
	}
	else if (type==3)//txt文件
	{
		filter="实验室要需文件格式(*.txt)|*.txt||";
		defEx="txt";
	}
	CFileDialog saveFile(FALSE, defEx, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
	saveFile.m_ofn.lStructSize = structsize;
	CString s;	
	if (type==0||type==1) 
	{	
		s=m_directory+"SavedScene";
		saveFile.m_ofn.lpstrInitialDir=s;
	}
	else if (type==2) 
	{	
		s=m_directory+"SavedModel";
		saveFile.m_ofn.lpstrInitialDir=s;
	}	
	if(saveFile.DoModal()!=IDOK) return FALSE;//放弃储存
	FILE *fp=fopen(saveFile.GetPathName(),"w");	
	if (!fp)
	{
		AfxMessageBox("文件写入失败！");
		return FALSE;
	}
	CS *pCS=m_cs_head;
	Triangle *pt;
	if (saveFile.GetFileExt()=="mm") 
	{
		int i,j;
		fprintf(fp,"This is a ModelMaker file\n");
		fprintf(fp,"%d\n",m_num_model);//模型的个数
		
		for (i=0;i<m_num_model;i++)
		{
			pCS=pCS->next;
			fprintf(fp,"*here is the beginning of a model*\n");
			fprintf(fp,"%lf\n",pCS->m_size);
			for (j=0;j<16;j++)
			{
				fprintf(fp,"%lf\n",pCS->m_mvMatrix[j]);
			}
			fprintf(fp,"%lf\n",pCS->m_translate[0]);
			fprintf(fp,"%lf\n",pCS->m_translate[1]);
			fprintf(fp,"%lf\n",pCS->m_translate[2]);
			fprintf(fp,"%d\n",pCS->m_color[0]);
			fprintf(fp,"%d\n",pCS->m_color[1]);
			fprintf(fp,"%d\n",pCS->m_color[2]);
			fprintf(fp,"%d\n",pCS->m_selected);
			fprintf(fp,"%d\n",pCS->m_visible);
			fprintf(fp,"%d\n",pCS->m_free);
			fprintf(fp,"%d\n",pCS->getNumOfTriangles());
			fprintf(fp,"%s\n",pCS->m_name);
			//以下是三角形数据
			pt=pCS->it;
			while (pt->next)
			{
				pt=pt->next;
				fprintf(fp,"%lf\t%lf\t%lf\n",pt->pt[0].v[0],pt->pt[0].v[1],pt->pt[0].v[2]);
				fprintf(fp,"%lf\t%lf\t%lf\n",pt->pt[1].v[0],pt->pt[1].v[1],pt->pt[1].v[2]);
				fprintf(fp,"%lf\t%lf\t%lf\n\n",pt->pt[2].v[0],pt->pt[2].v[1],pt->pt[2].v[2]);
			}
			fprintf(fp,"*here is the end of this model*\n\n");
		}
	}
	else if(saveFile.GetFileExt()=="smm")
	{
		pCS=pActiveCS;//储存活动模型
		fprintf(fp,"This is a single ModelMaker file\n");
		fprintf(fp,"%lf\n",pCS->m_size);
		for (int j=0;j<16;j++)
		{
			fprintf(fp,"%lf\n",pCS->m_mvMatrix[j]);
		}
		fprintf(fp,"%lf\n",pCS->m_translate[0]);
		fprintf(fp,"%lf\n",pCS->m_translate[1]);
		fprintf(fp,"%lf\n",pCS->m_translate[2]);
		fprintf(fp,"%d\n",pCS->m_color[0]);
		fprintf(fp,"%d\n",pCS->m_color[1]);
		fprintf(fp,"%d\n",pCS->m_color[2]);
		fprintf(fp,"%d\n",pCS->getNumOfTriangles());
		fprintf(fp,"%s\n",pCS->m_name);
		//以下是三角形数据
		pt=pCS->it;
		while (pt->next)
		{
			pt=pt->next;
			fprintf(fp,"%lf\t%lf\t%lf\n",pt->pt[0].v[0],pt->pt[0].v[1],pt->pt[0].v[2]);
			fprintf(fp,"%lf\t%lf\t%lf\n",pt->pt[1].v[0],pt->pt[1].v[1],pt->pt[1].v[2]);
			fprintf(fp,"%lf\t%lf\t%lf\n\n",pt->pt[2].v[0],pt->pt[2].v[1],pt->pt[2].v[2]);
		}
	}
	else if (saveFile.GetFileExt()=="txt")//实验室所用文件
	{
		double m[16];
		double x,y,z,dz;
		while(pCS->next)
		{
			pCS=pCS->next;
			//获取模型变换矩阵
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glTranslated(pCS->m_translate[0],pCS->m_translate[1],pCS->m_translate[2]);
			glMultMatrixd(pCS->m_mvMatrix);
			glScaled(pCS->m_size,pCS->m_size,pCS->m_size);
			glGetDoublev(GL_MODELVIEW_MATRIX, m);
			glPopMatrix();
			dz=1e-5-m_outputDlg.m_size*pCS->rminZ;//将物体都移动到平面上
			pt=pCS->it;
			while (pt->next)
			{
				pt=pt->next;
				fprintf(fp,"21840.00\n3.000000\n",x,y,z);
				x=m[0]*pt->pt[0].v[0]+m[4]*pt->pt[0].v[1]+m[8]*pt->pt[0].v[2]+m[12];
				y=m[1]*pt->pt[0].v[0]+m[5]*pt->pt[0].v[1]+m[9]*pt->pt[0].v[2]+m[13];
				z=m[2]*pt->pt[0].v[0]+m[6]*pt->pt[0].v[1]+m[10]*pt->pt[0].v[2]+m[14];
				x=x*m_outputDlg.m_size+m_outputDlg.m_moveX;
				y=y*m_outputDlg.m_size+m_outputDlg.m_moveY;
				z=z*m_outputDlg.m_size+dz;
				fprintf(fp,"%.8lf\n%.8lf\n%.8lf\n",x,y,z);//8位小数
				x=m[0]*pt->pt[1].v[0]+m[4]*pt->pt[1].v[1]+m[8]*pt->pt[1].v[2]+m[12];
				y=m[1]*pt->pt[1].v[0]+m[5]*pt->pt[1].v[1]+m[9]*pt->pt[1].v[2]+m[13];
				z=m[2]*pt->pt[1].v[0]+m[6]*pt->pt[1].v[1]+m[10]*pt->pt[1].v[2]+m[14];
				x=x*m_outputDlg.m_size+m_outputDlg.m_moveX;
				y=y*m_outputDlg.m_size+m_outputDlg.m_moveY;
				z=z*m_outputDlg.m_size+dz;
				fprintf(fp,"%.8lf\n%.8lf\n%.8lf\n",x,y,z);
				x=m[0]*pt->pt[2].v[0]+m[4]*pt->pt[2].v[1]+m[8]*pt->pt[2].v[2]+m[12];
				y=m[1]*pt->pt[2].v[0]+m[5]*pt->pt[2].v[1]+m[9]*pt->pt[2].v[2]+m[13];
				z=m[2]*pt->pt[2].v[0]+m[6]*pt->pt[2].v[1]+m[10]*pt->pt[2].v[2]+m[14];
				x=x*m_outputDlg.m_size+m_outputDlg.m_moveX;
				y=y*m_outputDlg.m_size+m_outputDlg.m_moveY;
				z=z*m_outputDlg.m_size+dz;
				fprintf(fp,"%.8lf\n%.8lf\n%.8lf\n",x,y,z);
			}			
		}
	}
	else 
	{
		MessageBox(" 文件扩展名不正确！");
		return FALSE;
	}
	fclose(fp);
	return TRUE;
}


void CMMDlg::densify(double increase)
{
	if (pActiveCS)
	{
		if (increase==0) pActiveCS->densify();
		else pActiveCS->densify(increase);
		renderScene();
	}
	else MessageBox("错误：物体选择不正确！");
}

void CMMDlg::densifySelected()//加密选中者
{
	if (pActiveCS)
	{
		pActiveCS->densify((bool)FALSE);
		renderScene();
	}
	else MessageBox("错误：物体选择不正确！");
}

void CMMDlg::OnOpen() //打开文档（mm/smm）
{
	Openfile(0);
}

CString CMMDlg::readLine(FILE *fp)
{	
	int ch='a';
	CString s;
	while((ch=fgetc(fp))!='\n')	s+=ch;
	return s;
}

void CMMDlg::OnSave()//存盘命令
{
	if(m_filePath!=""&&m_num_model)
	{
		FILE *fp=fopen(m_filePath,"w");
		CS *pCS=m_cs_head;
		Triangle *pt;
		int i,j;
		fprintf(fp,"This is a ModelMaker file\n");
		fprintf(fp,"%d\n",m_num_model);//模型的个数
		
		for (i=0;i<m_num_model;i++)
		{
			pCS=pCS->next;
			fprintf(fp,"*here is the beginning of a model*\n");
			fprintf(fp,"%lf\n",pCS->m_size);
			for (j=0;j<16;j++)
			{
				fprintf(fp,"%lf\n",pCS->m_mvMatrix[j]);
			}
			fprintf(fp,"%lf\n",pCS->m_translate[0]);
			fprintf(fp,"%lf\n",pCS->m_translate[1]);
			fprintf(fp,"%lf\n",pCS->m_translate[2]);
			fprintf(fp,"%d\n",pCS->m_color[0]);
			fprintf(fp,"%d\n",pCS->m_color[1]);
			fprintf(fp,"%d\n",pCS->m_color[2]);
			fprintf(fp,"%d\n",pCS->m_selected);
			fprintf(fp,"%d\n",pCS->m_visible);
			fprintf(fp,"%d\n",pCS->m_free);
			fprintf(fp,"%d\n",pCS->getNumOfTriangles());
			fprintf(fp,"%s\n",pCS->m_name);
			//以下是三角形数据
			pt=pCS->it;
			while (pt->next)
			{
				pt=pt->next;
				fprintf(fp,"%lf\t%lf\t%lf\n",pt->pt[0].v[0],pt->pt[0].v[1],pt->pt[0].v[2]);
				fprintf(fp,"%lf\t%lf\t%lf\n",pt->pt[1].v[0],pt->pt[1].v[1],pt->pt[1].v[2]);
				fprintf(fp,"%lf\t%lf\t%lf\n\n",pt->pt[2].v[0],pt->pt[2].v[1],pt->pt[2].v[2]);
			}
			fprintf(fp,"*here is the end of this model*\n\n");
		}
		fclose(fp);
		MessageBox("文件保存成功！");
	}	
}

void CMMDlg::changeWorkMode(double mode)
{
	m_workMode=mode;
	if (mode==5)
	{
		m_numBuffHits=0;
		m_numPointHits=0;
	}
	if (mode==7.1)
	{
		m_hits[0]=0;
		m_chooseDensify=1;
	}
}

int CMMDlg::getCSNum()//获取模型个数
{
	CS *p=m_cs_head;
	int num=0;
	while (p->next)
	{
		p=p->next;
		num++;
	}
	return num;
}

void CMMDlg::OnOutput() //输出为实验室所用文件
{
	if (m_num_model)
	{
		GLdouble minX,maxX,minY,maxY,maxH;
		CS *pCS=m_cs_head->next;
		pCS->getRange();	
		minX=pCS->rminX;
		maxX=pCS->rmaxX;
		minY=pCS->rminY;
		maxY=pCS->rmaxY;
		maxH=pCS->rmaxZ-pCS->rminZ;
		while (pCS->next)
		{
			pCS=pCS->next;
			pCS->getRange();
			if(minX>pCS->rminX) minX=pCS->rminX;
			if(maxX<pCS->rmaxX) maxX=pCS->rmaxX;
			if(minY>pCS->rminY) minY=pCS->rminY;
			if(maxY<pCS->rmaxY) maxY=pCS->rmaxY;
			if(maxH<pCS->rmaxZ-pCS->rminZ) maxH=pCS->rmaxZ-pCS->rminZ;
		}
		m_outputDlg.m_mLenght=maxX-minX;//默认范围是长度100nm
		m_outputDlg.m_mWidth=maxY-minY;
		m_outputDlg.m_mHeight=maxH;
		m_outputDlg.m_mix=minX;
		m_outputDlg.m_miy=minY;
		if (m_outputDlg.DoModal()==IDOK)//立即输出
		{
			saveFile(3);//输出为实验室所用文件
		}
	}	
}

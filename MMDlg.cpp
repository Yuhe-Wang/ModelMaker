// MMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "MMDlg.h"
#include "Queue.h"

#define Lw 100.0f //�����Ӽ��ռ�߳�
#define KAXIS 0.7f//�������������
#define NUM_LATTICE 20 //3Dդ�����
#define INCREASE 6 //��ʾ���������
#define SIZEOFLATTICE 15.0 //������ʾ��դ����
#define PRECISION 8
#define AREA 35
#define DELTA 0.000001

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMMDlg *g_p; //ȫ�ֱ������Ա���������
bool g_renderMode;//��Ⱦ״̬,����CS����
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
	//////////////////������ʼ��/////////////////
	g_p=this;
	g_renderMode=1;//1��������Ⱦģʽ��0����ѡģʽ
	m_theta=70;
	m_phi=30;
	m_R=70;
	m_num_view=4;
	m_projectManner=TRUE;//TRUE��ʾ��ͶӰ��FALSE��ʾ͸��ͶӰ
	m_activeView=0;
	m_viewport=0;//0��ʾ3d���������룬1��2��3���α�ʾtop��front��left��ͼ������
	m_view_style=0;//0��ʾ�ĸ����ִ��ڣ�1��ʾ3��1С����
	m_numOfLattice=15.0;
	m_size[0]=m_size[1]=m_size[2]=m_size[3]=1;
	//m_update[0]=m_update[1]=m_update[2]=m_update[3]=0;
	m_viewManner[1]=m_viewManner[2]=m_viewManner[3]=1;
	m_viewManner[0]=1;
	m_workMode=0;//����ģʽ��0��ʾ������1��ʾѡ��2���ƶ���3�Ǿ��ȷ�����4����ת��5����϶�
	m_select=0;//��ʼʱû��ѡ���κε�
	m_num_model=0;//��ʼʱ������Ϊ0
	m_viewSide[0]=m_viewSide[1]=m_viewSide[2]=m_viewSide[3]=0;
	m_maxIndex=0;
	m_move=0;//��û���ƶ�����
	m_drawChooseBox=0;//��ʼʱ������ѡ���
	m_drawRotateCross=0;//��ʼʱ��������תʮ�ּ�
	m_LBdown=0;//��ʱ��������û�а���
	m_BoxChoose=0;//��ѡ�ر�
	m_create=0;//����״̬,0��������1�����壬2���壬3���壬4׵�壬5Բ����6��״��
	m_numToCopy=0;
	m_numPointHits=0;//��ʼʱû�е㱻ѡ��
	m_cursorBox=0;//��ʱ����������
	
	
	/////////////////��ʼ����ݼ�����////////////////////////
	FILE *fp=fopen("HotKeysSetting.txt","r");
	if (fp)//�����û���ݼ�
	{
		fscanf(fp,"%d%d%d%d%d%d%d",&m_hotKeys.theta_plus,&m_hotKeys.theta_minus,&m_hotKeys.phi_plus,
			&m_hotKeys.phi_minus,&m_hotKeys.R_plus,&m_hotKeys.R_minus,&m_hotKeys.del_object);	
	}
	else //Ĭ�Ͽ�ݼ�
	{
		m_hotKeys.theta_plus=83;
		m_hotKeys.theta_minus=87;
		m_hotKeys.phi_plus=65;
		m_hotKeys.phi_minus=68;
		m_hotKeys.R_plus=VK_PRIOR;
		m_hotKeys.R_minus=VK_NEXT;
		m_hotKeys.del_object=VK_DELETE;
	}
	//////////////////����/////////////////
	m_cs_head=new CS;
	m_cs_head->next=NULL;
	pActiveCS=NULL; //��δ��������
	////////////////////////���ð�ťͼ��////////////////////////
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
	calculateRc();//����4����ͼ������Դ������λ��
	GetClientRect(&m_old_rect);
	////////////////////////��������ʼ��////////////////////////
	//�����ǩ
	m_tab.InsertItem(0, _T("����"));
    m_tab.InsertItem(1, _T("�޸�"));
	//�����Ի���
    m_page1.Create(IDD_PANEL_CREATE, &m_tab);
    m_page2.Create(IDD_PANEL_MODIFY, &m_tab);
	//�趨��Tab����ʾ�ķ�Χ
    CRect rc;
    m_tab.GetClientRect(rc);
    rc.top += 20;
    rc.bottom -= 8;
    //rc.left += 8;
    //rc.right -= 8; 
    m_page1.MoveWindow(&rc);
    m_page2.MoveWindow(&rc);
    //������ѶԻ������ָ�뱣������
    pDlg[0] = &m_page1;
    pDlg[1] = &m_page2;
    //��ʾ��ʼҳ��
    pDlg[0]->ShowWindow(SW_SHOW);
    pDlg[1]->ShowWindow(SW_HIDE);
	m_cur=0; 	
	////////////////////////״̬����ʼ��////////////////////////
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
	//////////////////////��ʼ����ɫ���//////////////////////////
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
		m_activeColor=RGB(r,g,b);//��ȡ���ɫ
		fclose(fp);
	}
	//����64�ֻ���ɫ
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
		AfxMessageBox("����BasicColors.txt��ʧ��\n  ������Ҫ�ر�...");
		exit(0);
	}
	getRandomColor();//���ָ��һ����ɫ
	//////////////////ϵͳʱ��������������/////////////////////
	CTime tm=CTime::GetCurrentTime();
	short y,m,d,h,n,s;
	y=tm.GetYear();
	m=tm.GetMonth();
	d=tm.GetDay();
	h=tm.GetHour();
	n=tm.GetMinute();
	s=tm.GetSecond();
	m_In=y+70*(m+12*(d+31*(h+23*(n+59*s))));
	///////////////////////////��ȡ����ǰ����·��,���ڱ����ļ�///////////////////
	TCHAR   exeFullPath[256]; 
	DWORD   path=GetCurrentDirectory(256,exeFullPath); 	
	m_directory=exeFullPath;
	m_directory+="\\";
	//m_directory.Replace("\\", "\\\\");
	///////////////////////////��ʼ����ͼģ�;���/////////////////////////////////
	InitOpenGL();//��ʼ��openGL
	for (int i=0;i<4;i++)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		lookAt(i);
		glScalef(50,50,50);
		glScalef(m_size[i],m_size[i],m_size[i]);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelMatrix[i]);
	}
	m_knife.init();//��ʼ���и�������
	//////////////////////////////////////////////////////////////////
	SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE);//��ʼʱ�������
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

void CMMDlg::InitOpenGL()//��ʼ��openGL
{
	//��û�ͼDC
	m_pdc=GetDlgItem(IDC_ALL)->GetDC();
	if (!bSetupPixelFormat(m_pdc))	return;
	HGLRC hrc= wglCreateContext(m_pdc->GetSafeHdc());
	wglMakeCurrent(m_pdc->GetSafeHdc(), hrc);
	//////////////����λͼ����//////////////////
	char fontname[]="Arial"; //ѡ������ΪArial������
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
	DeleteObject(hfont);//ɾ��������	
	//////////////////////////////////
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);  
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);	
	//���������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	//ֻ������ʱ������
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//��������
	GLfloat ambientLight[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat diffuseLight[]={0.7f,0.7f,0.7f,1.0f};
	GLfloat lightPos[]={80.0f,80.0f,80.0f,0.0f};//��Դλ��
	GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);	
	//������ɫ׷��
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMateriali(GL_FRONT,GL_SHININESS,128);

	glShadeModel(GL_SMOOTH);
	glSelectBuffer(BUFFER_LENGTH,m_selectBuff);//����ѡ�񻺳���
	glPointSize(5.0);//����ѡ�е���ʾ�Ĵ�С

	glEnable(GL_RESCALE_NORMAL);//�������Զ���׼��
	glClearColor(0.5, 0.5, 0.5, 0.0f);//����������ɫ
}

bool CMMDlg::bSetupPixelFormat(CDC * m_pdc)  //�������ظ�ʽ����
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
		MessageBox("����ƥ��ʧ�ܣ�");
		return FALSE;
	}
	if (SetPixelFormat(m_pdc->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("��������ʧ�ܣ�");
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
	if(m_num_model) //��ģ������
	{
		int choice=MessageBox("�Ƿ񱣴�ģ���ļ�?","������ʾ:",MB_YESNOCANCEL);
		if(choice!=IDCANCEL)
		{
			if (choice==IDYES&&!saveFile(0)) return;	
		}
		else return;
	}
	HGLRC hrc = ::wglGetCurrentContext();
	::wglMakeCurrent(NULL,  NULL);
	if (hrc) ::wglDeleteContext(hrc);
	//������ɫ��
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
		fprintf(fp,"%d\t%d\t%d\n",r,g,b);//��ǰѡ�е���ɫ
		fclose(fp);
	}
	CDialog::OnClose();	
}


void CMMDlg::renderScene()
{
	int i=0;	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����������Ȼ�����
	/*****************���¿�ʼƽ�����*****************/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,rcALL.Width(),0,rcALL.Height(),-Lw,Lw);//����ͶӰ�ռ�����Ļһ�£�����ƽ���ͼ
	glViewport(0,0,rcALL.Width(),rcALL.Height());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDepthMask(GL_TRUE);
	if (m_drawChooseBox)//����ѡ���
	{		
		glColor3ub(255,255,255);
		glBegin(GL_LINE_LOOP);
			glVertex2d(m_pp[0][0],m_pp[0][1]);
			glVertex2d(m_pp[0][0],m_pp[2][1]);
			glVertex2d(m_pp[2][0],m_pp[2][1]);
			glVertex2d(m_pp[2][0],m_pp[0][1]);
		glEnd();
	}
	if (m_drawRotateCross)//������תʮ�ּ�
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
		glEnable(GL_LINE_STIPPLE);//�����㻭��
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
	if (m_cursorBox&&(m_workMode==5||m_workMode==7.1))//��������
	{
		glColor3ub(255,0,0);
		glBegin(GL_LINE_LOOP);
		glVertex3d(m_pp[2][0]-m_page2.m_cursorW/2,m_pp[2][1]-m_page2.m_cursorH/2,Lw);
		glVertex3d(m_pp[2][0]+m_page2.m_cursorW/2,m_pp[2][1]-m_page2.m_cursorH/2,Lw);		
		glVertex3d(m_pp[2][0]+m_page2.m_cursorW/2,m_pp[2][1]+m_page2.m_cursorH/2,Lw);
		glVertex3d(m_pp[2][0]-m_page2.m_cursorW/2,m_pp[2][1]+m_page2.m_cursorH/2,Lw);
		glEnd();
	}
	glDepthMask(GL_FALSE);//�ر����д��
	if(m_num_view==1)//����ͼ
	{
		int width=rcALL.Height(),length=rcALL.Width();
		if (m_activeView!=0)//��3D��ͼʱ����ƽ��դ��
		{
			glColor3ub(155,155,155);//����դ����ɫ
			glBegin(GL_LINES);			
				//m_numOfLattice=20;
				int delta=width/m_numOfLattice;  //һ��դ��ı߳�
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
		//���ƻ��ͼ�ı߿�
		glColor3ub(255,255,0);
		glBegin(GL_LINE_LOOP);
			glVertex2f(0,0);
			glVertex2f(length,0);
			glVertex2f(length,width);
			glVertex2f(0,width);
		glEnd();
		//������ʾ������ʾ�ı�		
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
		//���Ʒ��ذ�ť
		glBegin(GL_LINE_LOOP);
			glVertex2f(length-40,width-30);
			glVertex2f(length-6,width-30);
			glVertex2f(length-6,width-6);
			glVertex2f(length-40,width-6);
		glEnd();
		GLTextOut(length-39,width-28,0,"Min");
		
		/********************3D��ͼ*******************/
		glDepthMask(GL_TRUE);
		CS *pCS=m_cs_head;
		float aspectRatio=(float)(rcALL.Width())/rcALL.Height();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-Lw*aspectRatio,Lw*aspectRatio,-Lw,Lw,-10*Lw,10*Lw);//�����Ӽ��ռ�
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
		if (m_workMode==0) drawNewObject();//����ʱ����������
		if(m_workMode==6) drawKnife();//�����и
	}
	else //����ͼ
	{
		//���ƻ��ͼ�ı߿�
		glColor3ub(255,255,0);
		glBegin(GL_LINE_LOOP);	
			glVertex2f(rc[m_activeView].left,rc[m_activeView].bottom);
			glVertex2f(rc[m_activeView].right,rc[m_activeView].bottom);
			glVertex2f(rc[m_activeView].right,rc[m_activeView].top);
			glVertex2f(rc[m_activeView].left,rc[m_activeView].top);	
		glEnd();
		//����ƽ��դ��
		glColor3ub(155,155,155);//����դ����ɫ
		glBegin(GL_LINES);
			int width,length,x,y,delta;
			for (i=1;i<=3;i++)
			{
				width=rc[i].top-rc[i].bottom;
				length=rc[i].right-rc[i].left;
				delta=width/m_numOfLattice;  //һ��դ��ı߳�
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
		glColor3ub(255,255,0);//����Ϊ��ɫ
		//������ʾ�ı�
		GLTextOut(rc[0].left,rc[0].top-20,0,"3D");
		if(m_viewSide[1]) GLTextOut(rc[1].left,rc[1].top-20,0,"bottom");
		else GLTextOut(rc[1].left,rc[1].top-20,0,"top");
		if(m_viewSide[2]) GLTextOut(rc[2].left,rc[2].top-20,0,"back");
		else GLTextOut(rc[2].left,rc[2].top-20,0,"front");
		if(m_viewSide[3]) GLTextOut(rc[3].left,rc[3].top-20,0,"right");
		else GLTextOut(rc[3].left,rc[3].top-20,0,"left");
		//������󻯰�ť
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
		/**********************3D��ͼ**************************/
		glDepthMask(GL_TRUE);
		CS *pCS=m_cs_head;
		float ratio;//��ͼ������������		
		//�����ĸ���ͼ
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
			if(!i) draw3DLattice();//view��ͼҪ������ά����
			DrawModel();
			pCS=m_cs_head;
			while (pCS->next)
			{
				pCS=pCS->next;
				if (pCS->m_visible) pCS->show(m_viewManner[i],m_workMode);
			}
			if (m_workMode==0) drawNewObject();//����ʱ����������
			if(m_workMode==6) drawKnife();//�����и
		}
	} 
	glFinish(); //ǿ�ƻ�ͼ�����ۻ������Ƿ�����
	SwapBuffers(wglGetCurrentDC()); //������ͼ����ʾ���棨��˫�������)	
}

void CMMDlg::lookAt(short i)//�����۲��ӽ�
{
	glLoadIdentity();
	switch(i)
	{
	case 0:		
		{
			double ks1=sin(m_theta*PI/180.0),ks2=sin(m_phi*PI/180.0),kc1=cos(m_theta*PI/180.0),kc2=cos(m_phi*PI/180.0);
			gluLookAt(m_R*ks1*kc2,m_R*ks1*ks2,m_R*kc1,0.0f,0.0f,0.0f,-kc1*kc2,-kc1*ks2,ks1); //ȷ���ӽ�
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

void CMMDlg::DrawModel() //����ģ�Ͳ���
{
	/**************����������****************/
	glColor3f(1.0,1.0,1.0);//��ͼ��ɫ��Ϊ��ɫ
	glBegin(GL_LINES);
	glVertex3f(-KAXIS*Lw,0.0f,0.0f);
	glVertex3f(KAXIS*Lw,0.0f,0.0f);
	glVertex3f(0.0f,-KAXIS*Lw,0.0f);
	glVertex3f(0.0f,KAXIS*Lw,0.0f);
	glVertex3f(0.0f,0.0f,-KAXIS*Lw);
	glVertex3f(0.0f,0.0f,KAXIS*Lw);
	glEnd();
	glBegin(GL_LINE_STRIP); //����x���ͷ
	glVertex3f(KAXIS*0.95*Lw,0.0f,0.02*Lw);
	glVertex3f(KAXIS*Lw,0.0f,0.0f);
	glVertex3f(KAXIS*0.95*Lw,0.0f,-0.02*Lw);
	glEnd();
	GLTextOut(KAXIS*Lw*1.1,0.0f,0,"x");
	glBegin(GL_LINE_STRIP); //����y���ͷ
	glVertex3f(0,KAXIS*0.95*Lw,0.02*Lw);// 
	glVertex3f(0.0f,KAXIS*Lw,0.0f);
	glVertex3f(0,KAXIS*0.95*Lw,-0.02*Lw);
	glEnd();
	GLTextOut(0,KAXIS*Lw*1.1,0,"y");
	glBegin(GL_LINE_STRIP); //����z���ͷ
	glVertex3f(0,0.02*Lw,KAXIS*0.95*Lw);
	glVertex3f(0,0.0f,KAXIS*Lw);
	glVertex3f(0,-0.02*Lw,KAXIS*0.95*Lw);
	glEnd();
	GLTextOut(0,0,KAXIS*Lw*1.1,"z");
	/****************����ʵ��ģ��*******************/
	glScalef(50,50,50);
}

BOOL CMMDlg::PreTranslateMessage(MSG* pMsg) //���ؼ����ַ�����Ϣ����Ԥ����
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
		//����0�Ŵ�����ͼģ�;���
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

void CMMDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) //ѡ��л���Ӧ
{
	pDlg[m_cur]->ShowWindow(SW_HIDE);
    m_cur = m_tab.GetCurSel();
    pDlg[m_cur]->ShowWindow(SW_SHOW);	

	*pResult = 0;
}

void CMMDlg::draw3DLattice()
{
	glDepthMask(GL_FALSE);//����դ��ʱ��ֹд�����ֵ
	glColor3ub(155,155,155);//����դ����ɫ
	static float lw=Lw;//*0.9;
	static float step=2*lw/NUM_LATTICE;//դ�񲽳�
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
	glDepthMask(GL_TRUE);//�ָ����д��
}

void CMMDlg::drawPlaneLattice()//����դ��
{
	glLoadIdentity();
	//���Ʊ߿�
	glColor3ub(54.0,63.0,75.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-Lw,-Lw);
		glVertex2f(Lw,-Lw);
		glVertex2f(Lw,Lw);
		glVertex2f(-Lw,Lw);
	glEnd();
	glColor3ub(155,155,155);//����դ����ɫ
	float step=2*Lw/NUM_LATTICE;//դ�񲽳�
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

void CMMDlg::changeViewportStyle(short m)//�ı���������1����4��ͶӰ�������л�
{
	if(m==1) //�л�����һ��ͼ
	{
		float aspectRatio=(float)(rcALL.right-rcALL.left)/(rcALL.bottom-rcALL.top);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(m_projectManner) glOrtho(-Lw*aspectRatio,Lw*aspectRatio,-Lw,Lw,-Lw,Lw);//����ͶӰ�ռ�
		else gluPerspective(45,aspectRatio,-100,100);
		glViewport(0,0,rcALL.right-rcALL.left,rcALL.bottom-rcALL.top);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		m_num_view=1;
	}
	else //�л���4����ͼ
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(m_projectManner) glOrtho(-Lw,Lw,-Lw,Lw,-Lw,Lw);//����ͶӰ�ռ�
		else gluPerspective(45,1,100,200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		m_num_view=4;
	}
}


bool CMMDlg::Openfile(byte type)//��ȡҪ���ļ���·��
{
	int structsize=0;//��ʾ������
	DWORD dwVersion= GetVersion();// ���Ŀǰ�Ĳ���ϵͳ 
	if (dwVersion < 0x80000000)	structsize =88; //����ϵͳ��Windows NT/2000
	else structsize =76; //Windows 95/98ϵͳ
	CString filter,defEx;
	if (type==0)//��mm��smm�ļ�
	{
		filter="ModelMaker�ļ�(*.mm)|*.mm|����ModelMakerģ��(*.smm)|*.smm||";
		defEx="mm";
	}
	else if (type==1)//mm�ļ�
	{
		filter="ModelMaker�ļ�(*.mm)|*.mm||";
		defEx="mm";
	}
	else if(type==2)//smm�ļ�
	{
		filter="ModelMaker�ļ�(*.smm)|*.smm||";
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
	if (openFile.GetFileExt()=="mm")//mm�ļ�
	{
		CS *pCS,*q;
		if (m_num_model) //ԭ���������ļ�
		{
			int choice=MessageBox("�Ƿ��滻���ڳ�����\nyes:�滻��ǰ����\nno:��ӵ���ǰ����\ncancel:ȡ����","�����滻",MB_YESNOCANCEL);
			if (choice==IDCANCEL) return FALSE;
			else if (choice==IDYES) //��ɾ�������е���������
			{
				//ɾ���ɳ���
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
		//��mm�ļ�������ģ��
		pCS=m_cs_head;
		while(pCS->next) pCS=pCS->next;
		FILE *fp=fopen(openFile.GetPathName(),"r");
		if (!fp)
		{
			MessageBox("�ļ���ʧ�ܣ�");
			return FALSE;
		}
		CString id=readLine(fp);
		if (id!="This is a ModelMaker file")
		{
			MessageBox("�ⲻ��mm�ļ���");
			fclose(fp);
			return FALSE;
		}
		int num=0;
		fscanf(fp,"%d",&num);//ģ�͵ĸ���
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
			//����������������
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
			//��ʱһ��CS����������
			m_num_model++;
			m_maxIndex++;
			pCS->m_index=m_maxIndex;//��������
			pCS->createVertexRef();
		}
		pCS->next=NULL;
		m_filePath=openFile.GetPathName();//��¼�򿪵��ļ�λ��
	}
	else //smm�ļ�,ֱ����ӽ�ԭ������
	{
		CS *p=m_cs_head;
		while (p->next) p=p->next;
		p->next=new CS;
		p=p->next;
		p->next=NULL;
		p->loadCS(openFile.GetPathName());//����ģ��
		m_maxIndex++;
		p->m_index=m_maxIndex;//�����Ʊ��	
		m_workMode=1;//?
		m_num_model++;
	}
	renderScene();
	return TRUE;
	
}

void CMMDlg::OnMouseMove(UINT nFlags, CPoint point) //����ƶ�ʱ����Ӧ
{
	CDialog::OnMouseMove(nFlags, point);	
	bool redraw=0;
	toGLCoordinate(&point);
	m_pp[2][0]=point.x;
	m_pp[2][1]=point.y;//��¼��ʱ����λ�ã��Ա����ѡ���
	CS *pCS;		
    /**********************************************************/	
	if (m_workMode==0)//����ģʽ
	{
		byte aim;
		if (m_num_view==4) aim=m_activeView;
		else aim=4;
		if (m_create==1.1||m_create==1.2)//����������
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
			if (m_activeView==1)//��top�ӿڴ���
			{		
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				m_vertex[0][2]=0;
			}
			if (m_activeView==2)//��front�ӿڴ���
			{		
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				m_vertex[0][0]=0;
			}
			if (m_activeView==3)//��left�ӿڴ���
			{		
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				m_vertex[0][1]=0;
			}
			m_create=1.2;
		}
		if (m_create==1.3||m_create==1.4)//����������ڶ����ƶ�
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
			if (m_activeView==1)//���ӿ�1����
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				m_vertex[2][2]=-m_vertex[2][0]+m_vertex[1][0];
				m_vertex[2][0]=m_vertex[1][0];
				m_vertex[2][1]=m_vertex[1][1];				
			}
			if (m_activeView==2)//���ӿ�2����
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				m_vertex[2][0]=-m_vertex[2][2]+m_vertex[1][2];
				m_vertex[2][1]=m_vertex[1][1];
				m_vertex[2][2]=m_vertex[1][2];				
			}
			if (m_activeView==3)//���ӿ�3����
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
				m_projMatrix[aim],m_port[aim],&m_vertex[2][0],&m_vertex[2][1],&m_vertex[2][2]);
				m_vertex[2][1]=m_vertex[2][2]-m_vertex[1][2];
				m_vertex[2][0]=m_vertex[1][0];
				m_vertex[2][2]=m_vertex[1][2];				
			}
			m_create=1.4;
		}
		if (m_create==2.11||m_create==2.12)//��������
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if (m_create==2.13||m_create==2.14)//��������
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if(m_create==2.2)//�㻭�ߴ���
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
			{
				gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[1][0],&m_vertex[1][1],&m_vertex[1][2]);
				double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
					ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
					az=cos(m_theta*PI/180.0);
				m_vertex[1][0]=m_vertex[1][0]-ax/az*m_vertex[1][2];
				m_vertex[1][1]=m_vertex[1][1]-ay/az*m_vertex[1][2];
				m_vertex[1][2]=0;
				//�޸�β������
				m_cylinder.end->x2=m_vertex[1][0];
				m_cylinder.end->y2=m_vertex[1][1];
				m_cylinder.end->z2=0;
			}
			redraw=1;//���ػ�
		}
		if (m_create==2.21)//�����ȡ�߶�
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if (m_create==4.11||m_create==4.12)//����׶��
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if (m_create==4.13||m_create==4.14)//����׶��
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if (m_create==3.1||m_create==3.2)//��������
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if (m_create==5.1||m_create==5.2)//������̥
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if (m_create==5.3||m_create==5.4)//����torus
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if (m_create==6.1||m_create==6.2)//����pipe
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if (m_create==6.3||m_create==6.4)//����pipe
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if (m_create==6.6||m_create==6.7)//����pipe
		{
			if (m_activeView==0)//��3D���ڴ�������øõ��3D����
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
		if(m_workMode==1||m_workMode==3)//ѡ��ģʽ
		{
			m_drawChooseBox=1;//����ѡ���			
		}

		if (m_workMode==2)//�ƶ�ģʽ
		{
			if (m_BoxChoose)	m_drawChooseBox=1;//����ѡ���
			if (m_move)//�����ƶ�ģʽ
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

		if (m_workMode==4)//��תģʽ
		{
			if (m_BoxChoose)//������ѡ
			{
				m_drawChooseBox=1;//����ѡ���
			}
			if (m_move)//������ת
			{
				m_drawRotateCross=1;//������תʮ�ּ�
				GLint dx=point.x-m_pp[0][0],dy=point.y-m_pp[0][1];
				GLdouble angle,rotateAxis[3];
				bool axis=0;//0��ʾ��x�ᣬ1��ʾ��y��
				if (abs(dx)>abs(dy)) //��y����ת
				{	
					angle=dx;
					axis=1;
				}
				else //��x����ת
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
				//���е�ѡ�ж��󶼽�����ת
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
		redraw=1;//��Ȼ�ػ�
	}	
	
	if (m_workMode==5)
	{
		if (m_drag==1.1||m_drag==1.2||m_drag==1.3)
		{
			if(m_drag==1.2) m_drag=1.3;
			GLdouble vertex[2][3];//��ʱ����
			GLdouble matrix[16];
			int aim;
			if (m_num_view==4) aim=m_activeView;
			else aim=4;
			//��ȡ��ͼģ�;���
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glMultMatrixd(m_modelMatrix[m_activeView]);//�����ӽ�
			//�����ƶ���
			glMultMatrixd(pActiveCS->m_mvMatrix);//������ת
			glScaled(pActiveCS->m_size,pActiveCS->m_size,pActiveCS->m_size);//���ط���
			glGetDoublev(GL_MODELVIEW_MATRIX,matrix);
			glPopMatrix();
			gluUnProject((GLdouble)m_lastMovePP[0],(GLdouble)m_lastMovePP[1],0,matrix,
				m_projMatrix[aim],m_port[aim],&vertex[0][0],&vertex[0][1],&vertex[0][2]);
			gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,matrix,
				m_projMatrix[aim],m_port[aim],&vertex[1][0],&vertex[1][1],&vertex[1][2]);
			m_lastMovePP[0]=point.x;
			m_lastMovePP[1]=point.y;
			//��������ѡ�е㣬�ı�������
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
		if (m_cursorBox&&m_drag<2) redraw=1;//��ѡʱ�̻��ƹ��
		if (m_drag==2.1||m_drag==2.11) //����ѡ���
		{
			if(m_drag==2.1)
			{
				m_drag=2.11;//ָʾ�������ƶ�
				m_drawChooseBox=1;
			}
			redraw=1;
		}
		if (m_drag==2.2||m_drag==2.21) //�϶�
		{
			if(m_drag==2.2) m_drag=2.21;//ָʾ�������ƶ�
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
			//��������ѡ�е㣬�ı�������
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
			if(m_knife.state==1.1) m_knife.state=1.2;//��ʾ�������ƶ�
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
			if(m_knife.state==2.1) m_knife.state=2.2;//��ʾ��������ת
			m_drawRotateCross=1;//������תʮ�ּ�
			GLint dx=point.x-m_pp[0][0],dy=point.y-m_pp[0][1];
			GLdouble angle,rotateAxis[3];
			bool axis=0;//0��ʾ��x�ᣬ1��ʾ��y��
			if (abs(dx)>abs(dy)) //��y����ת
			{	
				angle=dx;
				axis=1;
			}
			else //��x����ת
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
			//���е�ѡ�ж��󶼽�����ת
			m_knife.axis[0]=rotateAxis[0];
			m_knife.axis[1]=rotateAxis[1];
			m_knife.axis[2]=rotateAxis[2];
			m_knife.angle=angle;
			redraw=1;
		}
	}
	if (m_workMode==7.1)//ѡ���Լ���ģʽ
	{
		if (m_chooseDensify==2)
		{
			m_drawChooseBox=1;//����ѡ���
			redraw=1;
		}
		if (m_cursorBox) redraw=1;
	}

	if (redraw) renderScene();
}

BOOL CMMDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) //��������Ӧ
{
	if (m_workMode==3)//ѡ���������ģʽ
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
		//������ͼ����
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

void CMMDlg::OnLButtonDblClk(UINT nFlags, CPoint point) //���˫��ʱ����Ӧ
{
	CDialog::OnLButtonDblClk(nFlags, point);

	toGLCoordinate(&point);
	bool needRenderScene=0;
	if (m_num_view==4) 
	{	
		if (m_view_style==1)//3��1С��ͼ���
		{
			for (short i=0;i<4;i++)
			{
				if(rc[i].right-AREA<point.x&&point.x<rc[i].right&&rc[i].top-AREA<point.y&&point.y<rc[i].top)
				{
					if(m_viewport==i) 
					{
						m_num_view=1;//���뵥��ͼ
						//m_update[i]=1;
					}
					else //��view������ͼ
					{
						if(m_viewport!=0||i==0) exchangeRect(rc[0],rc[m_viewport]);
						exchangeRect(rc[0],rc[i]);
						m_viewport=i;
						//m_update[0]=m_update[1]=m_update[2]=m_update[3]=1;//�ĸ������е��ƽ�����궼Ҫ����
					}
					needRenderScene=1;
					break;
				}
			}
			//����4��С��ͼ���ӿھ����ͶӰ����
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
		if (m_view_style==0)//4������ͼ���
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
	{	//�ڵ���ͼʱ˫��������4��ͼ
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

void CMMDlg::toGLCoordinate(CPoint *point)//����Ļ����ת��Ϊ��ͼ��openGL�ӿ�����
{
	point->x-=rcALL.left;
	point->y=rcALL.bottom-point->y;	
}

void CMMDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonUp(nFlags, point);	
	toGLCoordinate(&point);
	int needRenderScene=0;
	short ctrlState=GetKeyState(VK_CONTROL);//��ȡ��ʱcontrol���Ƿ���
	CS *pCS;

	if (m_workMode==0)//����ģʽ
	{
		if (m_create==1.2)//����������ʱ�ȴ���һ���������
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
		if (m_create==2.12)//��������
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

		if (m_create==3.2)//��������
		{
			if(point.x!=m_pp[0][0]||point.y!=m_pp[0][1])
			{
				//���˻�ȡ���������Ϣ
				m_sphere.cx=m_vertex[0][0];
				m_sphere.cy=m_vertex[0][1];
				m_sphere.cz=0;
				m_sphere.R=sqrt((m_vertex[0][0]-m_vertex[1][0])*(m_vertex[0][0]-m_vertex[1][0])+(m_vertex[0][1]-m_vertex[1][1])*(m_vertex[0][1]-m_vertex[1][1]));
				createSphere();			
			}	
			m_create=3;
			needRenderScene=1;
		}
		if (m_create==4.12)//����׶��
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
		if (m_create==5.2)//����׶��
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
		if (m_create==6.2)//����pipe
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
		if (m_create==6.5)//����pipe
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
			if (ctrlState>=0)//û�а���ctrl�����ɴ���
			{
				for (GLuint i=0;i<m_numPointHits;i++) m_pointHits[i]->selected=0;//ȡ����ǰ��ѡ��̬
				m_numPointHits=m_numBuffHits;//����ѡ�е���
				for (i=0;i<m_numBuffHits;i++)
				{
					m_pointHits[i]=m_pointsBuffer[i];//����ָ������
					m_pointHits[i]->selected=1;
				}
			}
			else //������ctrl���ظ��Ļᱻ���
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
					if(j<m_numPointHits) //���ص�
					{
						m_pointHits[j]->selected=0;//����ص����ѡ��
						if(j!=m_numPointHits) m_pointHits[j]=m_pointHits[m_numPointHits-1];//�������һ���㣬��ô�����һ���ָ���Ƶ��ղ���մ�
						m_numPointHits--;//���������
					}
					else //û���ص������뵽β��
					{
						m_pointHits[m_numPointHits]=m_pointsBuffer[i];
						m_numPointHits++;//��������
					}
				}
			}

		}
		if(m_drag==2.1) //û����Ҳû���ƶ�,����κ���ѡ�еĵ�
		{
			if(ctrlState>=0&&m_numPointHits!=0)//û�а���ctrl�������ǰѡ�е�
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
		if(m_drag==2.11&&point.x!=m_pp[0][0]&&point.y!=m_pp[0][1])//������ѡ�϶�,���п�ѡ
		{
			int cx=(point.x+m_pp[0][0])/2,cy=(point.y+m_pp[0][1])/2;
			int w=abs(point.x-m_pp[0][0]),h=abs(point.y-m_pp[0][1]);
			GLuint hits=pickPoints(cx,cy,w,h);
			if (hits>0)
			{
				if (ctrlState>=0)//û�а���ctrl�����ɴ���
				{
					for (GLuint i=0;i<m_numPointHits;i++) m_pointHits[i]->selected=0;//ȡ����ǰ��ѡ��̬
					m_numPointHits=hits;//����ѡ�е���
					for (i=0;i<hits;i++)
					{
						m_pointHits[i]=m_pointsBuffer[i];//����ָ������
						m_pointHits[i]->selected=1;
					}
				}
				else //������ctrl���ظ��Ļᱻ���
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
						if(j<m_numPointHits) //���ص�
						{
							m_pointHits[j]->selected=0;//����ص����ѡ��
							if(j!=m_numPointHits) m_pointHits[j]=m_pointHits[m_numPointHits-1];//�������һ���㣬��ô�����һ���ָ���Ƶ��ղ���մ�
							m_numPointHits--;//���������
						}
						else //û���ص������뵽β��
						{
							m_pointHits[m_numPointHits]=m_pointsBuffer[i];
							m_numPointHits++;//��������
						}
					}
				}
			}
			else //���������ǰѡ�е�
			if(ctrlState>=0&&m_numPointHits!=0)//û�а���ctrl�������ǰѡ�е�
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
		if (m_drag==2.2)//�㵽�˵���û���϶�
		{
			m_drag=2;
		}
		if (m_drag==2.21) //�㵽����������϶�
		{
			m_drag=2;			
		}

		m_drawChooseBox=0;//�ر�ѡ����ͼ
	}

	/***************************************************************/
	if (m_LBdown)//��������Ч�������ʱִ��
	{
		if (m_workMode==1||m_workMode==3)//ѡ��ģʽ��������ģʽ
		{
			GLuint hits;
			
			if (point.x==m_pp[0][0]&&point.y==m_pp[0][1])//û���ƶ�����ѡ
			{
				hits=pickObjects(point.x-1,point.y-1,point.x+1,point.y+1);
				if (hits==0) //δѡ��
				{
					if (ctrlState>=0)//û�а���control,�������岻ѡ��
					{
						pCS=m_cs_head;
						while (pCS->next)
						{
							pCS=pCS->next;
							if (pCS->m_selected)
							{
								pCS->m_selected=0;
								needRenderScene=1;//�ı�ѡ�����ػ�
							}
						}
					}
				}
				else //������ѡ��
				{
					if (ctrlState<0)//��ѡ��ֻ�������������
					{
						pCS=m_cs_head;
						while (pCS->next)
						{
							pCS=pCS->next;
							if (pCS->m_index==m_hits[0])
							{
								pCS->m_selected=!pCS->m_selected;
								needRenderScene=1;//�ı�ѡ�����ػ�
							}
						}
					}
					else //��ѡ,ʹ��Զ�����屻ѡ��
					{
						GLuint sel,i;
						if (hits==1) sel=m_hits[0];
						else //ͬʱ��ֻһ������ѡ��
						{
							//�ɽ���Զɨ�裬�ҵ���Զ��ѡ����
							for (i=hits;i>0;i--)
							{
								if (m_hits[2*i-1]) break;
							}
							if (i==hits||i==0) sel=m_hits[0];//��Զ��ѡ��,���δѡ�У����������
							else sel=m_hits[2*i];
						}
						//�����õ���Ҫѡ����
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
			else //���ƶ�����ѡ
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
								needRenderScene=1;//�ı�ѡ�����ػ�
							}
						}
					}
					else //��ѡ��
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
									if (pCS->m_selected==0)//��δ��ѡ��
									{
										pCS->m_selected=1;
										needRenderScene=1;//�ı�ѡ�����ػ�
									}	
									break;			
								}
							}
							if(i==hits&&pCS->m_selected)//û�з��ϵģ�ʹ�䲻ѡ��
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
		if (m_workMode==2||m_workMode==4) //�ƶ�ģʽ,����תģʽ
		{
			if (point.x==m_pp[0][0]&&point.y==m_pp[0][1])//û���ƶ�
			{
				if (m_BoxChoose)//û�е㵽�κ����壬ȡ����ѡ�������������ѡ��
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
				if (m_move>1)//�����ص�ѡ��
				{
					GLuint sel,i,hits=m_move-1;
					for (i=hits;i>0;i--)
					{
						if (m_hits[2*i-1]) break;
					}
					if (i==hits||i==0) sel=m_hits[0];//��Զ��ѡ��,���δѡ�У����������
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
			else//�ƶ���
			{
				if (m_BoxChoose)//���п�ѡ
				{
					needRenderScene=1;
					if(point.x!=m_pp[0][0]&&point.y!=m_pp[0][1])
					{
						GLuint hits=pickObjects(m_pp[0][0],m_pp[0][1],point.x,point.y);
						if (hits==0) //ȫ��δѡ��
						{
							pCS=m_cs_head;
							while (pCS->next)
							{
								pCS=pCS->next;
								if (pCS->m_selected)
								{
									pCS->m_selected=0;
									needRenderScene=1;//�ı�ѡ�����ػ�
								}
							}
						}
						else //��ѡ��
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
										if (pCS->m_selected==0)//��δ��ѡ��
										{
											pCS->m_selected=1;
											needRenderScene=1;//�ı�ѡ�����ػ�
										}	
										break;			
									}
								}
								if(i==hits&&pCS->m_selected)//û�з��ϵģ�ʹ�䲻ѡ��
								{
									pCS->m_selected=0;
									needRenderScene=1;
								}				
							}
						}
					}
				}
				if (m_move)//Ҫ��¼�ƶ�
				{
					pCS=m_cs_head;
					while (pCS->next) //����ѡ�����崢��ģ����ͼ����
					{
						pCS=pCS->next;
						if (pCS->m_selected) pCS->storeMVMatrix();		
					}
				}
			}
			if (m_drawRotateCross)//��������תʮ�ּ�
			{
				m_drawRotateCross=0;
				needRenderScene=1;
			}
			m_move=0;//�ر��ƶ���ʶ
		}
		
		checkSelected();
		m_LBdown=0;
		m_BoxChoose=0;//�ر�ѡ���ʶ
		m_drawChooseBox=0;//������ѡ���
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
		if(point.x!=m_pp[0][0]&&point.y!=m_pp[0][1]&&m_chooseDensify==2)//��Ч�ƶ�
		{
			//���п�ѡ
			GLuint hits=pickTriangles((point.x+m_pp[0][0])/2,(point.y+m_pp[0][1])/2,abs(point.x-m_pp[0][0]),abs(point.y-m_pp[0][1]));
		}
		m_drawChooseBox=0;//�ر�ѡ���
		needRenderScene=1;
		m_chooseDensify=1;
	}

	m_pp[1][0]=point.x;
	m_pp[1][1]=point.y;//��¼�ڶ�ƽ���
	if(needRenderScene==1) renderScene();
}

///////////////////////������Ӧ����////////////////////////////
void CMMDlg::OnUndo() 
{	
	pickPoints(800,400,200,200);//ѡ��
}

void CMMDlg::OnRedo() 
{
	
}

void CMMDlg::OnSelect() //����ѡ��ģʽ
{
	m_workMode=1;
	renderScene();
	m_page2.unCheck();
}

void CMMDlg::OnSelectMove() //�ƶ�ģ��
{
	m_workMode=2;
	renderScene();
	m_page2.unCheck();
}

void CMMDlg::OnSelectSize() //ѡ�񲢾�������
{
	m_workMode=3;
	renderScene();
	m_page2.unCheck();
}

void CMMDlg::OnRotate() //ѡ����ת����
{
	m_workMode=4;
	renderScene();
	m_page2.unCheck();
}

void CMMDlg::OnNewfile() //�½�����������Ҫ����ɳ���
{
	CS *pCS=m_cs_head,*q;
	if (m_num_model)
	{
		int ans=MessageBox("ģ���ļ��Ѿ����޸ģ��Ƿ񱣴�?\nyes������\nno:������\ncancel:�����½�","������ʾ:",MB_YESNOCANCEL);
		if(ans==IDCANCEL) return;
		else if(ans==IDYES) //����ɳ���
		{
			
		}
	}
	//ɾ���ɳ���
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

void CMMDlg::GLTextOut(float x,float y,float z,char * const textstring) //����ַ���
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
	short ctrlState=GetKeyState(VK_CONTROL);//��ȡ��ʱcontrol���Ƿ���
	/*****************************************************************/
	if (m_num_view==4)//����ͼʱ�л������
	{
		for (i=0;i<4;i++)
		{
			if(inRect(rc[i],point)) //���ڵ�i������
			{
				//���������Ͻ����֣��л���Ⱦ��ʽ
				if (rc[i].left<point.x&&point.x<rc[i].left+AREA&&rc[i].top-AREA<point.y&&point.y<rc[i].top)
				{
					m_viewManner[i]=!m_viewManner[i];
					needRenderScene=1;
				}
				//���类��������ǻ�ģ������Ϊ���
				if(m_activeView!=i) 
				{
					m_activeView=i;
					needRenderScene=1;
				}				
				break;
			}
		}
	}
	else //����ͼ
	//���������Ͻ����֣��л���Ⱦ��ʽ
	if (0<point.x&&point.x<AREA&&rcALL.Height()-AREA<point.y&&point.y<rcALL.Height())
	{
		m_viewManner[m_activeView]=!m_viewManner[m_activeView];
		needRenderScene=1;
	}
	/************************�����빤��ģʽ�й�*******************************/
	if (inDrawArea(point))
	{
		
		//�������ͼ�����ȱ������ָ���	
		if (m_page1.GetDlgItem(IDC_NAME)->IsWindowEnabled()!=0) updateName();	
		CS *pCS;
		if (m_workMode==0)//����ģʽ
		{
			byte aim;
			if (m_num_view==4) aim=m_activeView;
			else aim=4;
			if (m_create==1)//����������ʱ�ȴ���һ���������
			{
				unSelectALL();
				//��ȡ���߷ֶ�
				m_cuboid.nx=10-m_page1.m_cuboidPanel.m_nx.GetTopIndex();
				m_cuboid.ny=10-m_page1.m_cuboidPanel.m_ny.GetTopIndex();
				m_cuboid.nz=10-m_page1.m_cuboidPanel.m_nz.GetTopIndex();
				if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//��3D���ڴ�������øõ��3D����
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
				if (m_activeView==1)//���ӿ�1����
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					m_vertex[0][2]=0;
				}
				if (m_activeView==2)//���ӿ�2����
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					m_vertex[0][0]=0;
				}
				if (m_activeView==3)//���ӿ�3����
				{
					gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
					m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
					m_vertex[0][1]=0;
				}
				m_create=1.1;//�����ƶ�������δ�ƶ�
			}
			if (m_create==1.4)//����������ʱ�ȴ��ڶ����������
			{
				if (point.y!=m_pp[1][1])
				{
					//��ȡ��������ͱ߳�
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

			if (m_create==2)//��������
			{
				//��ȡ����
				m_cylinder.n=ND_MAX-m_page1.m_cylinderPanel.m_DN.GetTopIndex();
				if(((CButton *)m_page1.m_cylinderPanel.GetDlgItem(IDC_REGULAR_POLYGON))->GetCheck()==1)
					m_cylinder.type=0;
				else m_cylinder.type=1;
				m_cylinder.nr=20-m_page1.m_cylinderPanel.m_NR.GetTopIndex();
				m_cylinder.nh=20-m_page1.m_cylinderPanel.m_NH.GetTopIndex();//��÷ֶ���
				unSelectALL();
				if (m_cylinder.type==0)//��������������Σ���ȡ���ĵ�
				{					
					if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//��3D���ڴ�������øõ��3D����
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
					m_create=2.11;//ָʾ�����ƶ�״̬
				}
				else //�ױ�����������
				{
					if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//��3D���ڴ�������øõ��3D����
					{
						gluUnProject((GLdouble)point.x,(GLdouble)point.y,0,m_modelMatrix[m_activeView],
							m_projMatrix[aim],m_port[aim],&m_vertex[0][0],&m_vertex[0][1],&m_vertex[0][2]);
						double ax=sin(m_theta*PI/180.0)*cos(m_phi*PI/180.0),
							ay=sin(m_theta*PI/180.0)*sin(m_phi*PI/180.0),
							az=cos(m_theta*PI/180.0);
						//��ȡ��ʼ��
						m_vertex[0][0]=m_vertex[0][0]-ax/az*m_vertex[0][2];
						m_vertex[0][1]=m_vertex[0][1]-ay/az*m_vertex[0][2];
						m_vertex[0][2]=0;
						//������һ����					
						m_cylinder.ehead=new Edge;
						Edge *p=new Edge;
						m_cylinder.ehead->next=p;
						m_cylinder.end=p;//�ƶ�ʱֻ���޸�β�������Ժ�����
						m_cylinder.numEdge=0;//����Ϊ0
						p->x1=p->x2=m_vertex[0][0];
						p->y1=p->y2=m_vertex[0][1];
						p->z1=0;
						p->next=NULL;
					}
					m_create=2.2;//ָʾ����㻭�ߵ�״̬,LBdown�����߶�ͷ
				}
			}
			else if (m_create==2.14)
			{
				if (point.y!=m_pp[1][1]) //���ݲɼ����
				{
					//��ȡ���������R,H
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
				if (m_cylinder.numEdge==0)//��һ���ߣ������
				{
					Edge *p;
					m_cylinder.end->next=p=new Edge;//�½��ڵ�
					p->next=NULL;
					m_cylinder.numEdge++;//�½�һ����
					p->x1=m_cylinder.end->x2;
					p->y1=m_cylinder.end->y2;
					p->z1=0;
					p->x2=m_cylinder.end->x2;
					p->y2=m_cylinder.end->y2;
					p->z2=0;
					m_cylinder.end=p;//ָ����β
				}
				else if(m_cylinder.numEdge==1) //�ڶ����ߣ�ֻ���ĩ���Ƿ��ڵ�һ������
				{
					byte r=m_p2t.pointOnEdge(m_cylinder.ehead->next,m_cylinder.end);
					if(r==0)
					{
						Edge *p;
						m_cylinder.end->next=p=new Edge;//�½��ڵ�
						p->next=NULL;
						m_cylinder.numEdge++;//�½�һ����
						p->x1=m_cylinder.end->x2;
						p->y1=m_cylinder.end->y2;
						p->z1=0;
						p->x2=m_cylinder.end->x2;
						p->y2=m_cylinder.end->y2;
						p->z2=0;
						m_cylinder.end=p;//ָ����β
					}
					else MessageBox("��һ���ߺ͵ڶ����߲���������");
				}
				else if(m_cylinder.numEdge>=2) //3~N���ߣ��������
				{
					byte r=m_p2t.checkEdge(m_cylinder.ehead);//������һ�����Ƿ����
					if(r)//����
					{
						if(r==1)
						{
							Edge *p;
							m_cylinder.end->next=p=new Edge;//�½��ڵ�
							p->next=NULL;
							m_cylinder.numEdge++;//�½�һ����
							p->x1=m_cylinder.end->x2;
							p->y1=m_cylinder.end->y2;
							p->z1=0;
							p->x2=m_cylinder.end->x2;
							p->y2=m_cylinder.end->y2;
							p->z2=0;
							m_cylinder.end=p;//ָ����β
						}
						else //�����ݲɼ�����
						{
							m_cylinder.numEdge++;//�˱߷���
							m_create=2.21;
						}
					}
					else //�߲���������
					{
						MessageBox("�߲������벻���ڵı��ཻ��");
					}						
				}				
			}
			else if (m_create==2.21) //�õ��߶�
			{
				m_create=2;
				createCylinder();//�ɲɼ������ݲ����ʷֶ����	
				needRenderScene=1;
			}

			if (m_create==3)//��������
			{
				m_sphere.nj=ND_MAX-m_page1.m_spherePanel.m_NJ.GetTopIndex();
				m_sphere.nw=ND_MAX-m_page1.m_spherePanel.m_NW.GetTopIndex();//��÷ֶ���	
				unSelectALL();
				if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//��3D���ڴ�������øõ��3D����
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
				m_create=3.1;//ָʾ�����ƶ�״̬
			}
			if (m_create==4)//����׶��
			{
				//��ȡ����
				m_cylinder.n=ND_MAX-m_page1.m_conePanel.m_DN.GetTopIndex();
				if(((CButton *)m_page1.m_conePanel.GetDlgItem(IDC_REGULAR_POLYGON))->GetCheck()==1)
					m_cylinder.type=0;
				else m_cylinder.type=1;
				m_cylinder.nr=20-m_page1.m_conePanel.m_NR.GetTopIndex();
				m_cylinder.nh=20-m_page1.m_conePanel.m_NH.GetTopIndex();//��÷ֶ���
				if (m_cylinder.type==0)//��������������Σ���ȡ���ĵ�
				{
					unSelectALL();
					if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//��3D���ڴ�������øõ��3D����
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
					m_create=4.11;//ָʾ�����ƶ�״̬
				}
				else
				{
					
				}
			}
			if (m_create==4.14) //����׶��
			{
				if (point.y!=m_pp[1][1]) //���ݲɼ����
				{
					//��ȡ���������R,H
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

			if (m_create==5)//������̥ʱ�ȴ���һ���������
			{
				unSelectALL();
				//��ȡ�ֶ�
				m_torus.nj=ND_MAX-m_page1.m_torusPanel.m_nj.GetTopIndex();
				m_torus.nw=ND_MAX-m_page1.m_torusPanel.m_nw.GetTopIndex();
				if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//��3D���ڴ�������øõ��3D����
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
				m_create=5.1;//�����ƶ�������δ�ƶ�
			}
			if (m_create==5.4)//������̥ʱ�ȴ��ڶ����������
			{
				if (point.y!=m_pp[1][1])
				{
					//��ȡ����
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
			if (m_create==6)//�������ӵȴ���һ���������
			{
				unSelectALL();
				//��ȡ�ֶ�
				m_pipe.n=ND_MAX-m_page1.m_pipePanel.m_N.GetTopIndex();
				m_pipe.nh=ND_MAX-m_page1.m_pipePanel.m_NH.GetTopIndex();
				m_pipe.nr=ND_MAX-m_page1.m_pipePanel.m_NR.GetTopIndex();
				if (m_activeView==0&&m_theta!=90&&m_theta!=0&&m_theta!=180)//��3D���ڴ�������øõ��3D����
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
				m_create=6.1;//�����ƶ�������δ�ƶ�
			}
			if (m_create==6.4)//�������ӵȴ��ڶ����������
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
			if (m_create==6.7)//����pipe
			{
				if(point.y!=m_pp[1][1])
				{
					if (m_activeView==0)
					{
						//��ȡȫ������
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
		if (m_workMode==1||m_workMode==3)//ѡ��ģʽ������ȷ���
		{	
			m_LBdown=1;
		}
		/***********************************************************/
		if (m_workMode==2||m_workMode==4)//�ƶ�ģʽ������תģʽ
		{
			
			GLuint hits=pickObjects(point.x-1,point.y-1,point.x+1,point.y+1);
			if (hits==0)
			{
				m_BoxChoose=1;//������ѡ
			}
			else
			{
				GLuint i=0;
				for (i=0;i<hits;i++)
				{
					if(m_hits[2*i+1]) break;
				}
				
				if(i!=hits)//�е��Ѿ�ѡ��
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
					else m_move=hits+1;//m_move����Ϊ2���������������
				}
				else //�����û�б�ѡ��,�������һ��ѡ��
				{
					
					pCS=m_cs_head;
					while(pCS->next)
					{
						pCS=pCS->next;
						if(pCS->m_index==m_hits[0]) pCS->m_selected=1;				
						else if(ctrlState>=0) pCS->m_selected=0;//����ctrl�Ƿ���
					}
					needRenderScene=1;
					m_move=1;
				}
			}
			m_LBdown=1;
		}
		/***********************************************************/
		if (m_workMode==5)//���϶�ģʽ
		{
			m_lastMovePP[0]=point.x;
			m_lastMovePP[1]=point.y;
			if(m_drag==1) //��ѡ
			{
				byte curseW=m_page2.m_cursorW,curseH=m_page2.m_cursorH;
				GLuint hits=pickPoints(point.x,point.y,curseW,curseH);
				if (hits==0)//δ����
				{
					if(ctrlState>=0)//û�а���ctrl�������ǰѡ�е�
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
				else //�е���
				{
					bool used=0;
					GLuint i=0,j=0,ok=0;
					if (m_numPointHits!=0)//��ǰ��ѡ��
					{
						for (GLuint i=0;i<hits;i++) //����Ƿ�㵽��ǰѡ�е�
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
					if (used)//������ǰ��
					{
						m_drag=1.2;//����LButtonUpʱ������ѡ
						m_numBuffHits=hits;//������ѡ�еĵ���
					}
					else //����ѡ�еĲ�������ǰѡ�е�
					{
						m_drag=1.1;//upʱ������ѡ
						needRenderScene=1;
						if(ctrlState>=0)//û�а���ctrl�����´���
						{
							for (GLuint i=0;i<m_numPointHits;i++)	m_pointHits[i]->selected=0;//ȡ����ǰ��ѡ��̬
							m_numPointHits=hits;//����ѡ�е���
							for (i=0;i<hits;i++)
							{
								m_pointHits[i]=m_pointsBuffer[i];//����ָ������
								m_pointHits[i]->selected=1;//ʹ�µ�����ѡ��
							}
						}
						else //������ctrl�����뵽����
						{
							for (i=0;i<hits;i++)
							{
								m_pointHits[i+m_numPointHits]=m_pointsBuffer[i];//����ָ������
								m_pointsBuffer[i]->selected=1;//ʹ�µ�����ѡ��
							}
							m_numPointHits+=hits;
						}
					}					
				}
			}
			if(m_drag==2) //��ѡ
			{
				GLuint hits=pickPoints(point.x,point.y,8,8);
				if (hits==0) m_drag=2.1;//����
				else m_drag=2.2;//�ƶ���
			}
		}
		/***********************************************************/
		if (m_workMode==6) //�и�ģʽ: �ƶ�����ת����
		{
			if (m_knife.state==1)//�ƶ�
			{
				//����Ƿ�����knife
				if (pickKnife(point.x,point.y))	m_knife.state=1.1;
			}
			if (m_knife.state==2)//��ת
			{
				if (pickKnife(point.x,point.y))	m_knife.state=2.1;	
			}
		}
		/***********************************************************/
		if (m_workMode==7.1)//ѡ���Լ���ģʽ
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

		checkSelected();//�������ֺ���ɫ
		m_pp[0][0]=point.x;
		m_pp[0][1]=point.y;//��¼�����ƽ��λ��
	}
	if (needRenderScene) renderScene();	
}

void CMMDlg::calculateRc()//������ʾ�����λ��
{
	GetDlgItem(IDC_ALL)->GetWindowRect(&rcALL);// ��ȡ��ͼ����λ��
	ScreenToClient(rcALL);
	int length=rcALL.right-rcALL.left,width=rcALL.bottom-rcALL.top;
	if (m_view_style==0)//4�����ȵĴ���
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
	else if (m_view_style==1)//3��1С
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
	//����4��С��ͼ���ӿھ����ͶӰ����
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
	//���µ���ͼ���ӿھ����ͶӰ����
	glLoadIdentity();
	float ratio=(float)rcALL.Width()/rcALL.Height();
	if(ratio>1)	glOrtho(-Lw*ratio,Lw*ratio,-Lw,Lw,-10*Lw,10*Lw);
	else glOrtho(-Lw,Lw,-Lw/ratio,Lw/ratio,-10*Lw,10*Lw);
	glViewport(0,0,rcALL.Width(),rcALL.Height());
	glGetIntegerv(GL_VIEWPORT, m_port[4]);
	glGetDoublev(GL_PROJECTION_MATRIX, m_projMatrix[4]);
	//m_update[0]=m_update[1]=m_update[2]=m_update[3]=1;//һ��calculateRc(),�ĸ������е��ƽ�����궼Ҫ����
}

void CMMDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	CWnd *pALL= GetDlgItem(IDC_ALL);     
	CWnd *pTab= GetDlgItem(IDC_TAB1); 
	CWnd *pState=GetDlgItem(IDD_PANEL_STATE);//��ȡ�ؼ����
    if(nType==SIZE_MINIMIZED) return;   //����Ǵ�����С����ʲô������
    if(pALL)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
    {
		int dx=cx-m_old_rect.Width(),dy=cy-m_old_rect.Height();
		CRect rectALL,rectTab,rectState;   //��ȡ�ؼ��仯ǰ��С
		pALL->GetWindowRect(&rectALL);
		pTab->GetWindowRect(&rectTab);
		m_statePanel.GetWindowRect(&rectState);
		ScreenToClient(&rectALL);
		ScreenToClient(&rectTab);
		ScreenToClient(&rectState);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rectALL.right+=dx;
		rectALL.bottom+=dy;
		rectTab.left+=dx;
		rectTab.right+=dx;
		rectTab.bottom+=dy;
		rectState.right+=dx;
		rectState.top+=dy;
		rectState.bottom+=dy;
		pALL->MoveWindow(rectALL);
		pTab->MoveWindow(rectTab);//���ÿؼ���С
		m_statePanel.MoveWindow(rectState);
		
		calculateRc();
		m_updateProjMatrix=1;//��ʶ��Ҫ����ͶӰ����
    }
    GetClientRect(&m_old_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С
}

bool CMMDlg::inRect(Rect &rc,CPoint &p)//�ж��Ƿ���������
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
		if (rc.left<p.x&&p.x<rc.left+AREA&&rc.top-AREA<p.y&&p.y<rc.top) return 2;//���Ͻ�
		else if (rc.right-AREA<p.x&&p.x<rc.right&&rc.top-AREA<p.y&&p.y<rc.top) return 3;//���Ͻ�
		else return 1;//��ͼ��
	}
	else return 0; //�����κ�����
}

void CMMDlg::exchangeRect(Rect &a,Rect &b) //������ͼ
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

void CMMDlg::changeViewStyle()//�ı���ͼ�ֲ��ķ��
{
	if(m_view_style==0) m_view_style=1;
	else m_view_style=0;
	calculateRc();
	renderScene();
}

void CMMDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{	
	CDialog::OnRButtonDown(nFlags, point);
	short ctrlState=GetKeyState(VK_CONTROL);//��ȡ��ʱcontrol���Ƿ���
	//�Ҽ��˵�׼��
	CPoint ps;
	ps.x=point.x;
	ps.y=point.y;
	CMenu myMenu ;
	myMenu.LoadMenu(IDR_MENU_POPUP) ;
	CMenu *pMenu = myMenu.GetSubMenu(0);//ֻ��ʾ��һ���Ӳ˵�
	ClientToScreen( &ps ) ;
	/***************************���ͼ�л�******************************/
	toGLCoordinate(&point);
	m_rpp[1][0]=point.x;
	m_rpp[1][1]=point.y;//�����Ҽ�����		
	short i=0;
	bool needRenderScene=0;
	if (m_num_view==4)//����ͼʱ�л������
	{
		for (i=0;i<4;i++)
		{
			if(inRect(rc[i],point)&&m_activeView!=i) //���ڵ�i������
			{
				m_activeView=i;
				needRenderScene=1;	
			}
			break;
		}
	}
	GLuint hits;
	if (inDrawArea(point)&&m_workMode!=5)//�༭ģʽ������ѡ��
	{
		hits=pickObjects(point.x-1,point.y-1,point.x+1,point.y+1);
		if (hits>0&&m_hits[1]==0) //ѡ���˵㣬�������֮ǰδ��ѡ��
		{			
			CS *pCS=m_cs_head;
			while (pCS->next)
			{
				pCS=pCS->next;
				if(pCS->m_index==m_hits[0]) pCS->m_selected=1;
				else if(ctrlState>=0) pCS->m_selected=0;//��ѡʱ�������彫��ѡ��
			}
			needRenderScene=1;
		}
	}
	if(needRenderScene) renderScene();
	if(inDrawArea(point)) pMenu->TrackPopupMenu(TPM_LEFTALIGN,ps.x, ps.y,this);
}

void CMMDlg::OnRButtonDblClk(UINT nFlags, CPoint point) //�Ҽ�˫�����л��ӽ�
{
	CDialog::OnRButtonDblClk(nFlags, point);
	toGLCoordinate(&point);
	bool needRenderScene=0;
	if (m_num_view==4)//����ͼʱ
	for(short i=1;i<4;i++)
	{
		if (rc[i].left<point.x&&point.x<rc[i].left+AREA&&rc[i].top-AREA<point.y&&point.y<rc[i].top)
		{
			m_viewSide[i]=!m_viewSide[i];
			//����i�Ŵ�����ͼģ�;���
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
	else //����ͼ
	//���������Ͻ����֣��л���Ⱦ��ʽ
	if (0<point.x&&point.x<AREA&&rcALL.Height()-AREA<point.y&&point.y<rcALL.Height()&&m_activeView!=0)
	{
		m_viewSide[m_activeView]=!m_viewSide[m_activeView];
		//����m_activeView�Ŵ�����ͼģ�;���
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

double CMMDlg::random()//���������
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
		if (p->m_selected)//ѡ���߸ı���ɫ
		{
			p->setColor(m_activeColor);
		}
	}
	renderScene();//�ػ�
}

GLint CMMDlg::pickObjects(GLint x1,GLint y1,GLint x2,GLint y2)//��ָ����������ѡ������
{	
	short aim;//���ĸ���۲�
	GLuint hits;
	float size; //ѡ����ͼ�ķ���
	GLint viewport[4];
	CPoint point;
	point.x=x1;
	point.y=y1;
	g_renderMode=0;//�л�����ѡģʽ��ʹһЩ����������
	if (m_num_view==4)//����ͼ
	{
		for (short i=0;i<4;i++)
		{
			if(inRect(rc[i],point)) //���ڵ�i������
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
	else //����ͼ
	{
		viewport[0]=viewport[1]=0;
		viewport[2]=rcALL.Width();
		viewport[3]=rcALL.Height();												
		aim=m_activeView+3*m_viewSide[m_activeView];		
		size=m_size[m_activeView];
	}			
	glRenderMode(GL_SELECT);//�л�Ϊѡ��ģʽ
	glInitNames();
	glPushName(0);//��ʼ�����ֶ�ջ
	glMatrixMode(GL_PROJECTION);//�л���ͶӰ����
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
	/**************��ѡ�е������ڲ���ʱ���Ƶ�������*************/
	while(pCS->next)
	{
		pCS=pCS->next;
		if (pCS->m_visible) pCS->show(m_viewManner[m_activeView],m_workMode);
	}
	hits=glRenderMode(GL_RENDER);//�л�����Ⱦģʽ
	g_renderMode=1;
	if (hits==1) m_hits[0]=m_selectBuff[3];
	if (hits>1)
	{
		GLuint ta,tb;
		m_hits[0]=m_selectBuff[3];
		m_hits[1]=m_selectBuff[1];//��ѹ��һ��ֵ
		for (GLuint i=1;i<hits;i++) //ѹ��һ��ֵ��Ȼ��Ƚ���ȴ�С��������
		{
			m_hits[2*i]=m_selectBuff[3+i*4];
			m_hits[2*i+1]=m_selectBuff[1+i*4];
			for (short j=i;j>1;j--)
			{
				if(m_hits[2*j+1]<m_hits[2*j-1]) //��Ի�
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
	//�ж�ÿ�������Ƿ�ѡ��
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
		if (count==hits) break;//����ѡ��������Ϣ������ȡ
	}
	return hits;
}

bool CMMDlg::inDrawArea(CPoint &p)//�ж��Ƿ����ڻ�ͼ��
{
	if (m_num_view==4)
	{
		for (short i=0;i<4;i++)
		{
			if (howInRect(rc[i],p)==1) 	return TRUE;
		}
	}
	else //����ͼ
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

void CMMDlg::checkSelected()//���ѡ��������������ֺ���ɫ
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
		s.Format("ѡ����%d������",count);
		pName->SetWindowText(s);
		pName->EnableWindow(FALSE);
	}
}

void CMMDlg::updateName()//���¶������ɫ
{	
	CS *pCS=m_cs_head;
	while (pCS->next)//���ұ�ѡ�����
	{
		pCS=pCS->next;
		if (pCS->m_selected) break;
	}
	CString s;
	m_page1.GetDlgItem(IDC_NAME)->GetWindowText(s);
	if(s!="") pCS->m_name=s;
}

void CMMDlg::createCuboid()//����������
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//�ƶ������
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createCuboid(m_cuboid);
	pCS->setColor(m_randomColor);
	getRandomColor();//�趨�µ����ɫ
	m_maxIndex++;
	pCS->m_index=m_maxIndex;
	m_num_model++;	
	renderScene();
}

void CMMDlg::createCone()//����׵��
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//�ƶ������
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createCone(m_cylinder);
	pCS->setColor(m_randomColor);
	getRandomColor();//�趨�µ����ɫ
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::createSphere()//��������
{

	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//�ƶ������
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createSphere(m_sphere);
	pCS->setColor(m_randomColor);
	getRandomColor();//�趨�µ����ɫ
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::createCylinder()//��������
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//�ƶ������
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createCylinder(m_cylinder);
	pCS->setColor(m_randomColor);
	getRandomColor();//�趨�µ����ɫ
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::createTorus()//��������
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//�ƶ������
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createTorus(m_torus);
	pCS->setColor(m_randomColor);
	getRandomColor();//�趨�µ����ɫ
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::createPipe()//������״��
{
	CS *pCS=m_cs_head;
	while(pCS->next) pCS=pCS->next;//�ƶ������
	pCS->next=new CS;
	pCS=pCS->next;
	pCS->next=NULL;
	pCS->createPipe(m_pipe);
	pCS->setColor(m_randomColor);
	getRandomColor();//�趨�µ����ɫ
	m_maxIndex++;
	pCS->m_index=m_maxIndex;	
	m_num_model++;	
	renderScene();
}

void CMMDlg::drawNewObject()//�������̻�������
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
	if(m_create==2.2)//�㻮�߹���ģʽ
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
	if(m_create==2.21)//�㻮��+��
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

bool CMMDlg::isHotKey(WPARAM VirKey)//�ж��Ƿ����ڿ�ݼ�
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

COLORREF CMMDlg::getRandomColor()//�����ɫ
{
	byte i=63*random();
	return m_randomColor=RGB(m_colors[i][0],m_colors[i][1],m_colors[i][2]);
}

void CMMDlg::unSelectALL()//ʹ��������ȫ��ѡ��
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
	case IDR_MENU_FILE_EXIT:   //���ټ�ID   
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
				pCS->m_selected=0;//��ѡ��
				pCS->m_visible=0;//���ɼ�
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
				pCS->m_visible=1;//ʹ��ɼ�
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
				pCS->m_free=0;//ʹ�䶳�ᣬ���ɱ༭
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
				pCS->m_free=1;//������ᣬʹ�ɱ༭
			}				
		}
		renderScene();
		break;
	case ID_DELETE_OBJECT:
		deleteObject();
		renderScene();
		break;
	case ID_COPY:
		//��Ҫ���Ƶ�ģ��ָ��д�뻺����
		m_numToCopy=0;
		pCS=m_cs_head;
		while(pCS->next)
		{
			pCS=pCS->next;
			if(pCS->m_selected)
			{
				if(m_numToCopy==MAX_COPY_INDEX) MessageBox("Ҫ���Ƶĸ����������ֵ!");
				m_copyPointer[m_numToCopy]=pCS;
				m_numToCopy++;
			}
		}
		m_rpp[0][0]=m_rpp[1][0];
		m_rpp[0][1]=m_rpp[1][1];//���µ������ʱ�Ҽ�����
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
				m_copyPointer[i]->m_selected=0;//ȡ��ԭ����ѡ��
				m_num_model++;//ģ��������
				m_maxIndex++;				
				pCS->m_index=m_maxIndex;//�����
				pCS->m_name+="_����";
				pCS->m_selected=1;//ѡ��
				if (m_activeView==0&&m_theta!=90||m_activeView==1)//��3D���ںʹ���1
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
			//�Ҽ���ѡ
			GLuint	hits=pickObjects(m_rpp[1][0]-1,m_rpp[1][1]-1,m_rpp[1][0]+1,m_rpp[1][1]+1);
			if (hits>0) //ѡ���˵�
			{			
				CS *pCS=m_cs_head;
				while (pCS->next)
				{
					pCS=pCS->next;
					if(pCS->m_index==m_hits[0])
					{
						pCS->m_selected=1;
						pActiveCS=pCS;//�ı����ڱ༭�Ķ���
					}
					else 
					{
						pCS->m_selected=0;//��ѡʱ�������彫��ѡ��
						pCS->m_visible=0;//������������
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
	case ID_EXPORT_CS://����Ϊsmm�ļ�
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
			if(num>1) MessageBox("��Ǹ��һ��ֻ�ܵ���һ������");
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
			if (m_num_model==0)//�Զ����봴���ȴ�
			{
				m_workMode=0;
				m_create=0;
			}
		}
		else pCS=pTemp;
	}
}

GLint CMMDlg::pickPoints(GLint x,GLint y,GLint w,GLint h)//ѡ��
{
	GLuint hits;
	GLdouble modelMatrix[16];
	byte aim;
	if (m_num_view==4) aim=m_activeView;
	else aim=4;		
	glRenderMode(GL_SELECT);//�л�Ϊѡ��ģʽ
	glInitNames();
	glPushName(0);//��ʼ�����ֶ�ջ
	glMatrixMode(GL_PROJECTION);//�л���ͶӰ����
	glLoadIdentity();
	gluPickMatrix(x,y,w,h,m_port[aim]); 
	glMultMatrixd(m_projMatrix[aim]);//�ȳ����Ѵ������ͼ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(m_modelMatrix[m_activeView]); //�����ӽ�		
	/*******************************************/
	CS *pCS=m_cs_head;
	GLuint index=0;	
	while(pCS->next)
	{
		pCS=pCS->next;
		if (pCS->m_visible) 
		{
			//�����ƶ�����ת
			glPushMatrix();
			glTranslated(pCS->m_translate[0],pCS->m_translate[1],pCS->m_translate[2]);//���ƽ��
			glMultMatrixd(pCS->m_mvMatrix);//�ٳ����Ѵ������ת����		
			glScaled(pCS->m_size,pCS->m_size,pCS->m_size);//�Ƚ��з���
			
			VertexRef *p=pCS->iv;			
			while (p->next)
			{
				p=p->next;
				//���õ�Ĵ�С
				index++;
				glLoadName(index);//�滻����
				glBegin(GL_POINTS);
				glVertex3d(p->v[0],p->v[1],p->v[2]);
				glEnd();
			}		
			glPopMatrix();
		} 
	}	
	glFinish();
	hits=glRenderMode(GL_RENDER);//�л�����Ⱦģʽ
	if(hits==0) return hits;
	if(hits>1024) 
	{
		MessageBox("ѡ�������");
		return 0;
	}
	GLuint buffCur=1;//ָ���¼�����������
	GLuint pointBuffCur=0;//�����������
	GLdouble wx,wy,wz;//ƽ������
	float depth;//ʵ����ʾ�����
	index=0;
	pCS=m_cs_head;
	while(pCS->next)
	{
		pCS=pCS->next;
		if (pCS->m_visible) 
		{
			glPushMatrix();
			//�����ƶ�����ת
			glTranslated(pCS->m_translate[0],pCS->m_translate[1],pCS->m_translate[2]);
			glMultMatrixd(pCS->m_mvMatrix);
			glScaled(pCS->m_size,pCS->m_size,pCS->m_size);
			VertexRef *p=pCS->iv;
			while (p->next)
			{
				p=p->next;
				index++;
				if(index==m_selectBuff[4*buffCur-1])//���ڱ���¼������,pָ��ѡ����
				{	
					glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);//��ȡ��ͼģ�;��󣬱��ڷ�ͶӰ
					//����Ƿ�ɼ�����Ϊ���ܱ����浲ס
					gluProject(p->v[0],p->v[1],p->v[2],modelMatrix,
						m_projMatrix[aim],m_port[aim],&wx,&wy,&wz);
					glReadPixels((int)wx,(int)wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
					if(fabs(wz-depth)<=0.005) //���ܿ���p��,ѹ���¼ָ������//���ֵ��С��������
					{
						m_pointsBuffer[pointBuffCur]=p;
						pointBuffCur++;
					}		
					if(buffCur==hits) return pointBuffCur;//ѡ��ĵ��Ѿ�ɨ����ϣ�ֱ������
					buffCur++;
				}
			}
			glPopMatrix();
		}
	}
	return pointBuffCur;
}

GLint CMMDlg::pickTriangles(GLint x,GLint y,GLint w,GLint h)//��ѡ������,�����ģʽ�����m_hits
{
	byte aim;
	if (m_num_view==4) aim=m_activeView;
	else aim=4;		
	glRenderMode(GL_SELECT);//�л�Ϊѡ��ģʽ
	glInitNames();
	glPushName(0);//��ʼ�����ֶ�ջ
	glMatrixMode(GL_PROJECTION);//�л���ͶӰ����
	glLoadIdentity();
	gluPickMatrix(x,y,w,h,m_port[aim]); 
	glMultMatrixd(m_projMatrix[aim]);//�ȳ����Ѵ������ͼ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(m_modelMatrix[m_activeView]); //�����ӽ�		
	/*******************************************/
	if(!pActiveCS) return 0;
	GLuint index=0;	
	if (pActiveCS->m_visible)//�ɼ��ǲŽ���ѡ�� 
	{
		//�����ƶ�����ת
		glPushMatrix();
		glTranslated(pActiveCS->m_translate[0],pActiveCS->m_translate[1],pActiveCS->m_translate[2]);//���ƽ��
		glMultMatrixd(pActiveCS->m_mvMatrix);//�ٳ����Ѵ������ת����		
		glScaled(pActiveCS->m_size,pActiveCS->m_size,pActiveCS->m_size);//�Ƚ��з���
		Triangle *p=pActiveCS->it;
		if (m_viewManner[m_activeView])//�Ȼ���������
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
			//���õ�Ĵ�С
			index++;
			glLoadName(index);//�滻����
			glBegin(GL_POINTS);
			glVertex3d((p->pt[0].v[0]+p->pt[1].v[0]+p->pt[2].v[0])/3,(p->pt[0].v[1]+p->pt[1].v[1]+p->pt[2].v[1])/3,(p->pt[0].v[2]+p->pt[1].v[2]+p->pt[2].v[2])/3);
			glEnd();
		}		
		glPopMatrix();
	}
	else return 0;
	glFinish();
	GLuint hits=glRenderMode(GL_RENDER);//�л�����Ⱦģʽ
	short ctrlState=GetKeyState(VK_CONTROL);//��ȡ��ʱcontrol���Ƿ���	
	if(ctrlState>=0)//û�а���control��
	{
		if (hits)
		{
			for(GLuint i=1;i<=hits;i++) m_hits[i]=m_selectBuff[4*i-1];
		}
		m_hits[0]=hits;		
	}
	else 
	{
		//ѡ�е���ӵ�����
		if (m_hits[0])//ԭ����ѡ��
		{
			GLuint cur=m_hits[0]+1;
			for (GLuint i=1;i<=hits;i++)
			{
				for (GLuint j=1;j<=m_hits[0];j++)
				{
					if (m_hits[j]==m_selectBuff[4*i-1]) break;//���صģ�����
				}
				if (j>m_hits[0])//û���ص�,����С����
				{
					m_hits[cur]=m_selectBuff[4*i-1];//�������
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
		else //ԭ��û��ѡ�У�ֱ�Ӽ���
		{
			for(GLuint i=1;i<=hits;i++) m_hits[i]=m_selectBuff[4*i-1];
			m_hits[0]=hits;
		}
	}
	return hits;
}

int CMMDlg::chooseOneObject()//ʹѡ�е�����ֻ��һ��
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

void CMMDlg::hideUnselected()//����δ��ѡ�е�
{
	CS* p=m_cs_head;
	while (p->next)
	{
		p=p->next;
		if (!p->m_selected)	p->m_visible=0;
	}
}

bool CMMDlg::pickKnife(GLint x,GLint y)//�ж��Ƿ���������
{
	GLuint hits;
	byte aim;
	if (m_num_view==4) aim=m_activeView;
	else aim=4;		
	glRenderMode(GL_SELECT);//�л�Ϊѡ��ģʽ
	glInitNames();
	glPushName(0);//��ʼ�����ֶ�ջ
	glMatrixMode(GL_PROJECTION);//�л���ͶӰ����
	glLoadIdentity();
	gluPickMatrix(x,y,2,2,m_port[aim]); 
	glMultMatrixd(m_projMatrix[aim]);//�ȳ����Ѵ������ͼ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(m_modelMatrix[m_activeView]); //�����ӽ�
	//���»���knife	
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
	hits=glRenderMode(GL_RENDER);//�л�����Ⱦģʽ
	if (hits==0) return FALSE;
	else return TRUE;
}

void CMMDlg::drawKnife()//�����и
{	
	glPushMatrix();
	glTranslated(m_knife.t[0]+m_knife.tx,m_knife.t[1]+m_knife.ty,m_knife.t[2]+m_knife.tz);//�ƶ�
	glRotated(m_knife.angle,m_knife.axis[0],m_knife.axis[1],m_knife.axis[2]);//��ʱת��
	glMultMatrixd(m_knife.rotateMatrix);//��ǰ��ת��
	glScaled(m_knife.size,m_knife.size,m_knife.size);//����
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

void CMMDlg::deleteTriangle(Triangle *q,CS *pCS,CSearch &s1,CSearch &s2)//ɾ�������κ���Ӧ�ڵ�
{
	Triangle *p=q->next;
	for (byte i=0;i<3;i++)
	{
		VertexRef *vr=p->pt[i].vr;
		TriangleRef *tr=p->pt[i].tr,*qtr;
		if (vr->nextRef->nextRef==NULL)//��ɾ��vr��tr
		{
			delete vr->nextRef;
			VertexRef *pvr=pCS->iv;
			while (pvr->next!=vr) pvr=pvr->next;
			pvr->next=vr->next;
			delete vr;
			s1.del(vr);//����ɾ��vr����
			s2.del(vr);//����ɾ��vr����
		}
		else//ֻɾ��tr
		{
			if (vr->nextRef==tr) //tr����vr
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
	//ɾ��������
	q->next=p->next;
	delete p;
}

void CMMDlg::cutCS()//��һ��CS�ָ�Ϊ����CS
{
	if (!pActiveCS)
	{
		MessageBox("��ѡ��һ�����壡");
	}
	else
	{
		//����knife����Ϣ
		GLdouble mvMatrix[16];
		GLdouble wx1,wy1,wz1,wx2,wy2,wz2,wx3,wy3,wz3,wx4,wy4,wz4;
		GLdouble vo[3],vn[3],vx[3],vy[3];//vo�����ĵ�����꣬vn�Ƿ���������һ���ǵ�λ������
		GLdouble dx=m_knife.dx*m_knife.size/pActiveCS->m_size;
		GLdouble dy=m_knife.dy*m_knife.size/pActiveCS->m_size;//��С
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixd(m_modelMatrix[0]);//�����ӽ�
		glPushMatrix();
		glTranslated(m_knife.t[0],m_knife.t[1],m_knife.t[2]);
		glMultMatrixd(m_knife.rotateMatrix);
		glScaled(m_knife.size,m_knife.size,m_knife.size);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);//ȡ��knife����ͼģ�;���
		gluProject(0,0,0,mvMatrix,m_projMatrix[0],m_port[0],&wx1,&wy1,&wz1);
		gluProject(0,0,1,mvMatrix,m_projMatrix[0],m_port[0],&wx2,&wy2,&wz2);
		gluProject(m_knife.dx,0,0,mvMatrix,m_projMatrix[0],m_port[0],&wx3,&wy3,&wz3);
		gluProject(0,m_knife.dy,0,mvMatrix,m_projMatrix[0],m_port[0],&wx4,&wy4,&wz4);
		glPopMatrix();
		glTranslated(pActiveCS->m_translate[0],pActiveCS->m_translate[1],pActiveCS->m_translate[2]);
		glMultMatrixd(pActiveCS->m_mvMatrix);
		glScaled(pActiveCS->m_size,pActiveCS->m_size,pActiveCS->m_size);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);//ȡ��activeCS����ͼģ�;���
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
		//��ʼ������
		Edge *ehead=new Edge;
		ehead->next=NULL;
		Edge *pe=ehead;
		//�½�����CS
		CS *pCS1=new CS;
		CS *pCS2=new CS;
		Triangle *pt1=pCS1->it;
		Triangle *pt2=pCS2->it;
		CSearch search1,search2;//������������
		//����pActiveCS��ָ�����������ж������κ�knife�ཻ���
		bool deleted=0;
		GLdouble r1,r2,r3,type=0;
		GLdouble v[3][3];//��������������
		Triangle *q=pActiveCS->it,*p;
		while(q->next)
		{
			p=q->next;
			deleted=0;
			r1=(p->pt[0].v[0]-vo[0])*vn[0]+(p->pt[0].v[1]-vo[1])*vn[1]+(p->pt[0].v[2]-vo[2])*vn[2];
			r2=(p->pt[1].v[0]-vo[0])*vn[0]+(p->pt[1].v[1]-vo[1])*vn[1]+(p->pt[1].v[2]-vo[2])*vn[2];
			r3=(p->pt[2].v[0]-vo[0])*vn[0]+(p->pt[2].v[1]-vo[1])*vn[1]+(p->pt[2].v[2]-vo[2])*vn[2];
			//�ཻ�������,����δ�жϽ����Ƿ�����knife��
			if (r1>0&&r2>0&&r3>0||r1<0&&r2<0&&r3<0) type=0;
			else if(r1*r2*r3!=0) //û�ж�����knife��
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
			else //�ж�����knife�ϣ����ʽ�С
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
				MessageBox("�и����\nԭ����������ȫ�����㶼�ڵ����ϣ�");
				return;
			}
			//���˷���������ֱ���д���
			GLdouble k;
			GLdouble ve[2][3];//�ߵ������˵�
			GLdouble kx,ky;
			if (type==1)
			{
				k=((v[1][0]-vo[0])*vn[0]+(v[1][1]-vo[1])*vn[1]+(v[1][2]-vo[2])*vn[2])/((v[1][0]-v[2][0])*vn[0]+(v[1][1]-v[2][1])*vn[1]+(v[1][2]-v[2][2])*vn[2]);
				ve[0][0]=(1-k)*v[1][0]+k*v[2][0]-vo[0];	
				ve[0][1]=(1-k)*v[1][1]+k*v[2][1]-vo[1];
				ve[0][2]=(1-k)*v[1][2]+k*v[2][2]-vo[2];//OD	
				kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
				ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)//�ڵ����ϲŽ�����һ��
				{
					k=((v[0][0]-vo[0])*vn[0]+(v[0][1]-vo[1])*vn[1]+(v[0][2]-vo[2])*vn[2])/((v[0][0]-v[2][0])*vn[0]+(v[0][1]-v[2][1])*vn[1]+(v[0][2]-v[2][2])*vn[2]);
					ve[1][0]=(1-k)*v[0][0]+k*v[2][0]-vo[0];	
					ve[1][1]=(1-k)*v[0][1]+k*v[2][1]-vo[1];
					ve[1][2]=(1-k)*v[0][2]+k*v[2][2]-vo[2];//OE
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //���㶼�ڵ�����,ѹ��ߣ�ȥ����д��������
					{
						ve[0][0]+=vo[0];
						ve[0][1]+=vo[1];
						ve[0][2]+=vo[2];
						ve[1][0]+=vo[0];
						ve[1][1]+=vo[1];
						ve[1][2]+=vo[2];
						//д��DE��
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=ve[0][0];
						pe->y1=ve[0][1];
						pe->z1=ve[0][2];
						pe->x2=ve[1][0];
						pe->y2=ve[1][1];
						pe->z2=ve[1][2];
						//ѹ����������
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
						//ɾ��p=q->next��ָ��������μ���������
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("�и����\nԭ�������β���ȫ�и");
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
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)//�ڵ����ϲŽ�����һ��
				{
					k=((v[0][0]-vo[0])*vn[0]+(v[0][1]-vo[1])*vn[1]+(v[0][2]-vo[2])*vn[2])/((v[0][0]-v[2][0])*vn[0]+(v[0][1]-v[2][1])*vn[1]+(v[0][2]-v[2][2])*vn[2]);
					ve[1][0]=(1-k)*v[0][0]+k*v[2][0]-vo[0];	
					ve[1][1]=(1-k)*v[0][1]+k*v[2][1]-vo[1];
					ve[1][2]=(1-k)*v[0][2]+k*v[2][2]-vo[2];//OE
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //���㶼�ڵ�����,ѹ��ߣ�ȥ����д��������
					{
						ve[0][0]+=vo[0];
						ve[0][1]+=vo[1];
						ve[0][2]+=vo[2];
						ve[1][0]+=vo[0];
						ve[1][1]+=vo[1];
						ve[1][2]+=vo[2];
						//д��DE��
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=ve[0][0];
						pe->y1=ve[0][1];
						pe->z1=ve[0][2];
						pe->x2=ve[1][0];
						pe->y2=ve[1][1];
						pe->z2=ve[1][2];
						//ѹ����������
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
						//ɾ��p=q->next��ָ��������μ���������
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("�и����\nԭ�������β���ȫ�и");
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
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)//�ڵ����ϲŽ�����һ��
				{
					k=((v[0][0]-vo[0])*vn[0]+(v[0][1]-vo[1])*vn[1]+(v[0][2]-vo[2])*vn[2])/((v[0][0]-v[2][0])*vn[0]+(v[0][1]-v[2][1])*vn[1]+(v[0][2]-v[2][2])*vn[2]);
					ve[1][0]=(1-k)*v[0][0]+k*v[2][0]-vo[0];	
					ve[1][1]=(1-k)*v[0][1]+k*v[2][1]-vo[1];
					ve[1][2]=(1-k)*v[0][2]+k*v[2][2]-vo[2];//OE
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //���㶼�ڵ�����,ѹ��ߣ�ȥ����д��������
					{
						ve[0][0]+=vo[0];
						ve[0][1]+=vo[1];
						ve[0][2]+=vo[2];
						ve[1][0]+=vo[0];
						ve[1][1]+=vo[1];
						ve[1][2]+=vo[2];
						//д��DE��
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=ve[0][0];
						pe->y1=ve[0][1];
						pe->z1=ve[0][2];
						pe->x2=ve[1][0];
						pe->y2=ve[1][1];
						pe->z2=ve[1][2];
						//ѹ����������
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
						//ɾ��p=q->next��ָ��������μ���������
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("�и����\nԭ�������β���ȫ�и");
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
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)//�ڵ����ϲŽ�����һ��
				{
					k=((v[0][0]-vo[0])*vn[0]+(v[0][1]-vo[1])*vn[1]+(v[0][2]-vo[2])*vn[2])/((v[0][0]-v[1][0])*vn[0]+(v[0][1]-v[1][1])*vn[1]+(v[0][2]-v[1][2])*vn[2]);
					ve[0][0]=(1-k)*v[0][0]+k*v[1][0]-vo[0];	
					ve[0][1]=(1-k)*v[0][1]+k*v[1][1]-vo[1];
					ve[0][2]=(1-k)*v[0][2]+k*v[1][2]-vo[2];//OE
					kx=(vx[0]*ve[0][0]+vx[1]*ve[0][1]+vx[2]*ve[0][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[0][0]+vy[1]*ve[0][1]+vy[2]*ve[0][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //���㶼�ڵ�����,ѹ��ߣ�ȥ����д��������
					{
						ve[0][0]+=vo[0];
						ve[0][1]+=vo[1];
						ve[0][2]+=vo[2];
						ve[1][0]+=vo[0];
						ve[1][1]+=vo[1];
						ve[1][2]+=vo[2];
						//д��DE��
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=ve[0][0];
						pe->y1=ve[0][1];
						pe->z1=ve[0][2];
						pe->x2=ve[1][0];
						pe->y2=ve[1][1];
						pe->z2=ve[1][2];
						//ѹ����������
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
						//ɾ��p=q->next��ָ��������μ���������
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("�и����\nԭ�������β���ȫ�и");
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
					//ɾ��p=q->next��ָ��������μ���������
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
					//ɾ��p=q->next��ָ��������μ���������
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
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //�ڵ����ϲŽ�����һ��
				{
					ve[1][0]=v[2][0]-vo[0];
					ve[1][1]=v[2][1]-vo[1];
					ve[1][2]=v[2][2]-vo[2];
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)
					{
						//д��DE��
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=v[1][0];
						pe->y1=v[1][1];
						pe->z1=v[1][2];
						pe->x2=v[2][0];
						pe->y2=v[2][1];
						pe->z2=v[2][2];
						//д����������
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
						//ɾ��p=q->next��ָ��������μ���������
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("�и����\nԭ�������β���ȫ�и");
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
				if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1) //�ڵ����ϲŽ�����һ��
				{
					ve[1][0]=v[2][0]-vo[0];
					ve[1][1]=v[2][1]-vo[1];
					ve[1][2]=v[2][2]-vo[2];
					kx=(vx[0]*ve[1][0]+vx[1]*ve[1][1]+vx[2]*ve[1][2])/(vx[0]*vx[0]+vx[1]*vx[1]+vx[2]*vx[2]);
					ky=(vy[0]*ve[1][0]+vy[1]*ve[1][1]+vy[2]*ve[1][2])/(vy[0]*vy[0]+vy[1]*vy[1]+vy[2]*vy[2]);
					if (-1<=kx&&kx<=1&&-1<=ky&&ky<=1)
					{
						//д��DE��
						pe->next=new Edge;
						pe=pe->next;
						pe->next=NULL;
						pe->x1=v[0][0];
						pe->y1=v[0][1];
						pe->z1=v[0][2];
						pe->x2=v[2][0];
						pe->y2=v[2][1];
						pe->z2=v[2][2];
						//д����������
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
						//ɾ��p=q->next��ָ��������μ���������
						deleteTriangle(q,pActiveCS,search1,search2);
						deleted=1;
					}
					else
					{
						MessageBox("�и����\nԭ�������β���ȫ�и");
						return;
					}
				}
			}
			//δɾ��p,��q����ƶ�
			if(!deleted) q=q->next;
		}
		//��pActiveCS���µ������εķ��ʱ����0
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
		//��ʼ�ڵ�����pActiveCS���µ������Σ���ΪpCS1��pCS2
		TriangleRef *tr=NULL;
		CQueue queue;//��ʼ����
		if (search1.shead->next)//�п��������Ž�������
		{
			queue.enQueue(search1.shead->next->pv);
			search1.shead->next->pv->visit=1;//��һ���ڵ㱻������
			while (queue.num) //���в���ʱ
			{
				vr=queue.deQueue();
				tr=vr->nextRef;
				while(tr)
				{
					if(!tr->t->visit)//δ�����ʣ����临�Ƶ�pCS1��ָ����������
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
						//�������ε���������ŵ�������
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
		
		if (search2.shead->next)//�п��������Ž�������
		{
			queue.enQueue(search2.shead->next->pv);
			search2.shead->next->pv->visit=1;//��һ���ڵ㱻������
			while (queue.num) //���в���ʱ
			{
				vr=queue.deQueue();
				tr=vr->nextRef;
				while(tr)
				{
					if(!tr->t->visit)//δ�����ʣ����临�Ƶ�pCS1��ָ����������
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
						//�������ε���������ŵ�������
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
		//����ߣ�ʹ�����ӳɻ�
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
					//�������
					te->x1=pe->x2;
					te->y1=pe->y2;
					te->z1=pe->z2;
					qe->next=te->next;
					just=1;
					break;
				}
				else qe=qe->next;
			}
			if(just==0) MessageBox("����û���ҵ���ȷ�ıߣ�");
			te->next=pe->next;
			pe->next=te;
			pe=pe->next;
		}
		if(!(fabs(ehead->next->x1-pe->x2)<DELTA&&fabs(ehead->next->y1-pe->y2)<DELTA&&fabs(ehead->next->z1-pe->z2)<DELTA)) MessageBox("�����и���ı߲��ܷ��");
		pe->x2=ehead->next->x1;
		pe->y2=ehead->next->y1;
		pe->z2=ehead->next->z1;
		//�Դ˻������ʷ�,�����ʷֺ�������νӵ�pcs
		Triangle *t1=m_p2t.polygonToTriangles(ehead,TRUE,vn);
		Triangle *t2=m_p2t.copyAnotherSide(t1);
		pt1->next=t1->next;
		delete t1;
		pt2->next=t2->next;
		delete t2;
		//pCS1��pCS2�踴��pActiveCS�е���ת�����ƶ�
		pCS1->m_size=pCS2->m_size=pActiveCS->m_size;
		pCS1->m_translate[0]=pCS2->m_translate[0]=pActiveCS->m_translate[0];
		pCS1->m_translate[1]=pCS2->m_translate[1]=pActiveCS->m_translate[1];
		pCS1->m_translate[2]=pCS2->m_translate[2]=pActiveCS->m_translate[2];
		for (int i=0;i<16;i++) 
		{
			pCS1->m_mvMatrix[i]=pCS2->m_mvMatrix[i]=pActiveCS->m_mvMatrix[i];
		}
		//ɾ��pActiveCS,����pCS1��pCS2,workmode=1
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
		pCS1->m_name="�ʷ���1";
		pCS1->m_visible=1;
		pCS1->m_selected=1;
		m_maxIndex++;
		pCS1->m_index=m_maxIndex;
		pCS1->setColor(getRandomColor());	
		m_num_model++;	
		pCS2->m_name="�ʷ���2";
		pCS2->m_visible=1;
		pCS2->m_selected=1;
		m_maxIndex++;
		pCS2->m_index=m_maxIndex;
		pCS->setColor(getRandomColor());	
		m_num_model++;	
		pCS1->createVertexRef();//��Triangle����ȡVertexRef��
		pCS2->createVertexRef();//��Triangle����ȡVertexRef��
		pCS1->next=pCS2;
		pCS2->next=m_cs_head->next;
		m_cs_head->next=pCS1;
		m_workMode=2;//�ƶ�ģʽ
		renderScene();
	}
}

bool CMMDlg::saveFile(byte type)//���̶Ի�
{
	CString filter,defEx;
	int structsize=0; //��ʾ������
	DWORD dwVersion;
	dwVersion = GetVersion();// ���Ŀǰ�Ĳ���ϵͳ 
	if (dwVersion < 0x80000000)	structsize =88; //����ϵͳ��Windows NT/2000
	else structsize =76; //Windows 95/98ϵͳ	
	if (type==0)//����ʾtxt��mm�ļ�ѡ��
	{
		filter="ModelMaker�ļ�(*.mm)|*.mm|ʵ�������ļ�(*.txt)|*.txt||";
		defEx="mm";
	}
	else if (type==1)//mm�ļ�
	{
		filter="ModelMaker�ļ�(*.mm)|*.mm||";
		defEx="mm";
	}
	else if (type==2)//smm�ļ�
	{
		filter="����ModelMakerģ���ļ�(*.smm)|*.smm||";
		defEx="smm";
	}
	else if (type==3)//txt�ļ�
	{
		filter="ʵ����Ҫ���ļ���ʽ(*.txt)|*.txt||";
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
	if(saveFile.DoModal()!=IDOK) return FALSE;//��������
	FILE *fp=fopen(saveFile.GetPathName(),"w");	
	if (!fp)
	{
		AfxMessageBox("�ļ�д��ʧ�ܣ�");
		return FALSE;
	}
	CS *pCS=m_cs_head;
	Triangle *pt;
	if (saveFile.GetFileExt()=="mm") 
	{
		int i,j;
		fprintf(fp,"This is a ModelMaker file\n");
		fprintf(fp,"%d\n",m_num_model);//ģ�͵ĸ���
		
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
			//����������������
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
		pCS=pActiveCS;//����ģ��
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
		//����������������
		pt=pCS->it;
		while (pt->next)
		{
			pt=pt->next;
			fprintf(fp,"%lf\t%lf\t%lf\n",pt->pt[0].v[0],pt->pt[0].v[1],pt->pt[0].v[2]);
			fprintf(fp,"%lf\t%lf\t%lf\n",pt->pt[1].v[0],pt->pt[1].v[1],pt->pt[1].v[2]);
			fprintf(fp,"%lf\t%lf\t%lf\n\n",pt->pt[2].v[0],pt->pt[2].v[1],pt->pt[2].v[2]);
		}
	}
	else if (saveFile.GetFileExt()=="txt")//ʵ���������ļ�
	{
		double m[16];
		double x,y,z,dz;
		while(pCS->next)
		{
			pCS=pCS->next;
			//��ȡģ�ͱ任����
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glTranslated(pCS->m_translate[0],pCS->m_translate[1],pCS->m_translate[2]);
			glMultMatrixd(pCS->m_mvMatrix);
			glScaled(pCS->m_size,pCS->m_size,pCS->m_size);
			glGetDoublev(GL_MODELVIEW_MATRIX, m);
			glPopMatrix();
			dz=1e-5-m_outputDlg.m_size*pCS->rminZ;//�����嶼�ƶ���ƽ����
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
				fprintf(fp,"%.8lf\n%.8lf\n%.8lf\n",x,y,z);//8λС��
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
		MessageBox(" �ļ���չ������ȷ��");
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
	else MessageBox("��������ѡ����ȷ��");
}

void CMMDlg::densifySelected()//����ѡ����
{
	if (pActiveCS)
	{
		pActiveCS->densify((bool)FALSE);
		renderScene();
	}
	else MessageBox("��������ѡ����ȷ��");
}

void CMMDlg::OnOpen() //���ĵ���mm/smm��
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

void CMMDlg::OnSave()//��������
{
	if(m_filePath!=""&&m_num_model)
	{
		FILE *fp=fopen(m_filePath,"w");
		CS *pCS=m_cs_head;
		Triangle *pt;
		int i,j;
		fprintf(fp,"This is a ModelMaker file\n");
		fprintf(fp,"%d\n",m_num_model);//ģ�͵ĸ���
		
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
			//����������������
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
		MessageBox("�ļ�����ɹ���");
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

int CMMDlg::getCSNum()//��ȡģ�͸���
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

void CMMDlg::OnOutput() //���Ϊʵ���������ļ�
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
		m_outputDlg.m_mLenght=maxX-minX;//Ĭ�Ϸ�Χ�ǳ���100nm
		m_outputDlg.m_mWidth=maxY-minY;
		m_outputDlg.m_mHeight=maxH;
		m_outputDlg.m_mix=minX;
		m_outputDlg.m_miy=minY;
		if (m_outputDlg.DoModal()==IDOK)//�������
		{
			saveFile(3);//���Ϊʵ���������ļ�
		}
	}	
}

// MMDlg.h : header file
//

#if !defined(AFX_MMDLG_H__589EFF52_DA66_4E48_8419_3E5C6275516E__INCLUDED_)
#define AFX_MMDLG_H__589EFF52_DA66_4E48_8419_3E5C6275516E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gl\glut.h"
#include "gl\glext.h"
#include "math.h"
#include "PanelCreate.h"
#include "PanelModify.h"
#include "ClosedSurface.h"
#include "StatePanel.h"
#include "Knife.h"
#include "Triangulation.h"
#include "Search.h"
#include "OutputDlg.h"

#define BUFFER_LENGTH 4096 //一次可以选中1024个图元
#define MAX_COPY_INDEX 128 //一次可复制的模型个数
typedef struct
{
	int left,right,top,bottom;
}Rect; 

typedef struct 
{
	WPARAM theta_plus,theta_minus,phi_plus,phi_minus,
			R_plus,R_minus,del_object;
}HotKeys; //快捷键集合


/////////////////////////////////////////////////////////////////////////////
// CMMDlg dialog

class CMMDlg : public CDialog
{
// Construction
public:
	static double random(bool func);
	CMMDlg(CWnd* pParent = NULL);	// standard constructor
	void renderScene();//渲染场景
	void changeViewStyle();//改变视图风格
	bool Openfile(byte type);//获取要打开文件的路径
	void changeColor();//改变物体颜色
	void updateName();//更新对象的颜色
	void createCuboid();//创建长方体
	void createCone();//创建椎体
	void createSphere();//创建球体
	void createCylinder();//创建椎体
	void createTorus();//创建环形
	void createPipe();//创建管状体
	bool isHotKey(WPARAM VirKey);//判断是否属于快捷键
	COLORREF getRandomColor();//挑选初始化颜色
	int chooseOneObject();//使选中的物体只有一个
	void hideUnselected();//隐藏未被选中的
	void drawKnife();//绘制切割刀
	void cutCS();//将一个CS分割为两个CS
	void densify(double increase);
	void changeWorkMode(double mode);
	void densifySelected();//加密选中者

	Triangulation m_p2t;//polygon to triangles的缩写
	CColorDialog m_pallete;//颜色选择面板
	COLORREF m_activeColor,m_randomColor;//活动颜色和当前系统随机色
	Cuboid m_cuboid;//储存要创建的长方体
	Cylinder m_cylinder;//储存要创建的柱体
	Sphere m_sphere;//储存要创建的球体
	Torus m_torus;//储存要创建的轮胎体
	Pipe m_pipe;//储存要创建的管状体
	double m_create;//不同的创建状态
	double m_drag;//拖动的不同状态
	double m_chooseDensify;//选择性加密的不同状态
	HotKeys m_hotKeys;//快捷键
	bool m_cursorBox;//是否需要绘制鼠标框
	Knife m_knife;//切割平面	
	short m_num_view;//视图的个数
	short m_activeView;//指示哪个视图窗口是选中活动的
	GLdouble m_modelMatrix[4][16],m_projMatrix[5][16];//当前视图模型矩阵，投影矩阵
	GLint m_port[5][4];//当前视口矩阵
	GLuint m_hits[512];//返回点击物体由近到远的名称
	///////////////////////////////////
	CPanelCreate m_page1;//创建面板
	CPanelModify m_page2;//修改面板
	CStatePanel m_statePanel;//状态面板

// Dialog Data
	//{{AFX_DATA(CMMDlg)
	enum { IDD = IDD_MM_DIALOG };
	CTabCtrl	m_tab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMMDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
		
	HICON m_hIcon;
	CDC *m_pdc;//绘图pdc
	CRect rcALL,m_old_rect;//绘图区位置
	Rect rc[4];//四绘图区
	int m_theta,m_phi,m_R;//视角变量
	//////////////////////////////////
	CDialog *pDlg[2]; //选项卡指针
	int m_cur; //指示哪个标签被选中
	bool m_projectManner;//定义投影方式？
	short m_viewport;//定义哪个viewport投影到中央位置
	int m_id;//openGL文字显示列表起始id	
	short m_view_style;//显示方式
	float m_numOfLattice;//栅格个数
	float m_size[4];//视图显示的缩放比例	
	bool m_select;//指示是否选中点
	int m_num_model;//已载入模型的个数
	VertexRef *m_vr;//记录选中的点的指针
	bool m_viewManner[4];//指示每个视图区显示的方式。0代表透视，1代表有面片遮盖反应深度
	CS *m_cs_head;//曲面链表头指针
	CS *pActiveCS;//当前编辑的曲面,因为一次只可以编辑一个曲面
	CString m_credit;//作者信息
	CString m_directory;//程序路径
	bool m_viewSide[4];//从哪边观看，六个面都可切换
	byte m_colors[64][3];//基本的64种颜色
	unsigned int m_In;//随机数变量
	unsigned int m_maxIndex;//名称编号的最大值，用于后续编号
	byte m_move;//标记是否进行物体，于onMouseMove相配合
	bool m_updateProjMatrix;//标识是否更新投影矩阵，size时将其置为1
	GLint m_pp[3][2];//记录平面上的3个点的坐标,是左键按下，第二个是左键弹起，第三个是移动时坐标
	GLint m_rpp[2][2];//记录右键点击的两个点的平面坐标,0储存后来要用的点，1储存当前右键坐标
	GLint m_lastMovePP[2];
	GLdouble m_vertex[4][3];//记录3维空间中的4个点
	bool m_drawChooseBox;//是否绘制选择框
	bool m_drawRotateCross;//是否绘制旋转十字架
	GLuint m_selectBuff[BUFFER_LENGTH];//选择缓冲区
	VertexRef *m_pointsBuffer[1024];//返回一次选中点的指针
	VertexRef *m_pointHits[1024];//储存所有选中点的指针
	GLuint m_numPointHits;//所有选中点的个数
	GLuint m_numBuffHits;//记录一次选中点的个数
	bool m_LBdown;//所需要的左键点击按下
	bool m_BoxChoose;//是否开启框选	
	CS * m_copyPointer[MAX_COPY_INDEX];//要复制的模型的指针缓冲区
	GLuint m_numToCopy;
	double m_workMode;//工作模式
	COutputDlg m_outputDlg;//输出对话框
	CString m_filePath;//当前打开的文件路径
	
	void draw3DLattice();	
	void InitOpenGL();
	bool bSetupPixelFormat(CDC *m_pdc_view);
	void DrawModel();
	void lookAt(short i);
	void drawPlaneLattice();
	void toGLCoordinate(CPoint *point);//将屏幕坐标转化为绘图区openGL坐标
	void toGLRect(CRect *rc);//将屏幕rect转化为openGL绘图区rect
	void changeViewportStyle(short m);
	void GLTextOut(float x,float y,float z,char * const textstring);
	void calculateRc();//计算显示区域的位置
	bool inRect(Rect &rc,CPoint &p);//判断是否在区域中
	int howInRect(Rect &rc,CPoint &p);//判断点在区域的哪个位置
	void exchangeRect(Rect &a,Rect &b);//交换视图区
	double random();//随机数函数
	GLint pickObjects(GLint x1,GLint y1,GLint x2,GLint y2);//在指定的区域内选择物体
	GLint pickPoints(GLint x,GLint y,GLint w,GLint h);//选点
	GLint pickTriangles(GLint x,GLint y,GLint w,GLint h);//挑选三角形
	bool pickKnife(GLint x,GLint y);//判断是否点击到刀面
	bool inDrawArea(CPoint &p);//判断是否点击在绘图区
	void checkSelected();//检查选择情况，更新名字和颜色
	void drawNewObject();//创建过程绘制物体
	void unSelectALL();//使所有物体全不选中
	void deleteObject();
	void deleteTriangle(Triangle *q,CS *pCS,CSearch &s1,CSearch &s2);//删除三角形和相应节点
	bool saveFile(byte type);//存盘
	CString readLine(FILE *fp);
	int getCSNum();//获取模型个数
	
	// Generated message map functions
	//{{AFX_MSG(CMMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRedo();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSelectMove();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNewfile();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSelectSize();
	afx_msg void OnSelect();
	afx_msg void OnRotate();
	afx_msg void OnUndo();
	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void OnOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MMDLG_H__589EFF52_DA66_4E48_8419_3E5C6275516E__INCLUDED_)

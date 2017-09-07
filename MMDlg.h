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

#define BUFFER_LENGTH 4096 //һ�ο���ѡ��1024��ͼԪ
#define MAX_COPY_INDEX 128 //һ�οɸ��Ƶ�ģ�͸���
typedef struct
{
	int left,right,top,bottom;
}Rect; 

typedef struct 
{
	WPARAM theta_plus,theta_minus,phi_plus,phi_minus,
			R_plus,R_minus,del_object;
}HotKeys; //��ݼ�����


/////////////////////////////////////////////////////////////////////////////
// CMMDlg dialog

class CMMDlg : public CDialog
{
// Construction
public:
	static double random(bool func);
	CMMDlg(CWnd* pParent = NULL);	// standard constructor
	void renderScene();//��Ⱦ����
	void changeViewStyle();//�ı���ͼ���
	bool Openfile(byte type);//��ȡҪ���ļ���·��
	void changeColor();//�ı�������ɫ
	void updateName();//���¶������ɫ
	void createCuboid();//����������
	void createCone();//����׵��
	void createSphere();//��������
	void createCylinder();//����׵��
	void createTorus();//��������
	void createPipe();//������״��
	bool isHotKey(WPARAM VirKey);//�ж��Ƿ����ڿ�ݼ�
	COLORREF getRandomColor();//��ѡ��ʼ����ɫ
	int chooseOneObject();//ʹѡ�е�����ֻ��һ��
	void hideUnselected();//����δ��ѡ�е�
	void drawKnife();//�����и
	void cutCS();//��һ��CS�ָ�Ϊ����CS
	void densify(double increase);
	void changeWorkMode(double mode);
	void densifySelected();//����ѡ����

	Triangulation m_p2t;//polygon to triangles����д
	CColorDialog m_pallete;//��ɫѡ�����
	COLORREF m_activeColor,m_randomColor;//���ɫ�͵�ǰϵͳ���ɫ
	Cuboid m_cuboid;//����Ҫ�����ĳ�����
	Cylinder m_cylinder;//����Ҫ����������
	Sphere m_sphere;//����Ҫ����������
	Torus m_torus;//����Ҫ��������̥��
	Pipe m_pipe;//����Ҫ�����Ĺ�״��
	double m_create;//��ͬ�Ĵ���״̬
	double m_drag;//�϶��Ĳ�ͬ״̬
	double m_chooseDensify;//ѡ���Լ��ܵĲ�ͬ״̬
	HotKeys m_hotKeys;//��ݼ�
	bool m_cursorBox;//�Ƿ���Ҫ��������
	Knife m_knife;//�и�ƽ��	
	short m_num_view;//��ͼ�ĸ���
	short m_activeView;//ָʾ�ĸ���ͼ������ѡ�л��
	GLdouble m_modelMatrix[4][16],m_projMatrix[5][16];//��ǰ��ͼģ�;���ͶӰ����
	GLint m_port[5][4];//��ǰ�ӿھ���
	GLuint m_hits[512];//���ص�������ɽ���Զ������
	///////////////////////////////////
	CPanelCreate m_page1;//�������
	CPanelModify m_page2;//�޸����
	CStatePanel m_statePanel;//״̬���

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
	CDC *m_pdc;//��ͼpdc
	CRect rcALL,m_old_rect;//��ͼ��λ��
	Rect rc[4];//�Ļ�ͼ��
	int m_theta,m_phi,m_R;//�ӽǱ���
	//////////////////////////////////
	CDialog *pDlg[2]; //ѡ�ָ��
	int m_cur; //ָʾ�ĸ���ǩ��ѡ��
	bool m_projectManner;//����ͶӰ��ʽ��
	short m_viewport;//�����ĸ�viewportͶӰ������λ��
	int m_id;//openGL������ʾ�б���ʼid	
	short m_view_style;//��ʾ��ʽ
	float m_numOfLattice;//դ�����
	float m_size[4];//��ͼ��ʾ�����ű���	
	bool m_select;//ָʾ�Ƿ�ѡ�е�
	int m_num_model;//������ģ�͵ĸ���
	VertexRef *m_vr;//��¼ѡ�еĵ��ָ��
	bool m_viewManner[4];//ָʾÿ����ͼ����ʾ�ķ�ʽ��0����͸�ӣ�1��������Ƭ�ڸǷ�Ӧ���
	CS *m_cs_head;//��������ͷָ��
	CS *pActiveCS;//��ǰ�༭������,��Ϊһ��ֻ���Ա༭һ������
	CString m_credit;//������Ϣ
	CString m_directory;//����·��
	bool m_viewSide[4];//���ı߹ۿ��������涼���л�
	byte m_colors[64][3];//������64����ɫ
	unsigned int m_In;//���������
	unsigned int m_maxIndex;//���Ʊ�ŵ����ֵ�����ں������
	byte m_move;//����Ƿ�������壬��onMouseMove�����
	bool m_updateProjMatrix;//��ʶ�Ƿ����ͶӰ����sizeʱ������Ϊ1
	GLint m_pp[3][2];//��¼ƽ���ϵ�3���������,��������£��ڶ�����������𣬵��������ƶ�ʱ����
	GLint m_rpp[2][2];//��¼�Ҽ�������������ƽ������,0�������Ҫ�õĵ㣬1���浱ǰ�Ҽ�����
	GLint m_lastMovePP[2];
	GLdouble m_vertex[4][3];//��¼3ά�ռ��е�4����
	bool m_drawChooseBox;//�Ƿ����ѡ���
	bool m_drawRotateCross;//�Ƿ������תʮ�ּ�
	GLuint m_selectBuff[BUFFER_LENGTH];//ѡ�񻺳���
	VertexRef *m_pointsBuffer[1024];//����һ��ѡ�е��ָ��
	VertexRef *m_pointHits[1024];//��������ѡ�е��ָ��
	GLuint m_numPointHits;//����ѡ�е�ĸ���
	GLuint m_numBuffHits;//��¼һ��ѡ�е�ĸ���
	bool m_LBdown;//����Ҫ������������
	bool m_BoxChoose;//�Ƿ�����ѡ	
	CS * m_copyPointer[MAX_COPY_INDEX];//Ҫ���Ƶ�ģ�͵�ָ�뻺����
	GLuint m_numToCopy;
	double m_workMode;//����ģʽ
	COutputDlg m_outputDlg;//����Ի���
	CString m_filePath;//��ǰ�򿪵��ļ�·��
	
	void draw3DLattice();	
	void InitOpenGL();
	bool bSetupPixelFormat(CDC *m_pdc_view);
	void DrawModel();
	void lookAt(short i);
	void drawPlaneLattice();
	void toGLCoordinate(CPoint *point);//����Ļ����ת��Ϊ��ͼ��openGL����
	void toGLRect(CRect *rc);//����Ļrectת��ΪopenGL��ͼ��rect
	void changeViewportStyle(short m);
	void GLTextOut(float x,float y,float z,char * const textstring);
	void calculateRc();//������ʾ�����λ��
	bool inRect(Rect &rc,CPoint &p);//�ж��Ƿ���������
	int howInRect(Rect &rc,CPoint &p);//�жϵ���������ĸ�λ��
	void exchangeRect(Rect &a,Rect &b);//������ͼ��
	double random();//���������
	GLint pickObjects(GLint x1,GLint y1,GLint x2,GLint y2);//��ָ����������ѡ������
	GLint pickPoints(GLint x,GLint y,GLint w,GLint h);//ѡ��
	GLint pickTriangles(GLint x,GLint y,GLint w,GLint h);//��ѡ������
	bool pickKnife(GLint x,GLint y);//�ж��Ƿ���������
	bool inDrawArea(CPoint &p);//�ж��Ƿ����ڻ�ͼ��
	void checkSelected();//���ѡ��������������ֺ���ɫ
	void drawNewObject();//�������̻�������
	void unSelectALL();//ʹ��������ȫ��ѡ��
	void deleteObject();
	void deleteTriangle(Triangle *q,CS *pCS,CSearch &s1,CSearch &s2);//ɾ�������κ���Ӧ�ڵ�
	bool saveFile(byte type);//����
	CString readLine(FILE *fp);
	int getCSNum();//��ȡģ�͸���
	
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

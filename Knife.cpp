// Knife.cpp: implementation of the Knife class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MM.h"
#include "Knife.h"
#include "MMDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMMDlg * g_p;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Knife::Knife()
{
	angle=0;
	axis[0]=1;
	axis[1]=axis[2]=0;
	
	size=1;
	t[0]=t[1]=0;
	t[2]=0.2; //初始时刀面上移0.2
	tx=ty=tz=0;
	mouseWheelSize=0;//默认不通过鼠标缩放
	state=1;//默认进行移动
	dx=dy=1;
}

Knife::~Knife()
{

}

void Knife::init() //数据初始化
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX,rotateMatrix);
	glPopMatrix();
}

void Knife::storeMatrix()//储存移动和旋转
{
	glPushMatrix();
	glLoadIdentity();
	glRotated(angle,axis[0],axis[1],axis[2]);//再乘上当前的旋转矩阵
	glMultMatrixd(rotateMatrix);//先乘上已储存的视图矩阵	
	glGetDoublev(GL_MODELVIEW_MATRIX,rotateMatrix);//获取相乘后的旋转矩阵
	glPopMatrix();
	t[0]+=tx;
	t[1]+=ty;
	t[2]+=tz;//积累所有平移
	tx=ty=tz=0;
	angle=0;
}

void Knife::getRealWorldPoints(CS *p)//由给定的物体获取其空间的坐标
{
	GLdouble modelMatrix[16];
	GLdouble wx1,wy1,wz1,wx2,wy2,wz2;//平面坐标和深度坐标
	byte aim;
	if (g_p->m_num_view==4) aim=g_p->m_activeView;
	else aim=4;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(g_p->m_modelMatrix[aim]);//最后加载视角
	glPushMatrix();
	glTranslated(t[0],t[1],t[2]);//然后平移
	glMultMatrixd(rotateMatrix);//再旋转
	glScaled(size,size,size);//先放缩
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);//获取刀面的视图模型矩阵
	gluProject(0,0,0,modelMatrix,g_p->m_projMatrix[aim],g_p->m_port[g_p->m_activeView],&wx1,&wy1,&wz1);//中心点
	gluProject(0,0,1,modelMatrix,g_p->m_projMatrix[aim],g_p->m_port[g_p->m_activeView],&wx2,&wy2,&wz2);//法向量顶点
	glPopMatrix();//弹出刀面的模型矩阵
	glTranslated(p->m_translate[0],p->m_translate[1],p->m_translate[2]);//然后移动
	glMultMatrixd(p->m_mvMatrix);//再旋转
	glScaled(p->m_size,p->m_size,p->m_size);//先放缩
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);//获取物体的视图模型矩阵
	gluUnProject(wx1,wy1,wz1,modelMatrix,g_p->m_projMatrix[aim],g_p->m_port[g_p->m_activeView],&cx,&cy,&cz);
	gluUnProject(wx2,wy2,wz2,modelMatrix,g_p->m_projMatrix[aim],g_p->m_port[g_p->m_activeView],&nx,&ny,&nz);
	glPopMatrix();
	nx=nx-cx;
	ny=ny-cy;
	nz=nz-cz;//计算法向量,可能不是单位长度
}
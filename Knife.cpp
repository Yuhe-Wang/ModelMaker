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
	t[2]=0.2; //��ʼʱ��������0.2
	tx=ty=tz=0;
	mouseWheelSize=0;//Ĭ�ϲ�ͨ���������
	state=1;//Ĭ�Ͻ����ƶ�
	dx=dy=1;
}

Knife::~Knife()
{

}

void Knife::init() //���ݳ�ʼ��
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX,rotateMatrix);
	glPopMatrix();
}

void Knife::storeMatrix()//�����ƶ�����ת
{
	glPushMatrix();
	glLoadIdentity();
	glRotated(angle,axis[0],axis[1],axis[2]);//�ٳ��ϵ�ǰ����ת����
	glMultMatrixd(rotateMatrix);//�ȳ����Ѵ������ͼ����	
	glGetDoublev(GL_MODELVIEW_MATRIX,rotateMatrix);//��ȡ��˺����ת����
	glPopMatrix();
	t[0]+=tx;
	t[1]+=ty;
	t[2]+=tz;//��������ƽ��
	tx=ty=tz=0;
	angle=0;
}

void Knife::getRealWorldPoints(CS *p)//�ɸ����������ȡ��ռ������
{
	GLdouble modelMatrix[16];
	GLdouble wx1,wy1,wz1,wx2,wy2,wz2;//ƽ��������������
	byte aim;
	if (g_p->m_num_view==4) aim=g_p->m_activeView;
	else aim=4;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(g_p->m_modelMatrix[aim]);//�������ӽ�
	glPushMatrix();
	glTranslated(t[0],t[1],t[2]);//Ȼ��ƽ��
	glMultMatrixd(rotateMatrix);//����ת
	glScaled(size,size,size);//�ȷ���
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);//��ȡ�������ͼģ�;���
	gluProject(0,0,0,modelMatrix,g_p->m_projMatrix[aim],g_p->m_port[g_p->m_activeView],&wx1,&wy1,&wz1);//���ĵ�
	gluProject(0,0,1,modelMatrix,g_p->m_projMatrix[aim],g_p->m_port[g_p->m_activeView],&wx2,&wy2,&wz2);//����������
	glPopMatrix();//���������ģ�;���
	glTranslated(p->m_translate[0],p->m_translate[1],p->m_translate[2]);//Ȼ���ƶ�
	glMultMatrixd(p->m_mvMatrix);//����ת
	glScaled(p->m_size,p->m_size,p->m_size);//�ȷ���
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);//��ȡ�������ͼģ�;���
	gluUnProject(wx1,wy1,wz1,modelMatrix,g_p->m_projMatrix[aim],g_p->m_port[g_p->m_activeView],&cx,&cy,&cz);
	gluUnProject(wx2,wy2,wz2,modelMatrix,g_p->m_projMatrix[aim],g_p->m_port[g_p->m_activeView],&nx,&ny,&nz);
	glPopMatrix();
	nx=nx-cx;
	ny=ny-cy;
	nz=nz-cz;//���㷨����,���ܲ��ǵ�λ����
}
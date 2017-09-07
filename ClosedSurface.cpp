// CS.cpp: implementation of the CS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MM.h"
#include "ClosedSurface.h"
#include "gl\gl.h"
#include "gl\glu.h"
#include "math.h"
#include "MMDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern bool g_renderMode;
extern CMMDlg *g_p;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CS::CS()
{
	m_num_vertex=0;
	m_num_triangle=0;
	m_selected=0;//δѡ��
	m_visible=0; //��ʼ��ʱ���治���ڣ���Ȼ���ɼ�
	m_free=1;//����ʱ����༭
	iv=new VertexRef;//����ͷ�ڵ�
	it=new Triangle;
	iv->next=NULL;
	it->next=NULL;
	tx=ty=tz=0;
	m_translate[0]=m_translate[1]=m_translate[2]=0;
	m_size=1;
	m_rotateAxis[0]=1;//��ת��
	m_rotateAxis[1]=0;
	m_rotateAxis[2]=0;
	m_angle=0;//��ת��

	//�õ�λ�����ʼ����ͼ����
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX,m_mvMatrix);
	glPopMatrix();
}

CS::~CS()//����������ɾ�����ж����������
{
	TriangleRef *q,*tempTR;
	VertexRef *p,*tempVR;
	Triangle *u,*tempT;
	//ɾ�����и�����
	p=iv;
	while (p->next)
	{		
		tempVR=p;
		p=p->next;
		delete tempVR;
		//ɾ����˶���ӳ���������ָ��
		q=p->nextRef;
		while (q->nextRef)
		{
			tempTR=q;
			q=q->nextRef;
			delete tempTR;
		}
		delete q;
	} 
	delete p;
	//ɾ������������
	u=it;
	while (u->next)
	{
		tempT=u;
		u=u->next;
		delete tempT;
	}
	delete u;
}

void CS::createCuboid(Cuboid &pc)//����һ��������
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;
	Triangle *p=it;
	int i,j;
	GLdouble x1,y1,z1,x2,y2,z2; 
	//��
	for(i=0;i<pc.nx;i++)
	{
		for (j=0;j<pc.ny;j++)
		{
			x1=-pc.lx/2+i*pc.lx/pc.nx;
			y1=-pc.ly/2+j*pc.ly/pc.ny;
			x2=-pc.lx/2+(i+1)*pc.lx/pc.nx;
			y2=-pc.ly/2+(j+1)*pc.ly/pc.ny;
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=y1;
			p->pt[0].v[2]=pc.lz/2;
			p->pt[1].v[0]=x2;
			p->pt[1].v[1]=y1;
			p->pt[1].v[2]=pc.lz/2;
			p->pt[2].v[0]=x2;
			p->pt[2].v[1]=y2;
			p->pt[2].v[2]=pc.lz/2;

			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x2;
			p->pt[0].v[1]=y2;
			p->pt[0].v[2]=pc.lz/2;			
			p->pt[1].v[0]=x1;
			p->pt[1].v[1]=y2;
			p->pt[1].v[2]=pc.lz/2;			
			p->pt[2].v[0]=x1;
			p->pt[2].v[1]=y1;
			p->pt[2].v[2]=pc.lz/2;
		}
	}
	//��
	for(i=0;i<pc.nx;i++)
	{
		for (j=0;j<pc.ny;j++)
		{
			x1=-pc.lx/2+i*pc.lx/pc.nx;
			y1=-pc.ly/2+j*pc.ly/pc.ny;
			x2=-pc.lx/2+(i+1)*pc.lx/pc.nx;
			y2=-pc.ly/2+(j+1)*pc.ly/pc.ny;
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=y1;
			p->pt[0].v[2]=-pc.lz/2;
			p->pt[1].v[0]=x2;
			p->pt[1].v[1]=y2;
			p->pt[1].v[2]=-pc.lz/2;
			p->pt[2].v[0]=x2;
			p->pt[2].v[1]=y1;
			p->pt[2].v[2]=-pc.lz/2;
						
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x2;
			p->pt[0].v[1]=y2;
			p->pt[0].v[2]=-pc.lz/2;
			p->pt[1].v[0]=x1;
			p->pt[1].v[1]=y1;
			p->pt[1].v[2]=-pc.lz/2;
			p->pt[2].v[0]=x1;
			p->pt[2].v[1]=y2;
			p->pt[2].v[2]=-pc.lz/2;					
		}
	}
	//ǰ
	for(i=0;i<pc.ny;i++)
	{
		for(j=0;j<pc.nz;j++)
		{
			y1=-pc.ly/2+i*pc.ly/pc.ny;
			z1=-pc.lz/2+j*pc.lz/pc.nz;
			y2=-pc.ly/2+(i+1)*pc.ly/pc.ny;
			z2=-pc.lz/2+(j+1)*pc.lz/pc.nz;
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=pc.lx/2;
			p->pt[0].v[1]=y1;
			p->pt[0].v[2]=z1;
			p->pt[1].v[0]=pc.lx/2;
			p->pt[1].v[1]=y2;
			p->pt[1].v[2]=z1;
			p->pt[2].v[0]=pc.lx/2;
			p->pt[2].v[1]=y2;
			p->pt[2].v[2]=z2;
						
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=pc.lx/2;
			p->pt[0].v[1]=y2;
			p->pt[0].v[2]=z2;
			p->pt[1].v[0]=pc.lx/2;
			p->pt[1].v[1]=y1;
			p->pt[1].v[2]=z2;
			p->pt[2].v[0]=pc.lx/2;
			p->pt[2].v[1]=y1;
			p->pt[2].v[2]=z1;				
		}
	}
	//��
	for(i=0;i<pc.ny;i++)
	{
		for (j=0;j<pc.nz;j++)
		{
			y1=-pc.ly/2+i*pc.ly/pc.ny;
			z1=-pc.lz/2+j*pc.lz/pc.nz;
			y2=-pc.ly/2+(i+1)*pc.ly/pc.ny;
			z2=-pc.lz/2+(j+1)*pc.lz/pc.nz;
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=-pc.lx/2;
			p->pt[0].v[1]=y1;
			p->pt[0].v[2]=z1;
			p->pt[1].v[0]=-pc.lx/2;
			p->pt[1].v[1]=y2;
			p->pt[1].v[2]=z2;
			p->pt[2].v[0]=-pc.lx/2;
			p->pt[2].v[1]=y2;
			p->pt[2].v[2]=z1;
						
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=-pc.lx/2;
			p->pt[0].v[1]=y2;
			p->pt[0].v[2]=z2;
			p->pt[1].v[0]=-pc.lx/2;
			p->pt[1].v[1]=y1;
			p->pt[1].v[2]=z1;
			p->pt[2].v[0]=-pc.lx/2;
			p->pt[2].v[1]=y1;
			p->pt[2].v[2]=z2;				
		}
	}
	//��
	for(i=0;i<pc.nx;i++)
	{
		for (j=0;j<pc.nz;j++)
		{
			x1=-pc.lx/2+i*pc.lx/pc.nx;
			z1=-pc.lz/2+j*pc.lz/pc.nz;
			x2=-pc.lx/2+(i+1)*pc.lx/pc.nx;
			z2=-pc.lz/2+(j+1)*pc.lz/pc.nz;
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=-pc.ly/2;
			p->pt[0].v[2]=z1;
			p->pt[1].v[0]=x2;
			p->pt[1].v[1]=-pc.ly/2;
			p->pt[1].v[2]=z1;
			p->pt[2].v[0]=x2;
			p->pt[2].v[1]=-pc.ly/2;
			p->pt[2].v[2]=z2;
						
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x2;
			p->pt[0].v[1]=-pc.ly/2;
			p->pt[0].v[2]=z2;
			p->pt[1].v[0]=x1;
			p->pt[1].v[1]=-pc.ly/2;
			p->pt[1].v[2]=z2;
			p->pt[2].v[0]=x1;
			p->pt[2].v[1]=-pc.ly/2;
			p->pt[2].v[2]=z1;				
		}
	}
	//��
	for(i=0;i<pc.nx;i++)
	{
		for (j=0;j<pc.nz;j++)
		{
			x1=-pc.lx/2+i*pc.lx/pc.nx;
			z1=-pc.lz/2+j*pc.lz/pc.nz;
			x2=-pc.lx/2+(i+1)*pc.lx/pc.nx;
			z2=-pc.lz/2+(j+1)*pc.lz/pc.nz;
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=pc.ly/2;
			p->pt[0].v[2]=z1;
			p->pt[1].v[0]=x2;
			p->pt[1].v[1]=pc.ly/2;
			p->pt[1].v[2]=z2;
			p->pt[2].v[0]=x2;
			p->pt[2].v[1]=pc.ly/2;
			p->pt[2].v[2]=z1;
						
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x2;
			p->pt[0].v[1]=pc.ly/2;
			p->pt[0].v[2]=z2;
			p->pt[1].v[0]=x1;
			p->pt[1].v[1]=pc.ly/2;
			p->pt[1].v[2]=z1;
			p->pt[2].v[0]=x1;
			p->pt[2].v[1]=pc.ly/2;
			p->pt[2].v[2]=z2;				
		}
	}
	p->next=NULL;

	m_name="�½�������";
	createVertexRef();//��Triangle����ȡVertexRef��
	m_visible=1;//��ʱ����ɼ�
	m_selected=1;//����ʱ��ѡ��
}

void CS::createVertexRef()//������������õ�����������
{
	Triangle *p=it;
	//�Ƚϵĳ�ʼֵ�����ڲ��ұ߿�
	minX=maxX=it->next->pt[0].v[0];
	minY=maxY=it->next->pt[0].v[1];
	minZ=maxZ=it->next->pt[0].v[2];
	while (p->next) //������������
	{
		p=p->next;
		//�������ε��������㶼����˳����ң�˳����ҿ���Ч�ʲ���
		for (short i=0;i<3;i++) 
		{
			//˳�������������Χ
			if(p->pt[i].v[0]<minX) minX=p->pt[i].v[0];
			if(p->pt[i].v[0]>maxX) maxX=p->pt[i].v[0];
			if(p->pt[i].v[1]<minY) minY=p->pt[i].v[1];
			if(p->pt[i].v[1]>maxY) maxY=p->pt[i].v[1];
			if(p->pt[i].v[2]<minZ) minZ=p->pt[i].v[2];
			if(p->pt[i].v[2]>maxZ) maxZ=p->pt[i].v[2];
			buildRef(iv,&p->pt[i],p);
		}
	}
}

void CS::show(short viewManner,double workMode)//����ģ��
{
	if(m_visible) //����ɼ�
	{
		glLoadName(m_index);//�滻����
		glPushMatrix();
		glTranslated(tx+m_translate[0],ty+m_translate[1],tz+m_translate[2]);//���ƽ��
		glRotated(m_angle,m_rotateAxis[0],m_rotateAxis[1],m_rotateAxis[2]);//�ٳ��ϵ�ǰ����ת����
		glMultMatrixd(m_mvMatrix);//�ٳ����Ѵ������ת����		
		glScaled(m_size,m_size,m_size);//�Ƚ��з���
		
		if (workMode==5&&m_selected)//����ѡ��״̬
		{
			VertexRef *q=iv;
			glColor3ub(0,255,0);//��ѡ����ʾΪ��ɫ
			glBegin(GL_POINTS);
			while (q->next)
			{
				q=q->next;
				if (q->selected)
				{
					glColor3ub(255,0,0);//ѡ�е�Ϊ��ɫ						
					glVertex3d(q->v[0],q->v[1],q->v[2]);
					glColor3ub(0,255,0);//�л�����ɫ
				}
				else glVertex3d(q->v[0],q->v[1],q->v[2]);
			}
			glEnd();
		}
		if (workMode==7.1&&m_selected) //��ѡ������״̬
		{
			Triangle *p=it;
			GLuint index=0,cur=1;
			glColor3ub(0,255,0);//��ѡ����ʾΪ��ɫ
			glBegin(GL_POINTS);
			while (p->next)
			{
				p=p->next;
				index++;
				if (index==g_p->m_hits[cur]&&cur<=g_p->m_hits[0])
				{
					cur++;
					glColor3ub(255,0,0);//ѡ�е�Ϊ��ɫ						
					glVertex3d((p->pt[0].v[0]+p->pt[1].v[0]+p->pt[2].v[0])/3,(p->pt[0].v[1]+p->pt[1].v[1]+p->pt[2].v[1])/3,(p->pt[0].v[2]+p->pt[1].v[2]+p->pt[2].v[2])/3);
					glColor3ub(0,255,0);//�л�����ɫ
				}
				else glVertex3d((p->pt[0].v[0]+p->pt[1].v[0]+p->pt[2].v[0])/3,(p->pt[0].v[1]+p->pt[1].v[1]+p->pt[2].v[1])/3,(p->pt[0].v[2]+p->pt[1].v[2]+p->pt[2].v[2])/3);
			}
			glEnd();
		}
		glColor3ub(m_color[0],m_color[1],m_color[2]);
		Triangle *p;
		if (viewManner)//��������
		{	
			glEnable(GL_LIGHTING);//��������
			p=it;				
			glBegin(GL_TRIANGLES);
			GLdouble nv[3];//������
			while (p->next)
			{
				p=p->next;
				getNVector(p->pt[0].v,p->pt[1].v,p->pt[2].v,nv);
				glNormal3dv(nv);
				glVertex3d(p->pt[0].v[0],p->pt[0].v[1],p->pt[0].v[2]);
				glVertex3d(p->pt[1].v[0],p->pt[1].v[1],p->pt[1].v[2]);
				glVertex3d(p->pt[2].v[0],p->pt[2].v[1],p->pt[2].v[2]);
			}
			glEnd();
			glDisable(GL_LIGHTING);//�رչ���
			if (m_selected&&g_renderMode&&workMode!=5)//��ѡ����,��������(������Ⱦģʽ��)
			{
				if(m_free) glColor3ub(255,255,255);//���Ϊ��ɫ
				else glColor3ub(0,0,0);
				GLdouble dx=(maxX-minX)/3,dy=(maxY-minY)/3,dz=(maxZ-minZ)/3;
				glBegin(GL_LINES);
					glVertex3d(minX,minY,minZ);
					glVertex3d(minX+dx,minY,minZ);
					glVertex3d(minX,minY,minZ);
					glVertex3d(minX,minY+dy,minZ);
					glVertex3d(minX,minY,minZ);
					glVertex3d(minX,minY,minZ+dz);

					glVertex3d(maxX,minY,minZ);
					glVertex3d(maxX-dy,minY,minZ);
					glVertex3d(maxX,minY,minZ);
					glVertex3d(maxX,minY+dy,minZ);
					glVertex3d(maxX,minY,minZ);
					glVertex3d(maxX,minY,minZ+dz);

					glVertex3d(maxX,maxY,minZ);
					glVertex3d(maxX-dx,maxY,minZ);
					glVertex3d(maxX,maxY,minZ);
					glVertex3d(maxX,maxY-dy,minZ);
					glVertex3d(maxX,maxY,minZ);
					glVertex3d(maxX,maxY,minZ+dz);

					glVertex3d(minX,maxY,minZ);
					glVertex3d(minX+dx,maxY,minZ);
					glVertex3d(minX,maxY,minZ);
					glVertex3d(minX,maxY-dy,minZ);
					glVertex3d(minX,maxY,minZ);
					glVertex3d(minX,maxY,minZ+dz);

					glVertex3d(minX,minY,maxZ);
					glVertex3d(minX+dx,minY,maxZ);
					glVertex3d(minX,minY,maxZ);
					glVertex3d(minX,minY+dy,maxZ);
					glVertex3d(minX,minY,maxZ);
					glVertex3d(minX,minY,maxZ-dz);

					glVertex3d(maxX,minY,maxZ);
					glVertex3d(maxX-dx,minY,maxZ);
					glVertex3d(maxX,minY,maxZ);
					glVertex3d(maxX,minY+dy,maxZ);
					glVertex3d(maxX,minY,maxZ);
					glVertex3d(maxX,minY,maxZ-dz);

					glVertex3d(maxX,maxY,maxZ);
					glVertex3d(maxX-dx,maxY,maxZ);
					glVertex3d(maxX,maxY,maxZ);
					glVertex3d(maxX,maxY-dy,maxZ);
					glVertex3d(maxX,maxY,maxZ);
					glVertex3d(maxX,maxY,maxZ-dz);

					glVertex3d(minX,maxY,maxZ);
					glVertex3d(minX+dx,maxY,maxZ);
					glVertex3d(minX,maxY,maxZ);
					glVertex3d(minX,maxY-dy,maxZ);
					glVertex3d(minX,maxY,maxZ);
					glVertex3d(minX,maxY,maxZ-dz);
				glEnd();
			}
		}
		else //������������
		{
			glDisable(GL_LIGHTING);
			if (m_selected) 
			{
				if(m_free) glColor3ub(255,255,255); //ѡ����ʹ�ð�ɫͻ����ʾ
				else glColor3ub(0,0,0);
			}
			p=it;
			while (p->next)//����������
			{
				p=p->next;
				glBegin(GL_LINE_LOOP);
				glVertex3d(p->pt[0].v[0],p->pt[0].v[1],p->pt[0].v[2]);
				glVertex3d(p->pt[1].v[0],p->pt[1].v[1],p->pt[1].v[2]);
				glVertex3d(p->pt[2].v[0],p->pt[2].v[1],p->pt[2].v[2]);
				glEnd();
			}
		}		
		glPopMatrix();
	}	
}

bool CS::loadCS(CString path)//���ļ���������ģ��
{
	if(it->next==NULL)//ֻ�пյ�CS�������ļ�����
	{
		Triangle *p=it;
		FILE *fp=fopen(path,"r");
		if (!fp)
		{
			AfxMessageBox("�ļ���ʧ�ܣ�");
			return FALSE;
		}
		if (readLine(fp)!="This is a single ModelMaker file")
		{
			AfxMessageBox("�ⲻ��һ��smm�ļ���");
			return FALSE;
		}
		fscanf(fp,"%lf",&m_size);
		for (int j=0;j<16;j++)
		{
			fscanf(fp,"%lf",&m_mvMatrix[j]);
		}
		fscanf(fp,"%lf",&m_translate[0]);
		fscanf(fp,"%lf",&m_translate[1]);
		fscanf(fp,"%lf",&m_translate[2]);
		fscanf(fp,"%d",&m_color[0]);
		fscanf(fp,"%d",&m_color[1]);
		fscanf(fp,"%d",&m_color[2]);
		fscanf(fp,"%d",&m_num_triangle);
		char name[40];
		fscanf(fp,"%s",&name);
		m_name=name;
		//��������������
		for(int i=0;i<m_num_triangle;i++)
		{
			p->next=new Triangle;
			p=p->next;
			fscanf(fp,"%lf%lf%lf",&p->pt[0].v[0],&p->pt[0].v[1],&p->pt[0].v[2]);
			fscanf(fp,"%lf%lf%lf",&p->pt[1].v[0],&p->pt[1].v[1],&p->pt[1].v[2]);
			fscanf(fp,"%lf%lf%lf",&p->pt[2].v[0],&p->pt[2].v[1],&p->pt[2].v[2]);
		}
		p->next=NULL;
		fclose(fp);
		createVertexRef();//��ȡ��Ӧ�Ķ�����
		m_visible=1;//��ʱ����ɼ�
		return TRUE;
	}
	else 
	{
		AfxMessageBox("��������������룡");
		return FALSE;
	}
}

void CS::densify(bool type)//��������
{
	Triangle *p=it,*q;
	VertexRef *m=iv;
	GLuint index=0,cur=1;
	bool needDensify;
	while (m->next) m=m->next;
	while (p->next)
	{
		index++;//ָʾ�����ε����
		if (type) needDensify=1;//ȫ������
		else
		{
			if (index==g_p->m_hits[cur]&&cur<=g_p->m_hits[0])//����������Ҫ����
			{
				cur++;
				needDensify=1;
			}
			else needDensify=0;
		}	

		if(needDensify)
		{
			q=p->next;
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=q->pt[0].v[0];
			p->pt[0].v[1]=q->pt[0].v[1];
			p->pt[0].v[2]=q->pt[0].v[2];
			p->pt[0].tr=q->pt[0].tr;
			p->pt[0].tr->p=&p->pt[0];
			
			p->pt[1].v[0]=(q->pt[0].v[0]+q->pt[1].v[0])/2;
			p->pt[1].v[1]=(q->pt[0].v[1]+q->pt[1].v[1])/2;
			p->pt[1].v[2]=(q->pt[0].v[2]+q->pt[1].v[2])/2;
			buildRef(m,&p->pt[1],p);
			p->pt[2].v[0]=(q->pt[0].v[0]+q->pt[2].v[0])/2;
			p->pt[2].v[1]=(q->pt[0].v[1]+q->pt[2].v[1])/2;
			p->pt[2].v[2]=(q->pt[0].v[2]+q->pt[2].v[2])/2;
			buildRef(m,&p->pt[2],p);
			//////////////////////////////////////////////////////////
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=q->pt[1].v[0];
			p->pt[0].v[1]=q->pt[1].v[1];
			p->pt[0].v[2]=q->pt[1].v[2];
			p->pt[0].tr=q->pt[1].tr;
			p->pt[0].tr->p=&p->pt[0];
			
			p->pt[1].v[0]=(q->pt[2].v[0]+q->pt[1].v[0])/2;
			p->pt[1].v[1]=(q->pt[2].v[1]+q->pt[1].v[1])/2;
			p->pt[1].v[2]=(q->pt[2].v[2]+q->pt[1].v[2])/2;
			buildRef(m,&p->pt[1],p);
			p->pt[2].v[0]=(q->pt[0].v[0]+q->pt[1].v[0])/2;
			p->pt[2].v[1]=(q->pt[0].v[1]+q->pt[1].v[1])/2;
			p->pt[2].v[2]=(q->pt[0].v[2]+q->pt[1].v[2])/2;
			buildRef(m,&p->pt[2],p);
			//////////////////////////////////////////////////////////
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=q->pt[2].v[0];
			p->pt[0].v[1]=q->pt[2].v[1];
			p->pt[0].v[2]=q->pt[2].v[2];
			p->pt[0].tr=q->pt[2].tr;
			p->pt[0].tr->p=&p->pt[0];
			
			p->pt[1].v[0]=(q->pt[2].v[0]+q->pt[0].v[0])/2;
			p->pt[1].v[1]=(q->pt[2].v[1]+q->pt[0].v[1])/2;
			p->pt[1].v[2]=(q->pt[2].v[2]+q->pt[0].v[2])/2;
			buildRef(m,&p->pt[1],p);
			p->pt[2].v[0]=(q->pt[2].v[0]+q->pt[1].v[0])/2;
			p->pt[2].v[1]=(q->pt[2].v[1]+q->pt[1].v[1])/2;
			p->pt[2].v[2]=(q->pt[2].v[2]+q->pt[1].v[2])/2;
			buildRef(m,&p->pt[2],p);
			//////////////////////////////////////////////////////////
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=(q->pt[0].v[0]+q->pt[1].v[0])/2;
			p->pt[0].v[1]=(q->pt[0].v[1]+q->pt[1].v[1])/2;
			p->pt[0].v[2]=(q->pt[0].v[2]+q->pt[1].v[2])/2;
			buildRef(m,&p->pt[0],p);
			p->pt[1].v[0]=(q->pt[2].v[0]+q->pt[1].v[0])/2;
			p->pt[1].v[1]=(q->pt[2].v[1]+q->pt[1].v[1])/2;
			p->pt[1].v[2]=(q->pt[2].v[2]+q->pt[1].v[2])/2;
			buildRef(m,&p->pt[1],p);
			p->pt[2].v[0]=(q->pt[0].v[0]+q->pt[2].v[0])/2;
			p->pt[2].v[1]=(q->pt[0].v[1]+q->pt[2].v[1])/2;
			p->pt[2].v[2]=(q->pt[0].v[2]+q->pt[2].v[2])/2;
			buildRef(m,&p->pt[2],p);
			p->next=q->next;
			delete q;
		}
		else p=p->next;
	}
	g_p->m_hits[0]=0;
}

void CS::densify(GLdouble increase)//��������increase�����ط����ƶ��ľ���
{
	Triangle *p=it,*q,*t;
	VertexRef *m=iv;
	TriangleRef *tr;
	short i=0;
	GLdouble d1,d2,d3;
	while (m->next) m=m->next;
	///////////////����ʱ�����ɷ������͸��ߵ�����/////////////////
	while (p->next)
	{
		t=p->next;//ָ��ԭ���������νڵ�
		p=p->next;
		getNVector(p->pt[0].v,p->pt[1].v,p->pt[2].v,p->v0);//��ȡABC�ķ�����
		//������AB�ڽӵ�������
		tr=p->pt[0].vr->nextRef;
		while (tr)
		{
			if (tr->t!=p) //��ȥԭ����������
			{
				if(tr->t->pt[0].vr==p->pt[1].vr) break;
				if(tr->t->pt[1].vr==p->pt[1].vr) break;
				if(tr->t->pt[2].vr==p->pt[1].vr) break;				
			}
			tr=tr->nextRef;
		}
		q=tr->t; //��ʱp,qָ����ABΪ�����ߵ�����������
		getNVector(q->pt[0].v,q->pt[1].v,q->pt[2].v,p->v1);
		for (i=0;i<3;i++) p->v1[i]+=p->v0[i];//�������������
		vectorNomalize(p->v1);//������λ��
		//������AC�ڽӵ�������
		tr=p->pt[0].vr->nextRef;
		while (tr)
		{
			if (tr->t!=p) //��ȥԭ����������
			{
				if(tr->t->pt[0].vr==p->pt[2].vr) break;
				if(tr->t->pt[1].vr==p->pt[2].vr) break;
				if(tr->t->pt[2].vr==p->pt[2].vr) break;	
			}
			tr=tr->nextRef;
		}
		q=tr->t;//��ʱp,qָ����ACΪ�����ߵ�����������
		getNVector(q->pt[0].v,q->pt[1].v,q->pt[2].v,p->v2);
		for (i=0;i<3;i++) p->v2[i]+=p->v0[i];//�������������
		vectorNomalize(p->v2);//������λ��
		//������BC�ڽӵ�������
		tr=p->pt[1].vr->nextRef;
		while (tr)
		{
			if (tr->t!=p) //��ȥԭ����������
			{
				if(tr->t->pt[0].vr==p->pt[2].vr) break;
				if(tr->t->pt[1].vr==p->pt[2].vr) break;
				if(tr->t->pt[2].vr==p->pt[2].vr) break;	
			}
			tr=tr->nextRef;
		}
		q=tr->t;//��ʱp,qָ����BCΪ�����ߵ�����������
		getNVector(q->pt[0].v,q->pt[1].v,q->pt[2].v,p->v3);
		for (i=0;i<3;i++) p->v3[i]+=p->v0[i];//�������������
		vectorNomalize(p->v3);//������λ��
	}
	////////////////////////////////����µĵ�////////////////////////////////////////////
	t=p=it;//�ص�������ͷ���
	Triangle *temp=it->next;//ָ��ԭ���������ĵ�һ���ڵ�
	while (t->next)
	{
		t=t->next;
		d1=increase/(t->v1[0]*t->v0[0]+t->v1[1]*t->v0[1]+t->v1[2]*t->v0[2]);//��v1�����ƶ��ľ���
		d2=increase/(t->v2[0]*t->v0[0]+t->v2[1]*t->v0[1]+t->v2[2]*t->v0[2]);//��v2�����ƶ��ľ���
		d3=increase/(t->v3[0]*t->v0[0]+t->v3[1]*t->v0[1]+t->v3[2]*t->v0[2]);//��v3�����ƶ��ľ���
		p->next=new Triangle;
		p=p->next;
		
		p->pt[0].v[0]=t->pt[0].v[0];
		p->pt[0].v[1]=t->pt[0].v[1];
		p->pt[0].v[2]=t->pt[0].v[2];
		p->pt[0].vr=t->pt[0].vr;
		p->pt[0].tr=t->pt[0].tr;
		t->pt[0].tr->p=&p->pt[0];
		t->pt[0].tr->t=p;
		
		p->pt[1].v[0]=(t->pt[0].v[0]+t->pt[1].v[0])/2+d1*t->v1[0];
		p->pt[1].v[1]=(t->pt[0].v[1]+t->pt[1].v[1])/2+d1*t->v1[1];
		p->pt[1].v[2]=(t->pt[0].v[2]+t->pt[1].v[2])/2+d1*t->v1[2];
		buildRef(m,&p->pt[1],p);

		p->pt[2].v[0]=(t->pt[0].v[0]+t->pt[2].v[0])/2+d2*t->v2[0];
		p->pt[2].v[1]=(t->pt[0].v[1]+t->pt[2].v[1])/2+d2*t->v2[1];
		p->pt[2].v[2]=(t->pt[0].v[2]+t->pt[2].v[2])/2+d2*t->v2[2];
		buildRef(m,&p->pt[2],p);
		//////////////////////////////////////////////////////////
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=t->pt[1].v[0];
		p->pt[0].v[1]=t->pt[1].v[1];
		p->pt[0].v[2]=t->pt[1].v[2];
		p->pt[0].vr=t->pt[1].vr;
		p->pt[0].tr=t->pt[1].tr;
		t->pt[1].tr->p=&p->pt[0];
		t->pt[1].tr->t=p;
		
		p->pt[1].v[0]=(t->pt[2].v[0]+t->pt[1].v[0])/2+d3*t->v3[0];
		p->pt[1].v[1]=(t->pt[2].v[1]+t->pt[1].v[1])/2+d3*t->v3[1];
		p->pt[1].v[2]=(t->pt[2].v[2]+t->pt[1].v[2])/2+d3*t->v3[2];
		buildRef(m,&p->pt[1],p);
		p->pt[2].v[0]=(t->pt[0].v[0]+t->pt[1].v[0])/2+d1*t->v1[0];
		p->pt[2].v[1]=(t->pt[0].v[1]+t->pt[1].v[1])/2+d1*t->v1[1];
		p->pt[2].v[2]=(t->pt[0].v[2]+t->pt[1].v[2])/2+d1*t->v1[2];
		buildRef(m,&p->pt[2],p);
		//////////////////////////////////////////////////////////
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=t->pt[2].v[0];
		p->pt[0].v[1]=t->pt[2].v[1];
		p->pt[0].v[2]=t->pt[2].v[2];
		p->pt[0].vr=t->pt[2].vr;
		p->pt[0].tr=t->pt[2].tr;
		t->pt[2].tr->p=&p->pt[0];
		t->pt[2].tr->t=p;
		
		p->pt[1].v[0]=(t->pt[2].v[0]+t->pt[0].v[0])/2+d2*t->v2[0];
		p->pt[1].v[1]=(t->pt[2].v[1]+t->pt[0].v[1])/2+d2*t->v2[1];
		p->pt[1].v[2]=(t->pt[2].v[2]+t->pt[0].v[2])/2+d2*t->v2[2];
		buildRef(m,&p->pt[1],p);
		p->pt[2].v[0]=(t->pt[2].v[0]+t->pt[1].v[0])/2+d3*t->v3[0];
		p->pt[2].v[1]=(t->pt[2].v[1]+t->pt[1].v[1])/2+d3*t->v3[1];
		p->pt[2].v[2]=(t->pt[2].v[2]+t->pt[1].v[2])/2+d3*t->v3[2];
		buildRef(m,&p->pt[2],p);
		//////////////////////////////////////////////////////////
		p->next=new Triangle;
		p=p->next;
		p->next=NULL;
		p->pt[0].v[0]=(t->pt[0].v[0]+t->pt[1].v[0])/2+d1*t->v1[0];
		p->pt[0].v[1]=(t->pt[0].v[1]+t->pt[1].v[1])/2+d1*t->v1[1];
		p->pt[0].v[2]=(t->pt[0].v[2]+t->pt[1].v[2])/2+d1*t->v1[2];
		buildRef(m,&p->pt[0],p);
		p->pt[1].v[0]=(t->pt[2].v[0]+t->pt[1].v[0])/2+d3*t->v3[0];
		p->pt[1].v[1]=(t->pt[2].v[1]+t->pt[1].v[1])/2+d3*t->v3[1];
		p->pt[1].v[2]=(t->pt[2].v[2]+t->pt[1].v[2])/2+d3*t->v3[2];
		buildRef(m,&p->pt[1],p);
		p->pt[2].v[0]=(t->pt[0].v[0]+t->pt[2].v[0])/2+d2*t->v2[0];
		p->pt[2].v[1]=(t->pt[0].v[1]+t->pt[2].v[1])/2+d2*t->v2[1];
		p->pt[2].v[2]=(t->pt[0].v[2]+t->pt[2].v[2])/2+d2*t->v2[2];
		buildRef(m,&p->pt[2],p);
	}
	//ɾ��ԭ�������εĽڵ�
	while (temp)
	{
		p=temp;
		temp=temp->next;
		delete p;
	}
}

void CS::buildRef(VertexRef *begin,Vertex *v,Triangle *t)//��begin˳�����v�������໥ӳ���ϵ��t��V���ڵ�Triangle
{
	//begin��VertexRefͷ���ָ��
	VertexRef *q=begin; 
	TriangleRef *m;
	bool success=0;
	while (q->next)
	{
		q=q->next;
		if ((q->v[0]-v->v[0])*(q->v[0]-v->v[0])+(q->v[1]-v->v[1])*(q->v[1]-v->v[1])+(q->v[2]-v->v[2])*(q->v[2]-v->v[2])<1e-8) //VertexRef��������z���Ѿ����ڸõ�
		{
			v->v[0]=q->v[0];
			v->v[1]=q->v[1];
			v->v[2]=q->v[2];//������һ��
			m=q->nextRef;
			while (m->nextRef) m=m->nextRef;//�ƶ������
			m->nextRef=new TriangleRef;
			m=m->nextRef;
			m->nextRef=NULL;//�½�һ��TriangleRef
			m->p=v; 
			m->t=t;//��TriangleRef�����Triangleӳ����ָ��
			v->tr=m; 
			v->vr=q;//��Triangle�ĵ������VertexRef��TriangleRef��ӳ��ָ��
			success=1;//����������
			break;
		}
	}
	if (!success)//�������û�ҵ��������½ڵ㣬q��Ȼָ�����һ��VertexRef�ڵ�
	{
		q->next=new VertexRef;
		q=q->next;
		q->v[0]=v->v[0];
		q->v[1]=v->v[1];
		q->v[2]=v->v[2];
		q->selected=0;//�¼ӵĵ�δ��ѡ��
		q->next=NULL;
		q->nextRef=m=new TriangleRef;
		m->nextRef=NULL;
		m->p=v; 
		m->t=t;//��TriangleRef�����Triangleӳ����ָ��
		v->tr=m; 
		v->vr=q;//��Triangle�ĵ������VertexRef��TriangleRef��ӳ��ָ��
	}		
}

void CS::getNVector(GLdouble *p1, GLdouble *p2, GLdouble *p3, GLdouble *vector)//�����ȡ������
{
	GLdouble w[3],v[3],nx,ny,nz,r;
	for (byte i=0;i<3;i++)
	{
		w[i]=p1[i]-p2[i];
		v[i]=p3[i]-p2[i];
	}
	nx=v[1]*w[2]-v[2]*w[1];
	ny=v[2]*w[0]-v[0]*w[2];
	nz=v[0]*w[1]-v[1]*w[0];
	r=sqrt(nx*nx+ny*ny+nz*nz);
	vector[0]=nx/r;
	vector[1]=ny/r;
	vector[2]=nz/r;	
}

void CS::vectorNomalize(GLdouble *vector) //������λ��
{
	GLdouble r=sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
	vector[0]/=r;
	vector[1]/=r;
	vector[2]/=r;
}

void CS::storeMVMatrix()//���ƶ�����ת������󴢴�����
{
	glPushMatrix();
	glLoadIdentity();
	glRotated(m_angle,m_rotateAxis[0],m_rotateAxis[1],m_rotateAxis[2]);//�ٳ��ϵ�ǰ����ת����
	glMultMatrixd(m_mvMatrix);//�ȳ����Ѵ������ͼ����	
	glGetDoublev(GL_MODELVIEW_MATRIX,m_mvMatrix);//��ȡ��˺����ת����
	glPopMatrix();
	m_translate[0]+=tx;
	m_translate[1]+=ty;
	m_translate[2]+=tz;//��������ƽ��
	tx=ty=tz=0;
	m_angle=0;
}

void CS::createCylinder(Cylinder &pc)//�����ݴ�������
{		
	if (pc.type==0)//������
	{
		m_translate[0]=pc.cx;
		m_translate[1]=pc.cy;
		m_translate[2]=pc.cz;//�����ƶ���
		Triangle *p=it,*q,*mark;
		int i;
		double x1,y1,x2,y2,x3,y3,x4,y4; 
		double ks=sin(PI/pc.n),kc=cos(PI/pc.n);
		//����Ͷ���
		for (i=1;i<=pc.nr;i++)
		{
			x1=i*pc.R/pc.nr*kc;
			y1=-i*pc.R/pc.nr*ks;
			x2=x1;
			y2=-y1;
			x3=(i-1)*pc.R/pc.nr*kc;
			y3=-(i-1)*pc.R/pc.nr*ks;
			x4=x3;
			y4=-y3;

			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=y1;
			p->pt[0].v[2]=0;
			
			p->pt[1].v[0]=x3;
			p->pt[1].v[1]=y3;
			p->pt[1].v[2]=0;
			
			p->pt[2].v[0]=x2;
			p->pt[2].v[1]=y2;
			p->pt[2].v[2]=0;

			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=y1;
			p->pt[0].v[2]=pc.h;
			
			p->pt[1].v[0]=x2;
			p->pt[1].v[1]=y2;
			p->pt[1].v[2]=pc.h;
			
			p->pt[2].v[0]=x3;
			p->pt[2].v[1]=y3;
			p->pt[2].v[2]=pc.h;

			if (i!=1)
			{
				p->next=new Triangle;
				p=p->next;
				p->pt[0].v[0]=x2;
				p->pt[0].v[1]=y2;
				p->pt[0].v[2]=0;
				
				p->pt[1].v[0]=x3;
				p->pt[1].v[1]=y3;
				p->pt[1].v[2]=0;
				
				p->pt[2].v[0]=x4;
				p->pt[2].v[1]=y4;
				p->pt[2].v[2]=0;
				
				p->next=new Triangle;
				p=p->next;
				p->pt[0].v[0]=x2;
				p->pt[0].v[1]=y2;
				p->pt[0].v[2]=pc.h;
				
				p->pt[1].v[0]=x4;
				p->pt[1].v[1]=y4;
				p->pt[1].v[2]=pc.h;
				
				p->pt[2].v[0]=x3;
				p->pt[2].v[1]=y3;
				p->pt[2].v[2]=pc.h;
			}
		}
		//����
		x1=pc.R*kc;
		y1=pc.R*ks;
		for (i=1;i<=pc.nh;i++)
		{
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=-y1;
			p->pt[0].v[2]=(i-1)*pc.h/pc.nh;
			
			p->pt[1].v[0]=x1;
			p->pt[1].v[1]=y1;
			p->pt[1].v[2]=(i-1)*pc.h/pc.nh;
			
			p->pt[2].v[0]=x1;
			p->pt[2].v[1]=-y1;
			p->pt[2].v[2]=i*pc.h/pc.nh;

			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=y1;
			p->pt[0].v[2]=(i-1)*pc.h/pc.nh;
			
			p->pt[1].v[0]=x1;
			p->pt[1].v[1]=y1;
			p->pt[1].v[2]=i*pc.h/pc.nh;
			
			p->pt[2].v[0]=x1;
			p->pt[2].v[1]=-y1;
			p->pt[2].v[2]=i*pc.h/pc.nh;
		}

		q=mark=p;//�������һ��������λ��
		for (i=1;i<pc.n;i++)
		{
			ks=sin(i*2*PI/pc.n);
			kc=cos(i*2*PI/pc.n);
			p=it;
			do 
			{
				p=p->next;
				q->next=new Triangle;
				q=q->next;
				q->pt[0].v[0]=p->pt[0].v[0]*kc-p->pt[0].v[1]*ks;
				q->pt[0].v[1]=p->pt[0].v[0]*ks+p->pt[0].v[1]*kc;
				q->pt[0].v[2]=p->pt[0].v[2];
				
				q->pt[1].v[0]=p->pt[1].v[0]*kc-p->pt[1].v[1]*ks;
				q->pt[1].v[1]=p->pt[1].v[0]*ks+p->pt[1].v[1]*kc;
				q->pt[1].v[2]=p->pt[1].v[2];
				
				q->pt[2].v[0]=p->pt[2].v[0]*kc-p->pt[2].v[1]*ks;
				q->pt[2].v[1]=p->pt[2].v[0]*ks+p->pt[2].v[1]*kc;
				q->pt[2].v[2]=p->pt[2].v[2];				
			}while(p!=mark);
		}
		q->next=NULL;
	}
	else //����Ϊ���������
	{
		g_p->m_p2t.correctDirection(pc.ehead);//����Ϊ��ʱ�����
		//�ȹ�������
		int i=0;
		Triangle *px=it;
		Edge *pe=pc.ehead;
		while (pe->next)
		{
			pe=pe->next;
			for (i=0;i<pc.nh;i++)
			{
				px->next=new Triangle;
				px=px->next;
				px->pt[0].v[0]=pe->x1;
				px->pt[0].v[1]=pe->y1;
				px->pt[0].v[2]=i*pc.h/pc.nh;
				px->pt[1].v[0]=pe->x2;
				px->pt[1].v[1]=pe->y2;
				px->pt[1].v[2]=i*pc.h/pc.nh;
				px->pt[2].v[0]=pe->x2;
				px->pt[2].v[1]=pe->y2;
				px->pt[2].v[2]=(i+1)*pc.h/pc.nh;

				px->next=new Triangle;
				px=px->next;
				px->pt[0].v[0]=pe->x1;
				px->pt[0].v[1]=pe->y1;
				px->pt[0].v[2]=i*pc.h/pc.nh;
				px->pt[1].v[0]=pe->x2;
				px->pt[1].v[1]=pe->y2;
				px->pt[1].v[2]=(i+1)*pc.h/pc.nh;
				px->pt[2].v[0]=pe->x1;
				px->pt[2].v[1]=pe->y1;
				px->pt[2].v[2]=(i+1)*pc.h/pc.nh;
			}
		}
		//��ʱpxָ�����һ��������
		Triangle *pt=g_p->m_p2t.polygonToTriangles(pc.ehead,FALSE);//�ʷ�������
		Triangle *thead=g_p->m_p2t.copyAnotherSide(pt);//�����ʷֺõ������Σ����ı�߶�
		Triangle *p=thead;
		while(p->next)
		{
			p=p->next;
			p->pt[0].v[2]=pc.h;
			p->pt[1].v[2]=pc.h;
			p->pt[2].v[2]=pc.h;
		}
		//����������
		px->next=pt->next;
		delete pt;
		while (px->next) px=px->next;
		px->next=thead->next;
		delete thead;
		g_p->changeWorkMode(1);//����ѡ��ģʽ
	}	
	m_name="�½�����";
	createVertexRef();//��Triangle����ȡVertexRef��
	m_visible=1;//��ʱ����ɼ�
	m_selected=1;//����ʱ��ѡ��
}

void CS::createCone(Cylinder &pc)//�����ݴ���׶��
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;//�����ƶ���
	Triangle *p=it,*q,*mark;
	int i;	
	if (pc.type==0)//������
	{
		double x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4; 
		double ks=sin(PI/pc.n),kc=cos(PI/pc.n);
		//����
		for (i=1;i<=pc.nr;i++)
		{
			x1=i*pc.R/pc.nr*kc;
			y1=-i*pc.R/pc.nr*ks;
			x2=x1;
			y2=-y1;
			x3=(i-1)*pc.R/pc.nr*kc;
			y3=-(i-1)*pc.R/pc.nr*ks;
			x4=x3;
			y4=-y3;

			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=y1;
			p->pt[0].v[2]=0;
			
			p->pt[1].v[0]=x3;
			p->pt[1].v[1]=y3;
			p->pt[1].v[2]=0;
			
			p->pt[2].v[0]=x2;
			p->pt[2].v[1]=y2;
			p->pt[2].v[2]=0;

			if (i!=1)
			{
				p->next=new Triangle;
				p=p->next;
				p->pt[0].v[0]=x2;
				p->pt[0].v[1]=y2;
				p->pt[0].v[2]=0;
				
				p->pt[1].v[0]=x3;
				p->pt[1].v[1]=y3;
				p->pt[1].v[2]=0;
				
				p->pt[2].v[0]=x4;
				p->pt[2].v[1]=y4;
				p->pt[2].v[2]=0;
			}
		}
		//����
		for (i=1;i<=pc.nh;i++)
		{
			x1=i*pc.R/pc.nh*kc;
			y1=-i*pc.R/pc.nh*ks;
			z1=(pc.nh-i)*pc.h/pc.nh;
			x2=x1;
			y2=-y1;
			z2=z1;
			x3=(i-1)*pc.R/pc.nh*kc;
			y3=-(i-1)*pc.R/pc.nh*ks;
			z3=(pc.nh-i+1)*pc.h/pc.nh;
			x4=x3;
			y4=-y3;
			z4=z3;
			p->next=new Triangle;
			p=p->next;
			p->pt[0].v[0]=x1;
			p->pt[0].v[1]=y1;
			p->pt[0].v[2]=z1;
			
			p->pt[1].v[0]=x2;
			p->pt[1].v[1]=y2;
			p->pt[1].v[2]=z2;
			
			p->pt[2].v[0]=x3;
			p->pt[2].v[1]=y3;
			p->pt[2].v[2]=z3;

			if (i!=1)
			{
				p->next=new Triangle;
				p=p->next;
				p->pt[0].v[0]=x2;
				p->pt[0].v[1]=y2;
				p->pt[0].v[2]=z2;
				
				p->pt[1].v[0]=x4;
				p->pt[1].v[1]=y4;
				p->pt[1].v[2]=z4;
				
				p->pt[2].v[0]=x3;
				p->pt[2].v[1]=y3;
				p->pt[2].v[2]=z3;
			}		
		}
		q=mark=p;//�������һ��������λ��
		for (i=1;i<pc.n;i++)
		{
			ks=sin(i*2*PI/pc.n);
			kc=cos(i*2*PI/pc.n);
			p=it;
			do 
			{
				p=p->next;
				q->next=new Triangle;
				q=q->next;
				q->pt[0].v[0]=p->pt[0].v[0]*kc-p->pt[0].v[1]*ks;
				q->pt[0].v[1]=p->pt[0].v[0]*ks+p->pt[0].v[1]*kc;
				q->pt[0].v[2]=p->pt[0].v[2];
				
				q->pt[1].v[0]=p->pt[1].v[0]*kc-p->pt[1].v[1]*ks;
				q->pt[1].v[1]=p->pt[1].v[0]*ks+p->pt[1].v[1]*kc;
				q->pt[1].v[2]=p->pt[1].v[2];
				
				q->pt[2].v[0]=p->pt[2].v[0]*kc-p->pt[2].v[1]*ks;
				q->pt[2].v[1]=p->pt[2].v[0]*ks+p->pt[2].v[1]*kc;
				q->pt[2].v[2]=p->pt[2].v[2];				
			}while(p!=mark);
		}
		q->next=NULL;
	}
	else
	{

	}	
	m_name="�½���׶";
	createVertexRef();//��Triangle����ȡVertexRef��
	m_visible=1;//��ʱ����ɼ�
	m_selected=1;//����ʱ��ѡ��
}

void CS::createSphere(Sphere &pc)
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;//�����ƶ���
	Triangle *p=it,*q,*mark;
	int i;
	double ks=sin(2*PI/pc.nj),kc=cos(2*PI/pc.nj);
	double x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
    //����һ��
	p->next=new Triangle;
	p=p->next;
	p->pt[0].v[0]=pc.R*sin(PI/pc.nw);
	p->pt[0].v[1]=0;
	p->pt[0].v[2]=pc.R*cos(PI/pc.nw);
	
	p->pt[1].v[0]=pc.R*sin(PI/pc.nw)*kc;
	p->pt[1].v[1]=pc.R*sin(PI/pc.nw)*ks;
	p->pt[1].v[2]=pc.R*cos(PI/pc.nw);
	
	p->pt[2].v[0]=0;
	p->pt[2].v[1]=0;
	p->pt[2].v[2]=pc.R;

	for(i=2;i<=pc.nw;i++)
	{
		x1=pc.R*sin(i*PI/pc.nw);
		y1=0;
		z1=pc.R*cos(i*PI/pc.nw);

		x2=x1*kc;
		y2=x1*ks;
		z2=z1;
		
		x3=pc.R*sin((i-1)*PI/pc.nw);
		y3=0;
		z3=pc.R*cos((i-1)*PI/pc.nw);

		x4=x3*kc;
		y4=x3*ks;
		z4=z3;
		
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=x1;
		p->pt[0].v[1]=y1;
		p->pt[0].v[2]=z1;
		
		p->pt[1].v[0]=x2;
		p->pt[1].v[1]=y2;
		p->pt[1].v[2]=z2;
		
		p->pt[2].v[0]=x3;
		p->pt[2].v[1]=y3;
		p->pt[2].v[2]=z3;		

		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=x2;
		p->pt[0].v[1]=y2;
		p->pt[0].v[2]=z2;
		
		p->pt[1].v[0]=x4;
		p->pt[1].v[1]=y4;
		p->pt[1].v[2]=z4;
		
		p->pt[2].v[0]=x3;
		p->pt[2].v[1]=y3;
		p->pt[2].v[2]=z3;
	}
	p->next=new Triangle;
	p=p->next;
	p->pt[0].v[0]=pc.R*sin((pc.nw-1)*PI/pc.nw);
	p->pt[0].v[1]=0;
	p->pt[0].v[2]=pc.R*cos((pc.nw-1)*PI/pc.nw);
	
	p->pt[1].v[0]=0;
	p->pt[1].v[1]=0;
	p->pt[1].v[2]=-pc.R;
	
	p->pt[2].v[0]=pc.R*sin((pc.nw-1)*PI/pc.nw)*kc;
	p->pt[2].v[1]=pc.R*sin((pc.nw-1)*PI/pc.nw)*ks;
	p->pt[2].v[2]=pc.R*cos((pc.nw-1)*PI/pc.nw);

	//����nj-1��
	q=mark=p;//�������һ��������λ��
	for (i=1;i<pc.nj;i++)
	{
		ks=sin(i*2*PI/pc.nj);
		kc=cos(i*2*PI/pc.nj);
		p=it;
		do 
		{
			p=p->next;
			q->next=new Triangle;
			q=q->next;
			q->pt[0].v[0]=p->pt[0].v[0]*kc-p->pt[0].v[1]*ks;
			q->pt[0].v[1]=p->pt[0].v[0]*ks+p->pt[0].v[1]*kc;
			q->pt[0].v[2]=p->pt[0].v[2];
			
			q->pt[1].v[0]=p->pt[1].v[0]*kc-p->pt[1].v[1]*ks;
			q->pt[1].v[1]=p->pt[1].v[0]*ks+p->pt[1].v[1]*kc;
			q->pt[1].v[2]=p->pt[1].v[2];
			
			q->pt[2].v[0]=p->pt[2].v[0]*kc-p->pt[2].v[1]*ks;
			q->pt[2].v[1]=p->pt[2].v[0]*ks+p->pt[2].v[1]*kc;
			q->pt[2].v[2]=p->pt[2].v[2];				
		}while(p!=mark);
	}
	q->next=NULL;

	m_name="�½�����";
	createVertexRef();//��Triangle����ȡVertexRef��
	m_visible=1;//��ʱ����ɼ�
	m_selected=1;//����ʱ��ѡ��
}

void CS::createTorus(Torus &pc)
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;//�����ƶ���
	Triangle *p=it,*q,*mark;
	int i;
	double ks=sin(2*PI/pc.nj),kc=cos(2*PI/pc.nj);
	double x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
    //����һ��

	for(i=1;i<=pc.nw;i++)
	{
		x1=pc.R+pc.r*sin(i*2*PI/pc.nw);
		y1=-x1*tan(PI/pc.nj);
		z1=pc.r*cos(i*2*PI/pc.nw);

		x2=x1;
		y2=-y1;
		z2=z1;
		
		x3=pc.R+pc.r*sin((i-1)*2*PI/pc.nw);
		y3=-x3*tan(PI/pc.nj);
		z3=pc.r*cos((i-1)*2*PI/pc.nw);

		x4=x3;
		y4=-y3;
		z4=z3;
		
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=x1;
		p->pt[0].v[1]=y1;
		p->pt[0].v[2]=z1;
		
		p->pt[1].v[0]=x2;
		p->pt[1].v[1]=y2;
		p->pt[1].v[2]=z2;
		
		p->pt[2].v[0]=x3;
		p->pt[2].v[1]=y3;
		p->pt[2].v[2]=z3;		

		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=x2;
		p->pt[0].v[1]=y2;
		p->pt[0].v[2]=z2;
		
		p->pt[1].v[0]=x4;
		p->pt[1].v[1]=y4;
		p->pt[1].v[2]=z4;
		
		p->pt[2].v[0]=x3;
		p->pt[2].v[1]=y3;
		p->pt[2].v[2]=z3;
	}
	//����nj-1��
	q=mark=p;//�������һ��������λ��
	for (i=1;i<pc.nj;i++)
	{
		ks=sin(i*2*PI/pc.nj);
		kc=cos(i*2*PI/pc.nj);
		p=it;
		do 
		{
			p=p->next;
			q->next=new Triangle;
			q=q->next;
			q->pt[0].v[0]=p->pt[0].v[0]*kc-p->pt[0].v[1]*ks;
			q->pt[0].v[1]=p->pt[0].v[0]*ks+p->pt[0].v[1]*kc;
			q->pt[0].v[2]=p->pt[0].v[2];
			
			q->pt[1].v[0]=p->pt[1].v[0]*kc-p->pt[1].v[1]*ks;
			q->pt[1].v[1]=p->pt[1].v[0]*ks+p->pt[1].v[1]*kc;
			q->pt[1].v[2]=p->pt[1].v[2];
			
			q->pt[2].v[0]=p->pt[2].v[0]*kc-p->pt[2].v[1]*ks;
			q->pt[2].v[1]=p->pt[2].v[0]*ks+p->pt[2].v[1]*kc;
			q->pt[2].v[2]=p->pt[2].v[2];				
		}while(p!=mark);
	}
	q->next=NULL;

	m_name="�½�Բ��";
	createVertexRef();//��Triangle����ȡVertexRef��
	m_visible=1;//��ʱ����ɼ�
	m_selected=1;//����ʱ��ѡ��
}

void CS::createPipe(Pipe &pc)
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;//�����ƶ���
	Triangle *p=it,*q,*mark;
	int i;
	double x1,y1,z1,x2,y2,z2,x3,y3,x4,y4;
    //����һ��
	//����Ͷ���
	for(i=1;i<=pc.nr;i++)
	{
		x1=pc.r+i*(pc.R-pc.r)/pc.nr;
		y1=-x1*tan(PI/pc.n);
		
		x2=x1;
		y2=-y1;
		
		x3=pc.r+(i-1)*(pc.R-pc.r)/pc.nr;
		y3=-x3*tan(PI/pc.n);
		
		x4=x3;
		y4=-y3;
		
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=x1;
		p->pt[0].v[1]=y1;
		p->pt[0].v[2]=0;
		
		p->pt[1].v[0]=x3;
		p->pt[1].v[1]=y3;
		p->pt[1].v[2]=0;
		
		p->pt[2].v[0]=x2;
		p->pt[2].v[1]=y2;
		p->pt[2].v[2]=0;
		
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=x1;
		p->pt[0].v[1]=y1;
		p->pt[0].v[2]=pc.h;
		
		p->pt[1].v[0]=x2;
		p->pt[1].v[1]=y2;
		p->pt[1].v[2]=pc.h;
		
		p->pt[2].v[0]=x3;
		p->pt[2].v[1]=y3;
		p->pt[2].v[2]=pc.h;


		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=x2;
		p->pt[0].v[1]=y2;
		p->pt[0].v[2]=0;
		
		p->pt[1].v[0]=x3;
		p->pt[1].v[1]=y3;
		p->pt[1].v[2]=0;
		
		p->pt[2].v[0]=x4;
		p->pt[2].v[1]=y4;
		p->pt[2].v[2]=0;
		
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=x2;
		p->pt[0].v[1]=y2;
		p->pt[0].v[2]=pc.h;
		
		p->pt[1].v[0]=x4;
		p->pt[1].v[1]=y4;
		p->pt[1].v[2]=pc.h;
		
		p->pt[2].v[0]=x3;
		p->pt[2].v[1]=y3;
		p->pt[2].v[2]=pc.h;
	}
	//����
	y1=pc.R*tan(PI/pc.n);//���
	y2=pc.r*y1/pc.R;//�ڲ�
	for (i=1;i<=pc.nh;i++)
	{
		z1=(i-1)*pc.h/pc.nh;
		z2=i*pc.h/pc.nh;
		//���
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=pc.R;
		p->pt[0].v[1]=-y1;
		p->pt[0].v[2]=z1;
		
		p->pt[1].v[0]=pc.R;
		p->pt[1].v[1]=y1;
		p->pt[1].v[2]=z1;
		
		p->pt[2].v[0]=pc.R;
		p->pt[2].v[1]=-y1;
		p->pt[2].v[2]=z2;

		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=pc.R;
		p->pt[0].v[1]=y1;
		p->pt[0].v[2]=z1;
		
		p->pt[1].v[0]=pc.R;
		p->pt[1].v[1]=y1;
		p->pt[1].v[2]=z2;
		
		p->pt[2].v[0]=pc.R;
		p->pt[2].v[1]=-y1;
		p->pt[2].v[2]=z2;
		//�ڲ�
		
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=pc.r;
		p->pt[0].v[1]=-y2;
		p->pt[0].v[2]=z1;
		
		p->pt[1].v[0]=pc.r;
		p->pt[1].v[1]=-y2;
		p->pt[1].v[2]=z2;
		
		p->pt[2].v[0]=pc.r;
		p->pt[2].v[1]=y2;
		p->pt[2].v[2]=z1;
		
		p->next=new Triangle;
		p=p->next;
		p->pt[0].v[0]=pc.r;
		p->pt[0].v[1]=y2;
		p->pt[0].v[2]=z1;
		
		p->pt[1].v[0]=pc.r;
		p->pt[1].v[1]=-y2;
		p->pt[1].v[2]=z2;
		
		p->pt[2].v[0]=pc.r;
		p->pt[2].v[1]=y2;
		p->pt[2].v[2]=z2;
	}
	//����n-1��
	q=mark=p;//�������һ��������λ��
	for (i=1;i<pc.n;i++)
	{
		double ks=sin(i*2*PI/pc.n);
		double kc=cos(i*2*PI/pc.n);
		p=it;
		do 
		{
			p=p->next;
			q->next=new Triangle;
			q=q->next;
			q->pt[0].v[0]=p->pt[0].v[0]*kc-p->pt[0].v[1]*ks;
			q->pt[0].v[1]=p->pt[0].v[0]*ks+p->pt[0].v[1]*kc;
			q->pt[0].v[2]=p->pt[0].v[2];
			
			q->pt[1].v[0]=p->pt[1].v[0]*kc-p->pt[1].v[1]*ks;
			q->pt[1].v[1]=p->pt[1].v[0]*ks+p->pt[1].v[1]*kc;
			q->pt[1].v[2]=p->pt[1].v[2];
			
			q->pt[2].v[0]=p->pt[2].v[0]*kc-p->pt[2].v[1]*ks;
			q->pt[2].v[1]=p->pt[2].v[0]*ks+p->pt[2].v[1]*kc;
			q->pt[2].v[2]=p->pt[2].v[2];				
		}while(p!=mark);
	}
	q->next=NULL;
	
	m_name="�½�Բ��";
	createVertexRef();//��Triangle����ȡVertexRef��
	m_visible=1;//��ʱ����ɼ�
	m_selected=1;//����ʱ��ѡ��
	
}

void CS::copy(CS *p)//���ṩ��CS����õ�һ��clone
{
	//���Ƴ�Ա����
	m_angle=p->m_angle;
	m_color[0]=p->m_color[0];
	m_color[1]=p->m_color[1];
	m_color[2]=p->m_color[2];
	m_free=p->m_free;
	for(int i=0;i<16;i++)
	m_mvMatrix[i]=p->m_mvMatrix[i];
	m_name=p->m_name;
	m_num_triangle=p->m_num_triangle;
	m_num_vertex=p->m_num_vertex;
	m_rotateAxis[0]=p->m_rotateAxis[0];
	m_rotateAxis[1]=p->m_rotateAxis[1];
	m_rotateAxis[1]=p->m_rotateAxis[1];
	m_selected=p->m_selected;
	m_size=p->m_size;
	m_translate[0]=p->m_translate[0];
	m_translate[1]=p->m_translate[1];
	m_translate[2]=p->m_translate[2];
	m_visible=p->m_visible;
	maxX=p->maxX;
	maxY=p->maxY;
	maxZ=p->maxZ;
	minX=p->minX;
	minY=p->minY;
	minZ=p->minZ;
	tx=p->tx;
	ty=p->ty;
	tz=p->tz;
	//���ƶ�����
	Triangle *m=p->it,*n=it;
	while (m->next)
	{
		m=m->next;
		n->next=new Triangle;
		n=n->next;
		n->pt[0].v[0]=m->pt[0].v[0];
		n->pt[0].v[1]=m->pt[0].v[1];
		n->pt[0].v[2]=m->pt[0].v[2];
		n->pt[1].v[0]=m->pt[1].v[0];
		n->pt[1].v[1]=m->pt[1].v[1];
		n->pt[1].v[2]=m->pt[1].v[2];
		n->pt[2].v[0]=m->pt[2].v[0];
		n->pt[2].v[1]=m->pt[2].v[1];
		n->pt[2].v[2]=m->pt[2].v[2];
	}
	n->next=NULL;
	createVertexRef();//��Triangle����ȡVertexRef��
}

void CS::setColor(COLORREF color)
{
	m_color[0]=GetRValue(color);	
	m_color[1]=GetGValue(color);
	m_color[2]=GetBValue(color);	
}

int CS::getNumOfTriangles()
{
	Triangle *pt=it;
	m_num_triangle=0;
	while (pt->next)
	{
		pt=pt->next;
		m_num_triangle++;
	}
	return m_num_triangle;
}

CString CS::readLine(FILE *fp)
{	
	int ch='a';
	CString s;
	while((ch=fgetc(fp))!='\n')	s+=ch;
	return s;
}

void CS::getRange()//�������Χ
{
	GLdouble m[16];
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(m_translate[0],m_translate[1],m_translate[2]);
	glMultMatrixd(m_mvMatrix);
	glScaled(m_size,m_size,m_size);
	glGetDoublev(GL_MODELVIEW_MATRIX, m);//��ȡģ�;���
	glPopMatrix();
	//////////////////////////////////////////////////////
	VertexRef *pv=iv->next;
	GLdouble x,y,z;
	rmaxX=rminX=m[0]*pv->v[0]+m[4]*pv->v[1]+m[8]*pv->v[2]+m[12];
	rmaxY=rminY=m[1]*pv->v[0]+m[5]*pv->v[1]+m[9]*pv->v[2]+m[13];
	rmaxZ=rminZ=m[2]*pv->v[0]+m[6]*pv->v[1]+m[10]*pv->v[2]+m[14];
	while (pv->next)
	{
		pv=pv->next;
		x=m[0]*pv->v[0]+m[4]*pv->v[1]+m[8]*pv->v[2]+m[12];
		y=m[1]*pv->v[0]+m[5]*pv->v[1]+m[9]*pv->v[2]+m[13];
		z=m[2]*pv->v[0]+m[6]*pv->v[1]+m[10]*pv->v[2]+m[14];
		if (rmaxX<x) rmaxX=x;
		else if(rminX>x) rminX=x;
		if (rmaxY<y) rmaxY=y;
		else if(rminY>y) rminY=y;
		if (rmaxZ<z) rmaxZ=z;
		else if(rminZ>z) rminZ=z;
	}
}
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
	m_selected=0;//未选中
	m_visible=0; //初始化时曲面不存在，当然不可见
	m_free=1;//创建时允许编辑
	iv=new VertexRef;//申请头节点
	it=new Triangle;
	iv->next=NULL;
	it->next=NULL;
	tx=ty=tz=0;
	m_translate[0]=m_translate[1]=m_translate[2]=0;
	m_size=1;
	m_rotateAxis[0]=1;//旋转轴
	m_rotateAxis[1]=0;
	m_rotateAxis[2]=0;
	m_angle=0;//旋转角

	//用单位矩阵初始化视图矩阵
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX,m_mvMatrix);
	glPopMatrix();
}

CS::~CS()//析构函数，删除所有顶点和三角形
{
	TriangleRef *q,*tempTR;
	VertexRef *p,*tempVR;
	Triangle *u,*tempT;
	//删除所有辅助点
	p=iv;
	while (p->next)
	{		
		tempVR=p;
		p=p->next;
		delete tempVR;
		//删除与此顶点映射的三角形指针
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
	//删除所有三角形
	u=it;
	while (u->next)
	{
		tempT=u;
		u=u->next;
		delete tempT;
	}
	delete u;
}

void CS::createCuboid(Cuboid &pc)//创建一个正方体
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;
	Triangle *p=it;
	int i,j;
	GLdouble x1,y1,z1,x2,y2,z2; 
	//上
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
	//下
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
	//前
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
	//后
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
	//左
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
	//右
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

	m_name="新建长方体";
	createVertexRef();//由Triangle链获取VertexRef链
	m_visible=1;//此时曲面可见
	m_selected=1;//创建时被选中
}

void CS::createVertexRef()//由三角形链表得到辅助点链表
{
	Triangle *p=it;
	//比较的初始值，便于查找边框
	minX=maxX=it->next->pt[0].v[0];
	minY=maxY=it->next->pt[0].v[1];
	minZ=maxZ=it->next->pt[0].v[2];
	while (p->next) //遍历三角形链
	{
		p=p->next;
		//对三角形的三个顶点都进行顺序查找，顺序查找可能效率不高
		for (short i=0;i<3;i++) 
		{
			//顺便查找坐标的最大范围
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

void CS::show(short viewManner,double workMode)//绘制模型
{
	if(m_visible) //曲面可见
	{
		glLoadName(m_index);//替换名称
		glPushMatrix();
		glTranslated(tx+m_translate[0],ty+m_translate[1],tz+m_translate[2]);//最后平移
		glRotated(m_angle,m_rotateAxis[0],m_rotateAxis[1],m_rotateAxis[2]);//再乘上当前的旋转矩阵
		glMultMatrixd(m_mvMatrix);//再乘上已储存的旋转矩阵		
		glScaled(m_size,m_size,m_size);//先进行放缩
		
		if (workMode==5&&m_selected)//进入选点状态
		{
			VertexRef *q=iv;
			glColor3ub(0,255,0);//待选点显示为绿色
			glBegin(GL_POINTS);
			while (q->next)
			{
				q=q->next;
				if (q->selected)
				{
					glColor3ub(255,0,0);//选中点为红色						
					glVertex3d(q->v[0],q->v[1],q->v[2]);
					glColor3ub(0,255,0);//切换回绿色
				}
				else glVertex3d(q->v[0],q->v[1],q->v[2]);
			}
			glEnd();
		}
		if (workMode==7.1&&m_selected) //挑选三角形状态
		{
			Triangle *p=it;
			GLuint index=0,cur=1;
			glColor3ub(0,255,0);//待选点显示为绿色
			glBegin(GL_POINTS);
			while (p->next)
			{
				p=p->next;
				index++;
				if (index==g_p->m_hits[cur]&&cur<=g_p->m_hits[0])
				{
					cur++;
					glColor3ub(255,0,0);//选中点为红色						
					glVertex3d((p->pt[0].v[0]+p->pt[1].v[0]+p->pt[2].v[0])/3,(p->pt[0].v[1]+p->pt[1].v[1]+p->pt[2].v[1])/3,(p->pt[0].v[2]+p->pt[1].v[2]+p->pt[2].v[2])/3);
					glColor3ub(0,255,0);//切换回绿色
				}
				else glVertex3d((p->pt[0].v[0]+p->pt[1].v[0]+p->pt[2].v[0])/3,(p->pt[0].v[1]+p->pt[1].v[1]+p->pt[2].v[1])/3,(p->pt[0].v[2]+p->pt[1].v[2]+p->pt[2].v[2])/3);
			}
			glEnd();
		}
		glColor3ub(m_color[0],m_color[1],m_color[2]);
		Triangle *p;
		if (viewManner)//绘制曲面
		{	
			glEnable(GL_LIGHTING);//开启光照
			p=it;				
			glBegin(GL_TRIANGLES);
			GLdouble nv[3];//法向量
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
			glDisable(GL_LIGHTING);//关闭光照
			if (m_selected&&g_renderMode&&workMode!=5)//被选中了,需绘制外框(正常渲染模式下)
			{
				if(m_free) glColor3ub(255,255,255);//外框为白色
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
		else //仅绘制网格线
		{
			glDisable(GL_LIGHTING);
			if (m_selected) 
			{
				if(m_free) glColor3ub(255,255,255); //选中了使用白色突出显示
				else glColor3ub(0,0,0);
			}
			p=it;
			while (p->next)//绘制三角线
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

bool CS::loadCS(CString path)//从文件载入曲面模型
{
	if(it->next==NULL)//只有空的CS才能由文件载入
	{
		Triangle *p=it;
		FILE *fp=fopen(path,"r");
		if (!fp)
		{
			AfxMessageBox("文件打开失败！");
			return FALSE;
		}
		if (readLine(fp)!="This is a single ModelMaker file")
		{
			AfxMessageBox("这不是一个smm文件！");
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
		//读入三角形数据
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
		createVertexRef();//获取相应的顶点链
		m_visible=1;//此时曲面可见
		return TRUE;
	}
	else 
	{
		AfxMessageBox("操作错误，请检查代码！");
		return FALSE;
	}
}

void CS::densify(bool type)//加密网格
{
	Triangle *p=it,*q;
	VertexRef *m=iv;
	GLuint index=0,cur=1;
	bool needDensify;
	while (m->next) m=m->next;
	while (p->next)
	{
		index++;//指示三角形的序号
		if (type) needDensify=1;//全部加密
		else
		{
			if (index==g_p->m_hits[cur]&&cur<=g_p->m_hits[0])//此三角形需要加密
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

void CS::densify(GLdouble increase)//加密网格，increase代表沿法线移动的距离
{
	Triangle *p=it,*q,*t;
	VertexRef *m=iv;
	TriangleRef *tr;
	short i=0;
	GLdouble d1,d2,d3;
	while (m->next) m=m->next;
	///////////////加密时才生成法向量和各边的向量/////////////////
	while (p->next)
	{
		t=p->next;//指向原来的三角形节点
		p=p->next;
		getNVector(p->pt[0].v,p->pt[1].v,p->pt[2].v,p->v0);//获取ABC的法向量
		//查找与AB邻接的三角形
		tr=p->pt[0].vr->nextRef;
		while (tr)
		{
			if (tr->t!=p) //除去原来的三角形
			{
				if(tr->t->pt[0].vr==p->pt[1].vr) break;
				if(tr->t->pt[1].vr==p->pt[1].vr) break;
				if(tr->t->pt[2].vr==p->pt[1].vr) break;				
			}
			tr=tr->nextRef;
		}
		q=tr->t; //此时p,q指向以AB为公共边的两个三角形
		getNVector(q->pt[0].v,q->pt[1].v,q->pt[2].v,p->v1);
		for (i=0;i<3;i++) p->v1[i]+=p->v0[i];//两个法向量相加
		vectorNomalize(p->v1);//向量单位化
		//查找与AC邻接的三角形
		tr=p->pt[0].vr->nextRef;
		while (tr)
		{
			if (tr->t!=p) //除去原来的三角形
			{
				if(tr->t->pt[0].vr==p->pt[2].vr) break;
				if(tr->t->pt[1].vr==p->pt[2].vr) break;
				if(tr->t->pt[2].vr==p->pt[2].vr) break;	
			}
			tr=tr->nextRef;
		}
		q=tr->t;//此时p,q指向以AC为公共边的两个三角形
		getNVector(q->pt[0].v,q->pt[1].v,q->pt[2].v,p->v2);
		for (i=0;i<3;i++) p->v2[i]+=p->v0[i];//两个法向量相加
		vectorNomalize(p->v2);//向量单位化
		//查找与BC邻接的三角形
		tr=p->pt[1].vr->nextRef;
		while (tr)
		{
			if (tr->t!=p) //除去原来的三角形
			{
				if(tr->t->pt[0].vr==p->pt[2].vr) break;
				if(tr->t->pt[1].vr==p->pt[2].vr) break;
				if(tr->t->pt[2].vr==p->pt[2].vr) break;	
			}
			tr=tr->nextRef;
		}
		q=tr->t;//此时p,q指向以BC为公共边的两个三角形
		getNVector(q->pt[0].v,q->pt[1].v,q->pt[2].v,p->v3);
		for (i=0;i<3;i++) p->v3[i]+=p->v0[i];//两个法向量相加
		vectorNomalize(p->v3);//向量单位化
	}
	////////////////////////////////添加新的点////////////////////////////////////////////
	t=p=it;//回到三角形头结点
	Triangle *temp=it->next;//指向原三角形链的第一个节点
	while (t->next)
	{
		t=t->next;
		d1=increase/(t->v1[0]*t->v0[0]+t->v1[1]*t->v0[1]+t->v1[2]*t->v0[2]);//沿v1方向移动的距离
		d2=increase/(t->v2[0]*t->v0[0]+t->v2[1]*t->v0[1]+t->v2[2]*t->v0[2]);//沿v2方向移动的距离
		d3=increase/(t->v3[0]*t->v0[0]+t->v3[1]*t->v0[1]+t->v3[2]*t->v0[2]);//沿v3方向移动的距离
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
	//删除原来三角形的节点
	while (temp)
	{
		p=temp;
		temp=temp->next;
		delete p;
	}
}

void CS::buildRef(VertexRef *begin,Vertex *v,Triangle *t)//从begin顺序查找v并建立相互映射关系，t是V所在的Triangle
{
	//begin是VertexRef头结点指针
	VertexRef *q=begin; 
	TriangleRef *m;
	bool success=0;
	while (q->next)
	{
		q=q->next;
		if ((q->v[0]-v->v[0])*(q->v[0]-v->v[0])+(q->v[1]-v->v[1])*(q->v[1]-v->v[1])+(q->v[2]-v->v[2])*(q->v[2]-v->v[2])<1e-8) //VertexRef辅助点链z中已经存在该点
		{
			v->v[0]=q->v[0];
			v->v[1]=q->v[1];
			v->v[2]=q->v[2];//跟点链一致
			m=q->nextRef;
			while (m->nextRef) m=m->nextRef;//移动到最后
			m->nextRef=new TriangleRef;
			m=m->nextRef;
			m->nextRef=NULL;//新建一个TriangleRef
			m->p=v; 
			m->t=t;//在TriangleRef中添加Triangle映射点的指针
			v->tr=m; 
			v->vr=q;//在Triangle的点中添加VertexRef和TriangleRef的映射指针
			success=1;//表明搜索到
			break;
		}
	}
	if (!success)//搜索完毕没找到，创建新节点，q必然指向最后一个VertexRef节点
	{
		q->next=new VertexRef;
		q=q->next;
		q->v[0]=v->v[0];
		q->v[1]=v->v[1];
		q->v[2]=v->v[2];
		q->selected=0;//新加的点未被选中
		q->next=NULL;
		q->nextRef=m=new TriangleRef;
		m->nextRef=NULL;
		m->p=v; 
		m->t=t;//在TriangleRef中添加Triangle映射点的指针
		v->tr=m; 
		v->vr=q;//在Triangle的点中添加VertexRef和TriangleRef的映射指针
	}		
}

void CS::getNVector(GLdouble *p1, GLdouble *p2, GLdouble *p3, GLdouble *vector)//三点获取法向量
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

void CS::vectorNomalize(GLdouble *vector) //向量单位化
{
	GLdouble r=sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
	vector[0]/=r;
	vector[1]/=r;
	vector[2]/=r;
}

void CS::storeMVMatrix()//将移动和旋转放入矩阵储存起来
{
	glPushMatrix();
	glLoadIdentity();
	glRotated(m_angle,m_rotateAxis[0],m_rotateAxis[1],m_rotateAxis[2]);//再乘上当前的旋转矩阵
	glMultMatrixd(m_mvMatrix);//先乘上已储存的视图矩阵	
	glGetDoublev(GL_MODELVIEW_MATRIX,m_mvMatrix);//获取相乘后的旋转矩阵
	glPopMatrix();
	m_translate[0]+=tx;
	m_translate[1]+=ty;
	m_translate[2]+=tz;//积累所有平移
	tx=ty=tz=0;
	m_angle=0;
}

void CS::createCylinder(Cylinder &pc)//由数据创建柱体
{		
	if (pc.type==0)//正棱柱
	{
		m_translate[0]=pc.cx;
		m_translate[1]=pc.cy;
		m_translate[2]=pc.cz;//储存移动量
		Triangle *p=it,*q,*mark;
		int i;
		double x1,y1,x2,y2,x3,y3,x4,y4; 
		double ks=sin(PI/pc.n),kc=cos(PI/pc.n);
		//底面和顶面
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
		//侧面
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

		q=mark=p;//记下最后一个三角形位置
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
	else //底面为任意多面体
	{
		g_p->m_p2t.correctDirection(pc.ehead);//更改为逆时针边链
		//先构建侧面
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
		//此时px指向最后一个三角形
		Triangle *pt=g_p->m_p2t.polygonToTriangles(pc.ehead,FALSE);//剖分三角形
		Triangle *thead=g_p->m_p2t.copyAnotherSide(pt);//复制剖分好的三角形，并改变高度
		Triangle *p=thead;
		while(p->next)
		{
			p=p->next;
			p->pt[0].v[2]=pc.h;
			p->pt[1].v[2]=pc.h;
			p->pt[2].v[2]=pc.h;
		}
		//连接三部分
		px->next=pt->next;
		delete pt;
		while (px->next) px=px->next;
		px->next=thead->next;
		delete thead;
		g_p->changeWorkMode(1);//进入选择模式
	}	
	m_name="新建棱柱";
	createVertexRef();//由Triangle链获取VertexRef链
	m_visible=1;//此时曲面可见
	m_selected=1;//创建时被选中
}

void CS::createCone(Cylinder &pc)//由数据创建锥体
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;//储存移动量
	Triangle *p=it,*q,*mark;
	int i;	
	if (pc.type==0)//正棱柱
	{
		double x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4; 
		double ks=sin(PI/pc.n),kc=cos(PI/pc.n);
		//底面
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
		//侧面
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
		q=mark=p;//记下最后一个三角形位置
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
	m_name="新建棱锥";
	createVertexRef();//由Triangle链获取VertexRef链
	m_visible=1;//此时曲面可见
	m_selected=1;//创建时被选中
}

void CS::createSphere(Sphere &pc)
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;//储存移动量
	Triangle *p=it,*q,*mark;
	int i;
	double ks=sin(2*PI/pc.nj),kc=cos(2*PI/pc.nj);
	double x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
    //绘制一瓣
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

	//复制nj-1瓣
	q=mark=p;//记下最后一个三角形位置
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

	m_name="新建球体";
	createVertexRef();//由Triangle链获取VertexRef链
	m_visible=1;//此时曲面可见
	m_selected=1;//创建时被选中
}

void CS::createTorus(Torus &pc)
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;//储存移动量
	Triangle *p=it,*q,*mark;
	int i;
	double ks=sin(2*PI/pc.nj),kc=cos(2*PI/pc.nj);
	double x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
    //绘制一瓣

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
	//复制nj-1瓣
	q=mark=p;//记下最后一个三角形位置
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

	m_name="新建圆环";
	createVertexRef();//由Triangle链获取VertexRef链
	m_visible=1;//此时曲面可见
	m_selected=1;//创建时被选中
}

void CS::createPipe(Pipe &pc)
{
	m_translate[0]=pc.cx;
	m_translate[1]=pc.cy;
	m_translate[2]=pc.cz;//储存移动量
	Triangle *p=it,*q,*mark;
	int i;
	double x1,y1,z1,x2,y2,z2,x3,y3,x4,y4;
    //绘制一瓣
	//底面和顶面
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
	//侧面
	y1=pc.R*tan(PI/pc.n);//外侧
	y2=pc.r*y1/pc.R;//内侧
	for (i=1;i<=pc.nh;i++)
	{
		z1=(i-1)*pc.h/pc.nh;
		z2=i*pc.h/pc.nh;
		//外侧
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
		//内侧
		
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
	//复制n-1瓣
	q=mark=p;//记下最后一个三角形位置
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
	
	m_name="新建圆环";
	createVertexRef();//由Triangle链获取VertexRef链
	m_visible=1;//此时曲面可见
	m_selected=1;//创建时被选中
	
}

void CS::copy(CS *p)//由提供的CS对象得到一个clone
{
	//复制成员变量
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
	//复制堆数据
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
	createVertexRef();//由Triangle链获取VertexRef链
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

void CS::getRange()//查找最大范围
{
	GLdouble m[16];
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(m_translate[0],m_translate[1],m_translate[2]);
	glMultMatrixd(m_mvMatrix);
	glScaled(m_size,m_size,m_size);
	glGetDoublev(GL_MODELVIEW_MATRIX, m);//获取模型矩阵
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
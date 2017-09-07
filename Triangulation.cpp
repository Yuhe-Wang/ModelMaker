// Triangulation.cpp: implementation of the Triangulation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MM.h"
#include "Triangulation.h"
#include "math.h"
#define DELTA 0.000001

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef struct _Radius
{
	double x,y,z,r;
	_Radius *next;
}Radius;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Triangulation::Triangulation()
{
	vn[0]=vn[1]=0;
	vn[2]=1;//Ĭ��Ϊƽ�����������Ϊz��������
}

Triangulation::~Triangulation()
{

}

Triangle * Triangulation::polygonToTriangles(Edge * head,bool side,GLdouble *pn)//������и��������,side=TRUE��ʾ���������������ʱ���
{	
	if (pn)//���·����������3ά���
	{
		vn[0]=pn[0];
		vn[1]=pn[1];
		vn[2]=pn[2];
	}
	///////////////////////////////////////////////////
	Triangle *thead=new Triangle;
	thead->next=NULL;
	Triangle *pt;
	Edge *p=head->next,*q;
	//����Ƿ��й��������߶Σ�����ȥ��
	bool sim=0;
	double x1,y1,z1,x2,y2,z2;
	while (p->next)
	{
		x1=p->x2-p->x1;
		y1=p->y2-p->y1;
		z1=p->z2-p->z1;
		x2=p->next->x2-p->next->x1;
		y2=p->next->y2-p->next->y1;
		z2=p->next->z2-p->next->z1;
		if (fabs(y1*z2-y2*z1)<DELTA&&fabs(x1*z2-x2*z1)<DELTA&&fabs(x1*y2-x2*y1)<DELTA)//��������
		{
			q=p->next;
			p->next=q->next;
			p->x2=q->x2;
			p->y2=q->y2;
			p->z2=q->z2;
			sim=1;
			delete q;
		}
		else p=p->next;
	}
	if(sim) AfxMessageBox("simple!");
	
	Radius *rhead=new Radius;
	rhead->next=NULL;//�����뾶����ͷָ��
	Radius *temp=NULL;
	int num=0;//����
	p=head;
	while (p->next) //���ұߵĸ���
	{
		p=p->next;		
		num++;
	}
	p=head->next;//pʼ��ָ���һ����
	while (num>3)
	{
		q=p; 
		while (q->next) //q������һ����֮���������
		{
			q=q->next;
			if(isInner(p,q)&&isSeen(q,head)) 
			{
				temp=new Radius;
				temp->next=rhead->next;
				rhead->next=temp;
				temp->x=q->x2;
				temp->y=q->y2;
				temp->z=q->z2;
				double a,b,c,d;
				a=sqrt((temp->x-p->x1)*(temp->x-p->x1)+(temp->y-p->y1)*(temp->y-p->y1)+(temp->z-p->z1)*(temp->z-p->z1));
				b=sqrt((temp->x-p->x2)*(temp->x-p->x2)+(temp->y-p->y2)*(temp->y-p->y2)+(temp->z-p->z2)*(temp->z-p->z2));
				c=sqrt((p->x1-p->x2)*(p->x1-p->x2)+(p->y1-p->y2)*(p->y1-p->y2)+(p->z1-p->z2)*(p->z1-p->z2));
				d=(a+b+c)/2;
				temp->r=a*b*c/4/sqrt(d*(d-a)*(d-b)*(d-c));
			}
		}
		//���ҵ���������С��
		if (rhead->next==NULL)
		{
			CString s;
			s.Format("num==%d",num);
			AfxMessageBox("û�в��ҵ��ڲ����Ӷ��㣡\n"+s);
		}
		double rmin=rhead->next->r;
		Radius *prmin=rhead->next;//��С�ߵ�ָ��
		temp=rhead;
		while (temp->next)
		{
			temp=temp->next;
			if(temp->r<rmin)
			{
				rmin=temp->r;
				prmin=temp;
			}
		}
		//ѹ��������
		pt=new Triangle;
		pt->next=thead->next;
		thead->next=pt;
		if (side)
		{		
			pt->pt[0].v[0]=p->x1;
			pt->pt[0].v[1]=p->y1;
			pt->pt[0].v[2]=p->z1;
			pt->pt[1].v[0]=p->x2;
			pt->pt[1].v[1]=p->y2;
			pt->pt[1].v[2]=p->z2;
			pt->pt[2].v[0]=prmin->x;
			pt->pt[2].v[1]=prmin->y;
			pt->pt[2].v[2]=prmin->z;
		}
		else
		{
			pt->pt[0].v[0]=p->x1;
			pt->pt[0].v[1]=p->y1;
			pt->pt[0].v[2]=p->z1;
			pt->pt[2].v[0]=p->x2;
			pt->pt[2].v[1]=p->y2;
			pt->pt[2].v[2]=p->z2;
			pt->pt[1].v[0]=prmin->x;
			pt->pt[1].v[1]=prmin->y;
			pt->pt[1].v[2]=prmin->z;
		}
		//�����ҵ��������εı��Ƿ����ڱ�����������ɾ���������������
		Edge *k;
		byte indicator1=0,indicator2=0;
		q=p;
		while (q->next)
		{
			k=q;
			q=q->next;
			if(q->x1==p->x2&&q->y1==p->y2&&q->z1==p->z2)
			{
				if(q->x2==prmin->x&&q->y2==prmin->y&&q->z2==prmin->z)  //�ָ�����������б�����ԭ������
				{
					k->next=q->next;
					delete q;
					q=k->next;
					num--;
					indicator1=1;
					if(q==NULL) break;//���һ����
				}
			}
			if(q->x2==p->x1&&q->y2==p->y1&&q->z2==p->z1) 
			{
				if(q->x1==prmin->x&&q->y1==prmin->y&&q->z1==prmin->z) //�ָ�����������б�����ԭ������
				{
					k->next=q->next;
					delete q;
					q=k->next;
					num--;
					indicator2=1;
					if(q==NULL) break;
				}
			}
		}		
		//��Ҫ�����±ߵ���������
		if (indicator1==0)
		{
			Edge *pe=new Edge;
			pe->x1=prmin->x;
			pe->y1=prmin->y;
			pe->z1=prmin->z;
			pe->x2=p->x2;
			pe->y2=p->y2;
			pe->z2=p->z2;
			pe->next=p->next;
			p->next=pe;
			num++;
		}
		if (indicator2==0)
		{
			Edge *pe=new Edge;
			pe->x2=prmin->x;
			pe->y2=prmin->y;
			pe->z2=prmin->z;
			pe->x1=p->x1;
			pe->y1=p->y1;
			pe->z1=p->z1;
			pe->next=p->next;
			p->next=pe;
			num++;
		}
		//ɾ����һ���ڵ�
		head->next=p->next;
		delete p;
		p=head->next;//��֤pָ���һ����
		num--;
		//������Ϻ���յ����������´�ѹ����ֵ
		temp=rhead->next;
		while (temp)
		{
			prmin=temp;
			temp=temp->next;
			delete prmin;
		}
		rhead->next=NULL;
	}
	//ѹ�����һ��������
	pt=new Triangle;
	pt->next=thead->next;
	thead->next=pt;
	if (side)
	{		
		pt->pt[0].v[0]=p->x1;
		pt->pt[0].v[1]=p->y1;
		pt->pt[0].v[2]=p->z1;
		pt->pt[1].v[0]=p->x2;
		pt->pt[1].v[1]=p->y2;
		pt->pt[1].v[2]=p->z2;
		pt->pt[2].v[0]=p->next->x2;
		pt->pt[2].v[1]=p->next->y2;
		pt->pt[2].v[2]=p->next->z2;
	}
	else
	{
		pt->pt[0].v[0]=p->x1;
		pt->pt[0].v[1]=p->y1;
		pt->pt[0].v[2]=p->z1;
		pt->pt[2].v[0]=p->x2;
		pt->pt[2].v[1]=p->y2;
		pt->pt[2].v[2]=p->z2;
		pt->pt[1].v[0]=p->next->x2;
		pt->pt[1].v[1]=p->next->y2;
		pt->pt[1].v[2]=p->next->z2;
	}	
	//ɾ��������,����ͷָ�룬�����´μ�������
	p=head;
	while (p->next)
	{
		q=p;
		p=p->next;
		delete q;
	}
	delete p;
	return thead;
}

bool Triangulation::isInner(Edge *e1,Edge *e2)//�ж��ܷ񹹳��������Σ�3ά
{
	double x1=e1->x2-e1->x1,y1=e1->y2-e1->y1,z1=e1->z2-e1->z1,
			x2=e2->x2-e1->x2,y2=e2->y2-e1->y2,z2=e2->z2-e1->z2;
	double r=vn[0]*(y1*z2-y2*z1)-vn[1]*(x1*z2-x2*z1)+vn[2]*(x1*y2-x2*y1);
	if(r>0) return TRUE;
	else return FALSE;
}

bool Triangulation::isIntersect(Edge *e1,Edge *e2)//�ж������߶��Ƿ��ཻ��3ά
{
	/*double r1=(e1->x2-e2->x1)*(e2->y2-e2->y1)-(e1->y2-e2->y1)*(e2->x2-e2->x1);
	double r2=(e2->x2-e2->x1)*(e1->y1-e2->y1)-(e2->y2-e2->y1)*(e1->x1-e2->x1);
	double r3=(e2->x2-e1->x1)*(e1->y2-e1->y1)-(e2->y2-e1->y1)*(e1->x2-e1->x1);
	double r4=(e1->x2-e1->x1)*(e2->y1-e1->y1)-(e1->y2-e1->y1)*(e2->x1-e1->x1); 
	if (r1*r2>0&&r3*r4>0) return TRUE;
	else return FALSE;
	*/
	double x1=e1->x1-e2->x1,y1=e1->y1-e2->y1,z1=e1->z1-e2->z1, 
		x2=e2->x2-e2->x1,y2=e2->y2-e2->y1,z2=e2->z2-e2->z1,
		x3=e1->x2-e2->x1,y3=e1->y2-e2->y1,z3=e1->z2-e2->z1;
	double r=-(x2*x2+y2*y2+z2*z2)*(x1*x3+y1*y3+z1*z3)+(x2*x3+y2*y3+z2*z3)*(x1*x2+y1*y2+z1*z2);
	if (r>0)
	{
		x1=e2->x1-e1->x1,y1=e2->y1-e1->y1,z1=e2->z1-e1->z1, 
		x2=e1->x2-e1->x1,y2=e1->y2-e1->y1,z2=e1->z2-e1->z1,
		x3=e2->x2-e1->x1,y3=e2->y2-e1->y1,z3=e2->z2-e1->z1;
		r=-(x2*x2+y2*y2+z2*z2)*(x1*x3+y1*y3+z1*z3)+(x2*x3+y2*y3+z2*z3)*(x1*x2+y1*y2+z1*z2);
		if (r>0) return TRUE;
		return FALSE;
	}
	else return FALSE;
}

bool Triangulation::isSeen(Edge *p,Edge *head)//�Ƿ���ӣ�3ά
{
	Edge e1,e2;
	Edge *q=head->next;
	e1.x1=q->x1;
	e1.y1=q->y1;
	e1.z1=q->z1;
	e1.x2=p->x2;
	e1.y2=p->y2;
	e1.z2=p->z2;

	e2.x1=p->x2;
	e2.y1=p->y2;
	e2.z1=p->z2;
	e2.x2=q->x2;
	e2.y2=q->y2;
	e2.z2=q->z2;
	while (q->next)
	{
		q=q->next;
		if(isIntersect(&e1,q)||isIntersect(&e2,q)) return FALSE;
	}
	return TRUE;
}

byte Triangulation::checkEdge(Edge *ehead)//�ж����һ���߶��Ƿ�����������,��ά���
{
	//���һ���ߵĲ����κα߽��棬���ҳ�����ʼ�㣬ĩ�˲�������֮ǰ���κ�һ������
	bool finish=0;
	Edge *p=ehead->next,*q=ehead;
	while (q->next) q=q->next;//qָ�����һ����
	while (p->next)
	{	
		if (p==ehead->next)//p�ǵ�һ����,����Դ�,�����ཻ
		{
			if((pointOnEdge(p,q)==0||pointOnEdge(p,q)==1)&&(p->x1-q->x2)*(p->x1-q->x2)+(p->y1-q->y2)*(p->y1-q->y2)<0.01) finish=1;//���ܹ��ɷ�գ����ȴ�����
		}
		if(isIntersect(p,q)||pointOnEdge(p,q)) return 0;
		p=p->next;
	}
	if(finish) 
	{
		p=ehead->next;
		q->x2=p->x1;
		q->y2=p->y1;
		q->z2=p->z1;
		return 2;//��ʱ���Թ��ɷ�յĶ������
	}
	return 1;//��ʱ���һ�߷������������������λ�û�з��
}

byte Triangulation::pointOnEdge(Edge *e1,Edge *e2)//�ж�e2��һ�����Ƿ��ڱ�e1�ϣ���ά���
{
	double x1=e1->x2-e1->x1,y1=e1->y2-e1->y1;
	double x2=e2->x2-e1->x1,y2=e2->y2-e1->y1;
	if(x1*y2-y1*x2==0)//��һ������
	{
		double r;
		if(x1!=0) r=x2/x1;
		else r=y2/y1;
		if(0<r&&r<1) return 3; //�ڱ�e1�м�
		if(r==0) return 1; //��e1��һ������
		if(r==1) return 2; //��e1�ڶ�������		
	}
	return 0;//���ڱ���
}

bool  Triangulation::isAnticlockwise(Edge * ehead)//�жϱ��Ƿ�����ʱ�����У���ά���
{
	Edge *p=ehead->next;//ָ���һ����
	double sgn;
	double positive=0,negative=0;
	while (p->next)
	{
		sgn=(p->x2-p->x1)*(p->next->y2-p->next->y1)-(p->y2-p->y1)*(p->next->x2-p->next->x1);
		if (sgn>0) positive++;
		if (sgn<0) negative++;
		p=p->next;
	}
	sgn=(p->x2-p->x1)*(ehead->next->y2-ehead->next->y1)-(ehead->next->x2-ehead->next->x1)*(p->y2-p->y1);
	if (sgn>0) positive++;
	if (sgn<0) negative++;
	if(positive>negative) return TRUE;
	else return FALSE;
}

void Triangulation::changeClockwise(Edge *head) //�ı�ʱ�뷽�򣬶�ά���
{
	double tx,ty;
	Edge *p=head->next,*q;
	Edge *ehead=new Edge;
	ehead->next=NULL;
	while(p)
	{	
		//�Ի���������
		tx=p->x1;
		ty=p->y1;
		p->x1=p->x2;
		p->y1=p->y2;
		p->x2=tx;
		p->y2=ty;
		q=p;
		p=p->next;
		//��q��ָ�����ݲ��뵽ehead������
		q->next=ehead->next;
		ehead->next=q;
	}
	head->next=ehead->next;
	delete ehead;
}

Triangle * Triangulation::copyAnotherSide(Triangle *thead)//���෴ʱ�뷽������������
{
	Triangle *p=thead,*q;
	Triangle *pt=new Triangle;
	q=pt;
	while(p->next)
	{
		p=p->next;
		q->next=new Triangle;
		q=q->next;
		q->pt[0].v[0]=p->pt[0].v[0];
		q->pt[0].v[1]=p->pt[0].v[1];
		q->pt[0].v[2]=p->pt[0].v[2];
		q->pt[1].v[0]=p->pt[2].v[0];
		q->pt[1].v[1]=p->pt[2].v[1];
		q->pt[1].v[2]=p->pt[2].v[2];
		q->pt[2].v[0]=p->pt[1].v[0];
		q->pt[2].v[1]=p->pt[1].v[1];
		q->pt[2].v[2]=p->pt[1].v[2];	
	}
	q->next=NULL;
	return pt;
}

void Triangulation::correctDirection(Edge *head)//�������ı߲�����ʱ�����У���������ʱ������
{
	if (!isAnticlockwise(head)) changeClockwise(head);
}
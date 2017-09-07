// Triangulation.h: interface for the Triangulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIANGULATION_H__4C688FE7_A494_4286_B49C_44071FC650F3__INCLUDED_)
#define AFX_TRIANGULATION_H__4C688FE7_A494_4286_B49C_44071FC650F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class Triangulation  
{
public:
	Triangulation();
	virtual ~Triangulation();
	
	bool isInner(Edge *e1,Edge *e2);//�ж��ܷ񹹳���������
	Triangle * polygonToTriangles(Edge * pe,bool side,GLdouble *pn=NULL);//������и��������
	bool isIntersect(Edge *e1,Edge *e2);//�ж������߶��Ƿ��ཻ
	bool isSeen(Edge *p,Edge *head);//�Ƿ����
	byte checkEdge(Edge *ehead);//�ж����һ���߶��Ƿ�����������
	byte pointOnEdge(Edge *e1,Edge *e2);//�ж�e2��һ�����Ƿ��ڱ�e1��
	bool isAnticlockwise(Edge * pe);//�жϱ��Ƿ�����ʱ������
	void changeClockwise(Edge *ehead);//�ı�ߵ�˳�淽��
	Triangle *copyAnotherSide(Triangle *thead);//���෴ʱ�뷽������������
	void correctDirection(Edge *head);//�������ı߲�����ʱ�����У���������ʱ������

	GLdouble vn[3];
};

#endif // !defined(AFX_TRIANGULATION_H__4C688FE7_A494_4286_B49C_44071FC650F3__INCLUDED_)

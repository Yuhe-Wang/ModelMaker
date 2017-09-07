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
	
	bool isInner(Edge *e1,Edge *e2);//判断能否构成内三角形
	Triangle * polygonToTriangles(Edge * pe,bool side,GLdouble *pn=NULL);//多边形切割成三角形
	bool isIntersect(Edge *e1,Edge *e2);//判断两条线段是否相交
	bool isSeen(Edge *p,Edge *head);//是否可视
	byte checkEdge(Edge *ehead);//判断最后一条线段是否符合添加条件
	byte pointOnEdge(Edge *e1,Edge *e2);//判断e2后一个点是否在边e1上
	bool isAnticlockwise(Edge * pe);//判断边是否是逆时针排列
	void changeClockwise(Edge *ehead);//改变边的顺逆方向
	Triangle *copyAnotherSide(Triangle *thead);//以相反时针方向复制三角形链
	void correctDirection(Edge *head);//如果输入的边不是逆时针排列，将其变成逆时针排列

	GLdouble vn[3];
};

#endif // !defined(AFX_TRIANGULATION_H__4C688FE7_A494_4286_B49C_44071FC650F3__INCLUDED_)

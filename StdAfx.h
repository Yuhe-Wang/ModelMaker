// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__14CA4694_53B1_4814_A84D_6F61660847B7__INCLUDED_)
#define AFX_STDAFX_H__14CA4694_53B1_4814_A84D_6F61660847B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define PI 3.1415926
#define ND_MAX 60
#include "gl/GLUT.H"

typedef struct _Edge
{
	double x1,y1,z1;
	double x2,y2,z2;
	_Edge *next;
}Edge;

typedef struct
{
	double cx,cy,cz,lx,ly,lz;
	int nx,ny,nz;//分段
}Cuboid;

typedef struct
{
	bool type;//哪一种,0正棱锥，1任意边形
	double cx,cy,cz,h;//公用数据
	double n,R;//n边正棱锥 n<=ND_MAX
	byte nr,nh;//分段
	Edge * ehead;//多边形底面边的头指针
	Edge * end;//尾部指针，减少访问用
	int numEdge;//边数
	double axis[3];//旋转轴
	double angle;//旋转角
}Cylinder;

typedef struct
{
	double cx,cy,cz,R;
	int nj,nw;//分段
}Sphere;

typedef struct
{
	double cx,cy,cz,R,r;
	int nj,nw;//分段
}Torus;

typedef struct
{
	double cx,cy,cz,R,r,h;
	int n,nr,nh;//分段
}Pipe;

typedef struct _Vertex//空间点结构
{
	GLdouble v[3];
	struct _VertexRef *vr;
	struct _TriangleRef *tr;
}Vertex;

typedef struct _Triangle//三角形结构
{
	Vertex pt[3];//三个顶点
	GLdouble v1[3],v2[3],v3[3];//三条边的向量
	GLdouble v0[3];//法向量
	bool visit;//是否被访问过，切割时使用
	struct _Triangle *next;
}Triangle;

typedef struct _VertexRef//辅助点结构
{
	GLdouble v[3];//x,y,z坐标
	bool selected;//指示是否被选中，生成点时进行初始化
	bool visit;//是否被访问过，切割时使用
	struct _TriangleRef *nextRef;
	struct _VertexRef *next;
}VertexRef;

typedef struct _TriangleRef//辅助点到三角形的映射节点
{
	struct _Vertex *p;//指向三角形中的顶点
	struct _Triangle *t;//指向三角形
	struct _TriangleRef *nextRef;
}TriangleRef;

#include "ClosedSurface.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__14CA4694_53B1_4814_A84D_6F61660847B7__INCLUDED_)

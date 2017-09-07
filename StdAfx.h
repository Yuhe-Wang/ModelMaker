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
	int nx,ny,nz;//�ֶ�
}Cuboid;

typedef struct
{
	bool type;//��һ��,0����׶��1�������
	double cx,cy,cz,h;//��������
	double n,R;//n������׶ n<=ND_MAX
	byte nr,nh;//�ֶ�
	Edge * ehead;//����ε���ߵ�ͷָ��
	Edge * end;//β��ָ�룬���ٷ�����
	int numEdge;//����
	double axis[3];//��ת��
	double angle;//��ת��
}Cylinder;

typedef struct
{
	double cx,cy,cz,R;
	int nj,nw;//�ֶ�
}Sphere;

typedef struct
{
	double cx,cy,cz,R,r;
	int nj,nw;//�ֶ�
}Torus;

typedef struct
{
	double cx,cy,cz,R,r,h;
	int n,nr,nh;//�ֶ�
}Pipe;

typedef struct _Vertex//�ռ��ṹ
{
	GLdouble v[3];
	struct _VertexRef *vr;
	struct _TriangleRef *tr;
}Vertex;

typedef struct _Triangle//�����νṹ
{
	Vertex pt[3];//��������
	GLdouble v1[3],v2[3],v3[3];//�����ߵ�����
	GLdouble v0[3];//������
	bool visit;//�Ƿ񱻷��ʹ����и�ʱʹ��
	struct _Triangle *next;
}Triangle;

typedef struct _VertexRef//������ṹ
{
	GLdouble v[3];//x,y,z����
	bool selected;//ָʾ�Ƿ�ѡ�У����ɵ�ʱ���г�ʼ��
	bool visit;//�Ƿ񱻷��ʹ����и�ʱʹ��
	struct _TriangleRef *nextRef;
	struct _VertexRef *next;
}VertexRef;

typedef struct _TriangleRef//�����㵽�����ε�ӳ��ڵ�
{
	struct _Vertex *p;//ָ���������еĶ���
	struct _Triangle *t;//ָ��������
	struct _TriangleRef *nextRef;
}TriangleRef;

#include "ClosedSurface.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__14CA4694_53B1_4814_A84D_6F61660847B7__INCLUDED_)

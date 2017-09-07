// Queue.h: interface for the CQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEUE_H__9A9463E4_418C_46C2_8E97_D8E75042AE3D__INCLUDED_)
#define AFX_QUEUE_H__9A9463E4_418C_46C2_8E97_D8E75042AE3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StdAfx.h"

typedef struct tagVLink
{
	VertexRef *vr;//节点指针
	struct tagVLink *next;
}VLink;

class CQueue  
{
public:
	CQueue();
	virtual ~CQueue();
	void enQueue(VertexRef *vr);//入队
	VertexRef *deQueue();//出队
	VLink *vhead;//头节点
	VLink *vend;//指向最后一个指针，便于入队
	unsigned int num;//队列中元素的个数
};

#endif // !defined(AFX_QUEUE_H__9A9463E4_418C_46C2_8E97_D8E75042AE3D__INCLUDED_)

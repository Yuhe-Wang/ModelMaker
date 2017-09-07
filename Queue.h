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
	VertexRef *vr;//�ڵ�ָ��
	struct tagVLink *next;
}VLink;

class CQueue  
{
public:
	CQueue();
	virtual ~CQueue();
	void enQueue(VertexRef *vr);//���
	VertexRef *deQueue();//����
	VLink *vhead;//ͷ�ڵ�
	VLink *vend;//ָ�����һ��ָ�룬�������
	unsigned int num;//������Ԫ�صĸ���
};

#endif // !defined(AFX_QUEUE_H__9A9463E4_418C_46C2_8E97_D8E75042AE3D__INCLUDED_)

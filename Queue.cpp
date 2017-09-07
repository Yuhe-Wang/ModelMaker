// Queue.cpp: implementation of the CQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MM.h"
#include "Queue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueue::CQueue()
{
	vhead=new VLink;
	vhead->next=NULL;
	vend=vhead;
	num=0;
}

CQueue::~CQueue()//�ͷ�������ڴ�
{
	VLink *p=vhead,*q;
	while (p->next)
	{
		q=p;
		p=p->next;
		delete q;
	}
	delete p;
}

void CQueue::enQueue(VertexRef *vr)//���
{
	vend->next=new VLink;
	vend=vend->next;
	vend->next=NULL;
	vend->vr=vr;
	num++;
}

VertexRef * CQueue::deQueue()//����
{
	VLink *p=vhead->next;
	vhead->next=p->next;
	VertexRef *vr=p->vr;
	delete p;
	num--;
	if(num==0) vend=vhead;
	return vr;
}

// Search.cpp: implementation of the CSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MM.h"
#include "Search.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSearch::CSearch()
{
	shead=new SLink;
	shead->next=NULL;//建立头结点
}

CSearch::~CSearch() //释放内存
{
	SLink *p;
	while (shead->next)
	{
		p=shead;
		shead=shead->next;
		delete p;
	}
	delete shead;
}

void CSearch::push(VertexRef *pv)//储存一个索引
{
	//遍历查找是否已存在
	SLink *p=shead;
	bool exist=0;
	while(p->next)
	{
		p=p->next;
		if (p->pv==pv)
		{
			exist=1;
			break;
		}
	}
	if (!exist)//不存在该索引，添加之
	{
		p->next=new SLink;
		p=p->next;
		p->next=NULL;
		p->pv=pv;
	}
}

void CSearch::del(VertexRef *pv)//删除一个索引
{
	//遍历查找是否已存在
	SLink *p=shead,*q;
	bool exist=0;
	while(p->next)
	{
		if (p->next->pv==pv)
		{
			q=p->next;
			p->next=q->next;
			delete q;
			break;
		}
		else p=p->next;	
	}
}
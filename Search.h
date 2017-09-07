// Search.h: interface for the CSearch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCH_H__D2F540B8_A589_48C5_BE68_8F910A3DB230__INCLUDED_)
#define AFX_SEARCH_H__D2F540B8_A589_48C5_BE68_8F910A3DB230__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StdAfx.h"

typedef struct tagSLink
{
	VertexRef *pv;
	struct tagSLink *next;
}SLink;

class CSearch  
{
public:
	CSearch();
	virtual ~CSearch();
	void push(VertexRef *pv);//����һ������
	void del(VertexRef *pv);//ɾ��һ������
	SLink * shead;
};

#endif // !defined(AFX_SEARCH_H__D2F540B8_A589_48C5_BE68_8F910A3DB230__INCLUDED_)

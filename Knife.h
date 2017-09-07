// Knife.h: interface for the Knife class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KNIFE_H__AF1336D0_9C76_4132_B8B9_60888F9B445C__INCLUDED_)
#define AFX_KNIFE_H__AF1336D0_9C76_4132_B8B9_60888F9B445C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Knife  //�и���
{
public:
	Knife();
	virtual ~Knife();
	void init();
	void storeMatrix();//�����ƶ�����ת
	void getRealWorldPoints(CS *cs);//�ɸ����������ȡ��ռ������
	//data
	double dx,dy;//����
	double t[3];//�ƶ���
	double rotateMatrix[16];//��ת����
	double size;//����
	double axis[3];//ת����
	double angle;
	double tx,ty,tz;//��ʱ�ƶ���
	bool mouseWheelSize;//�Ƿ�������ֽ������ŵ���
	double state;//�ƶ���������ת
	//ģ�Ϳռ�ĵ�������
	double cx,cy,cz;
	double rdx,rdy;
	double nx,ny,nz;
};

#endif // !defined(AFX_KNIFE_H__AF1336D0_9C76_4132_B8B9_60888F9B445C__INCLUDED_)

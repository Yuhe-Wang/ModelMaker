// Knife.h: interface for the Knife class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KNIFE_H__AF1336D0_9C76_4132_B8B9_60888F9B445C__INCLUDED_)
#define AFX_KNIFE_H__AF1336D0_9C76_4132_B8B9_60888F9B445C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Knife  //切割面
{
public:
	Knife();
	virtual ~Knife();
	void init();
	void storeMatrix();//储存移动和旋转
	void getRealWorldPoints(CS *cs);//由给定的物体获取其空间的坐标
	//data
	double dx,dy;//长宽
	double t[3];//移动量
	double rotateMatrix[16];//旋转矩阵
	double size;//放缩
	double axis[3];//转动轴
	double angle;
	double tx,ty,tz;//临时移动量
	bool mouseWheelSize;//是否允许滚轮进行缩放刀面
	double state;//移动，还是旋转
	//模型空间的刀面数据
	double cx,cy,cz;
	double rdx,rdy;
	double nx,ny,nz;
};

#endif // !defined(AFX_KNIFE_H__AF1336D0_9C76_4132_B8B9_60888F9B445C__INCLUDED_)

// CS.h: interface for the CS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CS_H__E2697785_4CC1_4B46_9A86_362C734C76C7__INCLUDED_)
#define AFX_CS_H__E2697785_4CC1_4B46_9A86_362C734C76C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "gl\gl.h"
#define ND_MAX 60

class CS  //ClosedSurface的缩写
{
public:
	void vectorNomalize(GLdouble *vector);
	void getNVector(GLdouble *p1,GLdouble *p2,GLdouble *p3,GLdouble *vector);//获取法向量
	void createCuboid(Cuboid &pc);
	void createCylinder(Cylinder &pc);
	void createCone(Cylinder &pc);
	void createSphere(Sphere &pc);
	void createTorus(Torus &pc);
	void createPipe(Pipe &pc);
	CS();
	virtual ~CS();
	void copy(CS *p);//由提供的CS对象得到一个clone
	void createVertexRef();//由三角形链表得到辅助点链表
	void show(short viewManner,double workMode);//绘制模型
	bool loadCS(CString path);//从文件载入曲面模型
	void densify(bool type=TRUE);//加密网格,默认全部加密
	void densify(GLdouble increase);//加密网格，increase代表扩张的距离
	void buildRef(VertexRef *begin,Vertex *v,Triangle *t);//从begin顺序查找v并建立相互映射关系
	void storeMVMatrix();//将移动和旋转放入矩阵储存起来
	void setColor(COLORREF color);
	int getNumOfTriangles();
	void getRange();//查找最大范围
	CString readLine(FILE *fp);

	CS *next;//便于链表访问
	int m_num_vertex,m_num_triangle;
	Triangle *it;//三角形头节点
	VertexRef *iv;//辅助点头节点
	bool m_visible;//曲面是否可见
	bool m_selected;//是否被选中，决定显示 
	bool m_free;//是否允许编辑
	byte m_color[3];//曲面颜色
	CString m_name;//曲面名称
	GLuint m_index;//名称索引，便于选择
	double minX,minY,minZ,maxX,maxY,maxZ;//用于绘制选择框
	GLdouble rminX,rmaxX,rminY,rmaxY,rminZ,rmaxZ;//真实的位置
	GLdouble m_mvMatrix[16];//储存经过旋转后的视图模型矩阵
	GLdouble tx,ty,tz;//当前的移动向量
	GLdouble m_translate[3];//已经储存的平移矢量
	GLdouble m_size;//均匀缩放值
	GLdouble m_rotateAxis[3];//旋转轴
	GLdouble m_angle;//旋转角

protected:

};

#endif // !defined(AFX_CS_H__E2697785_4CC1_4B46_9A86_362C734C76C7__INCLUDED_)

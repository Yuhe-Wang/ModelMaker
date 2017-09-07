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

class CS  //ClosedSurface����д
{
public:
	void vectorNomalize(GLdouble *vector);
	void getNVector(GLdouble *p1,GLdouble *p2,GLdouble *p3,GLdouble *vector);//��ȡ������
	void createCuboid(Cuboid &pc);
	void createCylinder(Cylinder &pc);
	void createCone(Cylinder &pc);
	void createSphere(Sphere &pc);
	void createTorus(Torus &pc);
	void createPipe(Pipe &pc);
	CS();
	virtual ~CS();
	void copy(CS *p);//���ṩ��CS����õ�һ��clone
	void createVertexRef();//������������õ�����������
	void show(short viewManner,double workMode);//����ģ��
	bool loadCS(CString path);//���ļ���������ģ��
	void densify(bool type=TRUE);//��������,Ĭ��ȫ������
	void densify(GLdouble increase);//��������increase�������ŵľ���
	void buildRef(VertexRef *begin,Vertex *v,Triangle *t);//��begin˳�����v�������໥ӳ���ϵ
	void storeMVMatrix();//���ƶ�����ת������󴢴�����
	void setColor(COLORREF color);
	int getNumOfTriangles();
	void getRange();//�������Χ
	CString readLine(FILE *fp);

	CS *next;//�����������
	int m_num_vertex,m_num_triangle;
	Triangle *it;//������ͷ�ڵ�
	VertexRef *iv;//������ͷ�ڵ�
	bool m_visible;//�����Ƿ�ɼ�
	bool m_selected;//�Ƿ�ѡ�У�������ʾ 
	bool m_free;//�Ƿ�����༭
	byte m_color[3];//������ɫ
	CString m_name;//��������
	GLuint m_index;//��������������ѡ��
	double minX,minY,minZ,maxX,maxY,maxZ;//���ڻ���ѡ���
	GLdouble rminX,rmaxX,rminY,rmaxY,rminZ,rmaxZ;//��ʵ��λ��
	GLdouble m_mvMatrix[16];//���澭����ת�����ͼģ�;���
	GLdouble tx,ty,tz;//��ǰ���ƶ�����
	GLdouble m_translate[3];//�Ѿ������ƽ��ʸ��
	GLdouble m_size;//��������ֵ
	GLdouble m_rotateAxis[3];//��ת��
	GLdouble m_angle;//��ת��

protected:

};

#endif // !defined(AFX_CS_H__E2697785_4CC1_4B46_9A86_362C734C76C7__INCLUDED_)

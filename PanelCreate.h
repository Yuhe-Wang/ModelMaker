#if !defined(AFX_PANELCREATE_H__3F60ED4D_33D6_4718_8794_787C8800B86A__INCLUDED_)
#define AFX_PANELCREATE_H__3F60ED4D_33D6_4718_8794_787C8800B86A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelCreate.h : header file
//
#include "Cuboid.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Cone.h"
#include "Torus.h"
#include "Pipe.h"
/////////////////////////////////////////////////////////////////////////////
// CPanelCreate dialog

class CPanelCreate : public CDialog
{
// Construction
public:
	CPanelCreate(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CPanelCreate)
	enum { IDD = IDD_PANEL_CREATE };
	CComboBox	m_createTypes;
	//}}AFX_DATA

	CCuboid m_cuboidPanel;//��������������
	CCylinder m_cylinderPanel;//������������
	CSphere m_spherePanel;//������������
	CCone m_conePanel;//������׶�����
	CTorus m_torusPanel;//����Բ�������
	CPipe m_pipePanel;//������״������
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelCreate)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COLORREF m_currentColor;//��ǰ��ɫ
	bool isEdit(); //�жϵ�ǰ�Ƿ�����������
	
	bool m_panelExist[6];//ָʾ��������Ƿ����
	CRect m_paraArea;
	CDialog *m_pp[7];//����ָ�룬���ڿ�����ʾ
	// Generated message map functions
	//{{AFX_MSG(CPanelCreate)
	virtual BOOL OnInitDialog();
	afx_msg void OnImport();
	afx_msg void OnColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCuboid();
	afx_msg void OnCone();
	afx_msg void OnSphere();
	afx_msg void OnCylinder();
	afx_msg void OnTorus();
	afx_msg void OnPipe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELCREATE_H__3F60ED4D_33D6_4718_8794_787C8800B86A__INCLUDED_)

#if !defined(AFX_CYLINDER_H__B954206B_7D76_4516_B7B3_38D42CD42478__INCLUDED_)
#define AFX_CYLINDER_H__B954206B_7D76_4516_B7B3_38D42CD42478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cylinder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCylinder dialog

class CCylinder : public CDialog
{
// Construction
public:
	CCylinder(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCylinder)
	enum { IDD = IDD_CYLINDER };
	CListBox	m_NR;
	CListBox	m_NH;
	CListBox	m_DN;
	double	m_KBX;
	double	m_KBY;
	double	m_KBZ;
	double	m_paraHeight;
	double	m_paraR;
	double	m_KBR;
	double	m_KBHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCylinder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CCylinder)
	afx_msg void OnKBCreate();
	afx_msg void OnRegularPolygon();
	afx_msg void OnNormalPolygon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CYLINDER_H__B954206B_7D76_4516_B7B3_38D42CD42478__INCLUDED_)

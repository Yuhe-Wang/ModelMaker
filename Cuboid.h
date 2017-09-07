#if !defined(AFX_CUBOID_H__0A74E053_0EEC_4106_964A_5C21414055DA__INCLUDED_)
#define AFX_CUBOID_H__0A74E053_0EEC_4106_964A_5C21414055DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cuboid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCuboid dialog

class CCuboid : public CDialog
{
// Construction
public:
	CCuboid(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCuboid)
	enum { IDD = IDD_CUBOID };
	CListBox	m_nz;
	CListBox	m_ny;
	CListBox	m_nx;
	double	m_KBHeight;
	double	m_KBLength;
	double	m_KBWidth;
	double	m_KBX;
	double	m_KBY;
	double	m_KBZ;
	double	m_height;
	double	m_length;
	double	m_width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCuboid)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	bool isEdit();//判断当前是否允许文本输入
	// Generated message map functions
	//{{AFX_MSG(CCuboid)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnKbCreate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUBOID_H__0A74E053_0EEC_4106_964A_5C21414055DA__INCLUDED_)

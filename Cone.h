#if !defined(AFX_CONE_H__F5A90E6C_873F_4136_A1AC_F08095D54BFF__INCLUDED_)
#define AFX_CONE_H__F5A90E6C_873F_4136_A1AC_F08095D54BFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Cone.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCone dialog

class CCone : public CDialog
{
// Construction
public:
	CCone(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCone)
	enum { IDD = IDD_CONE };
	CListBox	m_NR;
	CListBox	m_NH;
	CListBox	m_DN;
	double	m_KBHeight;
	double	m_KBR;
	double	m_KBX;
	double	m_KBY;
	double	m_KBZ;
	double	m_paraHeight;
	double	m_paraR;
	//}}AFX_DATA

	virtual BOOL OnInitDialog();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCone)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCone)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKBCreate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONE_H__F5A90E6C_873F_4136_A1AC_F08095D54BFF__INCLUDED_)

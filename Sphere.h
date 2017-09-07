#if !defined(AFX_SPHERE_H__7B94DF37_80B0_4D10_966F_81BBE26BA399__INCLUDED_)
#define AFX_SPHERE_H__7B94DF37_80B0_4D10_966F_81BBE26BA399__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Sphere.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSphere dialog

class CSphere : public CDialog
{
// Construction
public:
	CSphere(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSphere)
	enum { IDD = IDD_SPHERE };
	CListBox	m_NW;
	CListBox	m_NJ;
	double	m_KBR;
	double	m_KBX;
	double	m_KBY;
	double	m_KBZ;
	double	m_paraR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSphere)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CSphere)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKBCreate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPHERE_H__7B94DF37_80B0_4D10_966F_81BBE26BA399__INCLUDED_)

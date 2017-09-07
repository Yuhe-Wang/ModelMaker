#if !defined(AFX_TORUS_H__8E1E9FF3_9BE9_463C_A787_9C820313CDE3__INCLUDED_)
#define AFX_TORUS_H__8E1E9FF3_9BE9_463C_A787_9C820313CDE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Torus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTorus dialog

class CTorus : public CDialog
{
// Construction
public:
	CTorus(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTorus)
	enum { IDD = IDD_TORUS };
	CListBox	m_nw;
	CListBox	m_nj;
	double	m_KBr;
	double	m_KBR;
	double	m_KBX;
	double	m_KBY;
	double	m_KBZ;
	double	m_r;
	double	m_R;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTorus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CTorus)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKBCreate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TORUS_H__8E1E9FF3_9BE9_463C_A787_9C820313CDE3__INCLUDED_)

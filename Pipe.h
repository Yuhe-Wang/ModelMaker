#if !defined(AFX_PIPE_H__3EDDC6BD_0A7B_414D_9C94_B754550B454D__INCLUDED_)
#define AFX_PIPE_H__3EDDC6BD_0A7B_414D_9C94_B754550B454D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pipe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPipe dialog

class CPipe : public CDialog
{
// Construction
public:
	CPipe(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPipe)
	enum { IDD = IDD_PIPE };
	CListBox	m_NR;
	CListBox	m_NH;
	CListBox	m_N;
	double	m_KBH;
	double	m_KBr;
	double	m_KBR;
	double	m_KBX;
	double	m_KBY;
	double	m_KBZ;
	double	m_paraH;
	double	m_para_r;
	double	m_para_R;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPipe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CPipe)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKBCreate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIPE_H__3EDDC6BD_0A7B_414D_9C94_B754550B454D__INCLUDED_)

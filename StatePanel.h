#if !defined(AFX_STATEPANEL_H__428968F1_9EDC_4197_AF1B_82F215B9FFA4__INCLUDED_)
#define AFX_STATEPANEL_H__428968F1_9EDC_4197_AF1B_82F215B9FFA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatePanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatePanel dialog

class CStatePanel : public CDialog
{
// Construction
public:
	CStatePanel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatePanel)
	enum { IDD = IDD_PANEL_STATE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatePanel)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatePanel)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeViewstyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATEPANEL_H__428968F1_9EDC_4197_AF1B_82F215B9FFA4__INCLUDED_)

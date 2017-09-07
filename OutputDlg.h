#if !defined(AFX_OUTPUTDLG_H__1E64F6CD_4A77_4489_AE18_D1E50467DE49__INCLUDED_)
#define AFX_OUTPUTDLG_H__1E64F6CD_4A77_4489_AE18_D1E50467DE49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutputDlg dialog

class COutputDlg : public CDialog
{
// Construction
public:
	COutputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COutputDlg)
	enum { IDD = IDD_OUTPUT };
	double	m_ix;
	double	m_iy;
	double	m_length;
	double	m_width;
	double	m_height;
	double	m_size;
	//}}AFX_DATA
	double m_moveX;
	double m_moveY;//����ģ��Ҫ�ƶ�����
	double m_mix;
	double m_miy;//ԭ��ģ�͵�λ��
	double m_mLenght;//ԭ��ģ�͵ĳ���
	double m_mWidth;//ԭ��ģ�͵Ŀ��
	double m_mHeight;//ԭ��ģ�͵ĸ߶�

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COutputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSaveParameters();
	virtual void OnOK();
	afx_msg void OnChangeLength();
	afx_msg void OnChangeWidth();
	afx_msg void OnChangeHeight();
	afx_msg void OnChangeSize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTDLG_H__1E64F6CD_4A77_4489_AE18_D1E50467DE49__INCLUDED_)

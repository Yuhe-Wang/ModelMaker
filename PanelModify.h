#if !defined(AFX_PANELMODIFY_H__4193E258_ECE9_4EBB_ACD3_F13A4DE660FC__INCLUDED_)
#define AFX_PANELMODIFY_H__4193E258_ECE9_4EBB_ACD3_F13A4DE660FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelModify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelModify dialog

class CPanelModify : public CDialog
{
// Construction
public:
	CPanelModify(CWnd* pParent = NULL);   // standard constructor
	bool isEdit();//判断是否有控件处于编辑状态
	void unCheck();//使不选中

	int m_cursorW,m_cursorH;
// Dialog Data
	//{{AFX_DATA(CPanelModify)
	enum { IDD = IDD_PANEL_MODIFY };
	CSliderCtrl	m_moveDistance;
	CSliderCtrl	m_knifeRatio;
	CSliderCtrl	m_sliderWidth;
	CSliderCtrl	m_sliderHeight;
	CButton	m_cursorBox;
	CSliderCtrl	m_pointSize;
	CButton	m_startDrag;
	CButton m_startDissect;
	CButton m_startDensify;
	double	m_nMove;
	BOOL	m_chooseDensify;
	//}}AFX_DATA
	afx_msg void OnStartDrag();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelModify)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CPanelModify)
	afx_msg void OnChangeEditDesity();
	afx_msg void OnPointChoose();
	afx_msg void OnBoxChoose();
	afx_msg void OnReleasedcapturePointSize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCursorBox();
	afx_msg void OnReleasedcaptureCursorWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureCursorHeight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStartDissect();
	afx_msg void OnMove();
	afx_msg void OnRotate();
	afx_msg void OnKnifeSize();
	afx_msg void OnReleasedcaptureKnifeRatio(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStartDensify();
	afx_msg void On3PointsKnife();
	afx_msg void OnCut();
	afx_msg void OnDensify();
	afx_msg void OnDensifyChoosed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELMODIFY_H__4193E258_ECE9_4EBB_ACD3_F13A4DE660FC__INCLUDED_)

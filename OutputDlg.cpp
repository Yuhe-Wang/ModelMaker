// OutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MM.h"
#include "OutputDlg.h"
#include "MMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputDlg dialog


COutputDlg::COutputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COutputDlg)
	m_ix = 10.0;
	m_iy = 10.0;
	m_length = 100.0;
	m_width = 0.0;
	m_height = 0.0;
	m_size = 0.0;
	//}}AFX_DATA_INIT
}

void COutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputDlg)
	DDX_Text(pDX, IDC_IX, m_ix);
	DDX_Text(pDX, IDC_IY, m_iy);
	DDX_Text(pDX, IDC_L, m_length);
	DDX_Text(pDX, IDC_W, m_width);
	DDX_Text(pDX, IDC_H, m_height);
	DDX_Text(pDX, IDC_SIZE, m_size);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutputDlg, CDialog)
	//{{AFX_MSG_MAP(COutputDlg)
	ON_BN_CLICKED(ID_SAVE_PARAMETERS, OnSaveParameters)
	ON_EN_CHANGE(IDC_L, OnChangeLength)
	ON_EN_CHANGE(IDC_W, OnChangeWidth)
	ON_EN_CHANGE(IDC_H, OnChangeHeight)
	ON_EN_CHANGE(IDC_SIZE, OnChangeSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL COutputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_length = 100.0;
	m_size=100/m_mLenght;
	m_width=m_size*m_mWidth;
	m_height=m_size*m_mHeight;
	UpdateData(FALSE);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// COutputDlg message handlers

void COutputDlg::OnSaveParameters() 
{
	//计算模型放缩和移动
	UpdateData(TRUE);
	m_size=m_length/m_mLenght;
	m_moveX=m_ix-m_size*m_mix;
	m_moveY=m_iy-m_size*m_miy;

	COutputDlg::EndDialog(15);
}

void COutputDlg::OnOK() 
{
	//计算模型放缩和移动
	UpdateData(TRUE);
	m_size=m_length/m_mLenght;
	m_moveX=m_ix-m_size*m_mix;
	m_moveY=m_iy-m_size*m_miy;

	CDialog::OnOK();
}

void COutputDlg::OnChangeLength() 
{
	UpdateData(TRUE);
	m_size=m_length/m_mLenght;
	m_width=m_size*m_mWidth;
	m_height=m_size*m_mHeight;
	UpdateData(FALSE);
}

void COutputDlg::OnChangeWidth() 
{	
	UpdateData(TRUE);
	m_size=m_width/m_mWidth;
	m_length=m_size*m_mLenght;
	m_height=m_size*m_mHeight;
	UpdateData(FALSE);	
}

void COutputDlg::OnChangeHeight() 
{
	UpdateData(TRUE);
	m_size=m_height/m_mHeight;
	m_length=m_size*m_mLenght;
	m_width=m_size*m_mWidth;
	UpdateData(FALSE);
}

void COutputDlg::OnChangeSize() 
{
	UpdateData(TRUE);
	m_length=m_size*m_mLenght;
	m_width=m_size*m_mWidth;
	m_height=m_size*m_mHeight;
	UpdateData(FALSE);	
}

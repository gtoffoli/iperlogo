// Usage.cpp : implementation file
//
/* usage.cpp
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "Usage.h"

// #include "Cprot30.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CIperlogoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUsage dialog


CUsage::CUsage(CWnd* pParent /*=NULL*/)
	: CDialog(CUsage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsage)
	m_times = 0;
	m_last = _T("");
	m_total = _T("");
	//}}AFX_DATA_INIT
}


void CUsage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsage)
	DDX_Text(pDX, IDC_EDIT_TIMES, m_times);
	DDX_Text(pDX, IDC_EDIT_LAST, m_last);
	DDX_Text(pDX, IDC_EDIT_TOTAL, m_total);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUsage, CDialog)
	//{{AFX_MSG_MAP(CUsage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsage message handlers

BOOL CUsage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	/*	now you have the opportunity to enlighten the user
		of how much he has been using your great program...*/

	char s[64];
	// unsigned int times, t_hour, t_min, t_sec, l_hour, l_min, l_sec;
	unsigned int times = 0, t_hour = 0, t_min = 0, t_sec = 0, l_hour = 0, l_min = 0, l_sec = 0;
	if(FALSE /*cpGetUStatistics(theApp.m_regPath, &times, &t_hour, &t_min, &t_sec, &l_hour, &l_min, &l_sec)*/)
	{
		/* what happened to the security file??? */
		/* maybe tell the user??? */
//		return someerror;
	} else {
		m_times = times;
		sprintf (s, "%d:%d:%d", t_hour, t_min, t_sec);
		m_total = s;
		sprintf (s, "%d:%d:%d", l_hour, l_min, l_sec);
		m_last = s;
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#if !defined(AFX_MCIFRAME_H__6644F886_10CA_11D2_BE3E_444553540000__INCLUDED_)
#define AFX_MCIFRAME_H__6644F886_10CA_11D2_BE3E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MciFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMciFrame frame

class CMciFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMciFrame)
protected:
	CMciFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMciFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMciFrame();

	// Generated message map functions
	//{{AFX_MSG(CMciFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCIFRAME_H__6644F886_10CA_11D2_BE3E_444553540000__INCLUDED_)

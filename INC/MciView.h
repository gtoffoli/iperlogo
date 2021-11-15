#if !defined(AFX_MCIVIEW_H__6644F887_10CA_11D2_BE3E_444553540000__INCLUDED_)
#define AFX_MCIVIEW_H__6644F887_10CA_11D2_BE3E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MciView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMciView view

class CMciView : public CScrollView
{
protected:
	CMciView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMciView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMciView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMciView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMciView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCIVIEW_H__6644F887_10CA_11D2_BE3E_444553540000__INCLUDED_)

// turtleVp.h : interface of the CTurtleViewport class
//

#if !defined(AFX_TURTLEVP_H__BCD4B56D_5259_11D3_A012_444553540000__INCLUDED_)
#define AFX_TURTLEVP_H__BCD4B56D_5259_11D3_A012_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTurtleViewport : public CODBufferedViewport
{
public:
	CTurtleViewport();

	// @cmember Initializes the viewport.
	virtual void OnInitialUpdate();	// da sample OfficeLayoutVp.h 

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTurtleViewport)
	virtual BOOL CreateController();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTurtleViewport();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURTLEVP_H__BCD4B56D_5259_11D3_A012_444553540000__INCLUDED_)

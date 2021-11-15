// turtlefr.h : header file
//

#include "stdafx.h"		// added

// #define OVs
#define N_OVS_TOOLBARS 7

/////////////////////////////////////////////////////////////////////////////
// CTurtleFrame frame

class CTurtleFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CTurtleFrame)
protected:
	CTurtleFrame();			// protected constructor used by dynamic creation

// Attributes
public:
	HMENU m_hMenu;					// 981123
#ifdef OVs
	CToolBar* m_pToolbar [N_OVS_TOOLBARS];
	BOOL m_toolbarShown [N_OVS_TOOLBARS];
	UINT m_OVsToolbarIDs [N_OVS_TOOLBARS];
#endif // OVs

// Operations
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void SetTitleBar (BOOL setOn);	// 981123
	void SetMenuBar (BOOL setOn);	// 981123
#ifdef OVs
	void OVsMenuCollapse ();
#endif // OVs

// Implementation
protected:
	virtual ~CTurtleFrame();

	// Generated message map functions
	//{{AFX_MSG(CTurtleFrame)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

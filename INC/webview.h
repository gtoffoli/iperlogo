#if !defined(AFX_WEBVIEW_H__B29EC442_8C99_11D2_9A8E_E48102C10E27__INCLUDED_)
#define AFX_WEBVIEW_H__B29EC442_8C99_11D2_9A8E_E48102C10E27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// webview.h : header file
//
/* WEBVIEW.H

990917  aggiunto override di PreCreteWindow per soluzione di problemi rinfresco
990914  override di OnWndMsg per soluzione problemi di focus
*/

/////////////////////////////////////////////////////////////////////////////
// Cwebview html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CWebView : public CHtmlView
{
protected:
	CWebView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWebView)

// html Data
public:
	//{{AFX_DATA(Cwebview)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

	int m_self;					// index of self in editor array
	CWebFrame* m_frame;
	BOOL m_isExecute;

// Operations
public:

	void GetName(char* name);
	void ShowScroll (UINT tipo, BOOL stato);
	unsigned long GetAttr ();
	void SetAttr (node attrList);
	void FileOpen (CString pathName);

// Overrides

//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);	// 990917

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebView)
	public:
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWebView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:	// +
	// Generated message map functions
	//{{AFX_MSG(CWebView)
	afx_msg void OnFileClose();
	afx_msg void OnFileLoad();
	afx_msg void OnFileExit();
	afx_msg void OnNavigateBack();
	afx_msg void OnNavigateHome();
	afx_msg void OnNavigateReload();
	afx_msg void OnNavigateStop();
	afx_msg void OnNavigateForward();
	afx_msg void OnViewFontsSmallest();
	afx_msg void OnViewFontsSmall();
	afx_msg void OnViewFontsMedium();
	afx_msg void OnViewFontsLarge();
	afx_msg void OnViewFontsLargest();
	afx_msg void OnSelectBrowser();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBVIEW_H__B29EC442_8C99_11D2_9A8E_E48102C10E27__INCLUDED_)

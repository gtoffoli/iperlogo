// ilregcod.h : main header file for the ILREGCOD application
//

#if !defined(AFX_ILREGCOD_H__29C48AA6_A307_11D1_BE3E_444553540000__INCLUDED_)
#define AFX_ILREGCOD_H__29C48AA6_A307_11D1_BE3E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIlregcodApp:
// See ilregcod.cpp for the implementation of this class
//

class CIlregcodApp : public CWinApp
{
public:
	CIlregcodApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIlregcodApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIlregcodApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ILREGCOD_H__29C48AA6_A307_11D1_BE3E_444553540000__INCLUDED_)

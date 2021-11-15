/* TURTLEDOC.H

990915	prima versione generata con Class Wizard
		modifiche ispirate a testDoc.h, incluso virtual void DeleteContents() !!
*/
#if !defined(AFX_TURTLEDOC_H__E9279801_5333_11D3_A012_444553540000__INCLUDED_)
#define AFX_TURTLEDOC_H__E9279801_5333_11D3_A012_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TurtleDoc.h : header file
//

#include "turtleMdl.h"	// OVs

/////////////////////////////////////////////////////////////////////////////
// CTurtleDoc document

class CTurtleDoc : public CDocument
{
protected:
	CTurtleDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTurtleDoc)

// Attributes
public:

//> OVs (da User's Guide e sample Test) 
protected:
	// The model of the diagram. The model holds all information about 
	// the diagram, while the viewport actually displays it.
	CTurtleModel m_mdlDiagram;
public:
	// Gets a pointer to the model of the diagram.
	CTurtleModel* GetModel() { return &m_mdlDiagram; }
	// Determines if the diagram has been modified.
	virtual BOOL IsModified();
//<

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTurtleDoc)
	virtual void DeleteContents();
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTurtleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTurtleDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURTLEDOC_H__E9279801_5333_11D3_A012_444553540000__INCLUDED_)

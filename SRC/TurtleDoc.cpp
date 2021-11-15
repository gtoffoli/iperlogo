/* TURTLEDOC.CPP

990915	prima versione generata con Class Wizard
		modifiche ispirate a testDoc.cpp
*/
// TurtleDoc.cpp : implementation file
//

#include "stdafx.h"
#include "iperlogo.h"
#include "turtleMdl.h"
#include "turtleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurtleDoc

IMPLEMENT_DYNCREATE(CTurtleDoc, CDocument)

CTurtleDoc::CTurtleDoc()
{
}

BOOL CTurtleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

//> OVs
void CTurtleDoc::DeleteContents()
{   
	m_mdlDiagram.Initialize();
}
//<

CTurtleDoc::~CTurtleDoc()
{
}


BEGIN_MESSAGE_MAP(CTurtleDoc, CDocument)
	//{{AFX_MSG_MAP(CTurtleDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTurtleDoc diagnostics

#ifdef _DEBUG
void CTurtleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTurtleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTurtleDoc serialization

void CTurtleDoc::Serialize(CArchive& ar)
{
//> OVs
	// Serialize the model.
	m_mdlDiagram.Serialize(ar);
//<
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

//> OVs
BOOL CTurtleDoc::IsModified()
{
	return CDocument::IsModified() || m_mdlDiagram.IsModified();
}
//<

/////////////////////////////////////////////////////////////////////////////
// CTurtleDoc commands

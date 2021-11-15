// iperldoc.cpp : implementation of the CIperlogoDoc class
//

#include "stdafx.h"
#include "iperlogo.h"

#include "iperldoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIperlogoDoc

IMPLEMENT_DYNCREATE(CIperlogoDoc, CDocument)

BEGIN_MESSAGE_MAP(CIperlogoDoc, CDocument)
	//{{AFX_MSG_MAP(CIperlogoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIperlogoDoc construction/destruction

CIperlogoDoc::CIperlogoDoc()
{
	// TODO: add one-time construction code here
}

CIperlogoDoc::~CIperlogoDoc()
{
}

BOOL CIperlogoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIperlogoDoc serialization

void CIperlogoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIperlogoDoc diagnostics

#ifdef _DEBUG
void CIperlogoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIperlogoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIperlogoDoc commands

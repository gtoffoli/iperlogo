
// iperlogo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "iperlogo.h"

#include "mainfrm.h"
#include "iperldoc.h"
#include "iperlvw.h"

#include "turtlefr.h"
#include "turtlevw.h"




void GetNewFrame(CDocTemplate *pTemplate)
{

    CWnd* pOldMain;
    
	ASSERT(pTemplate != NULL);
	ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));

	// salviamo la 
	pOldMain = AfxGetApp()->m_pMainWnd;
	
	AfxGetApp()->m_pMainWnd = 0L;	 
	 
	pTemplate->OpenDocumentFile(NULL);
		// if returns NULL, the user has already been alerted
	//Conservo il ptr alla CTurtleView	
    theApp.m_FinestreTarta[0] = (CTurtleView*) ((CMainFrame*) theApp.m_pMainWnd)->GetActiveView();
    theApp.m_FinestreTarta[0]->GetDocument()->SetTitle("Turtle");
    
	AfxGetApp()->m_pMainWnd = pOldMain;
		
}









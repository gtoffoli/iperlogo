/* ILDDE.CPP
		
   970908 - per compatibilita' VC++5.0: _fstrcpy -> strcpy, _fstrlen -> strlen 
*/

#include "stdafx.h"
#include "ddeml.h"

#include "ildde.h"   

#include "myddemls.h"      


// extern "C"
// void execute (CString s);
	void execute (const char *s);	// 980831


DWORD idLogoDdeInst = 0; // BACO! in realta usa idInst definita più sotto come handle
HSZ hszLogoExec;
HSZ hszLogoName;


/***************************** Private Function ****************************\
* This passes out a standard tab-delimited list of topic names for this
* application.
*
* This support is required for other apps to be able to
* find out about us.  This kind of support should be in every DDE
* application.
*
\***************************************************************************/
HDDEDATA TopicListXfer(
PXFERINFO pXferInfo,
WORD iFmt)
{
    WORD cbAlloc, i;
    LPSTR pszTopicList;
    HDDEDATA hData;

    if (pXferInfo->wType == XTYP_ADVSTART)
        return (HDDEDATA) (TRUE);

    if (pXferInfo->wType != XTYP_REQUEST &&
            pXferInfo->wType != XTYP_ADVREQ)
        return (HDDEDATA) (0);
    /*
     * construct the list of topics we have
     */
    cbAlloc = 0;
    for (i = 0; i < CTOPICS; i++)
        cbAlloc += lstrlen(topicList[i].pszTopic) + 1;  // 1 for tab

    // allocate a data handle big enough for the list.
    hData = DdeCreateDataHandle(idInst, NULL, 0, cbAlloc, pXferInfo->hszItem,
            pXferInfo->wFmt, 0);
    pszTopicList = (LPSTR)DdeAccessData(hData, NULL);
    if (pszTopicList) {
        for (i = 0; i < CTOPICS; i++) {
            strcpy(pszTopicList, topicList[i].pszTopic);
            pszTopicList += strlen(topicList[i].pszTopic);
            *pszTopicList++ = '\t';
        }
        *--pszTopicList = '\0';
        DdeUnaccessData(hData);
        return(hData);
    }
    return(0);
}



/***************************** Private Function ****************************\
* This passes out a standard tab-delimited list of item names for the
* specified topic.
*
* This support is required for other apps to be able to
* find out about us.  This kind of support should be in every DDE
* application.
*
\***************************************************************************/
HDDEDATA ItemListXfer(
PXFERINFO pXferInfo,
WORD iFmt)
{
    WORD cbAlloc, i, iItem, cItems;
    ITEMLIST *pItemList = 0;
    LPSTR pszItemList;
    HDDEDATA hData;

    if (pXferInfo->wType == XTYP_ADVSTART)
        return (HDDEDATA) (TRUE);

    if (pXferInfo->wType != XTYP_REQUEST &&
                pXferInfo->wType != XTYP_ADVREQ)
        return (HDDEDATA) (0);
    /*
     * construct the list of items we support for this topic - this supports
     * more than the minimum standard which would support SysItems only on
     * the system topic.
     */

    // locate the requested topic item table
    for (i = 0; i < CTOPICS; i++) {
        if (pXferInfo->hszTopic == topicList[i].hszTopic) {
            pItemList = topicList[i].pItemList;
            cItems = topicList[i].cItems;
            break;
        }
    }

    if (!pItemList)
        return(0);  // item not found

    cbAlloc = 0;
    for (iItem = 0; iItem < cItems; iItem++)
        cbAlloc += lstrlen(pItemList[iItem].pszItem) + 1; // 1 for tab

    // allocate a data handle big enough for the list.
    hData = DdeCreateDataHandle(idInst, NULL, 0, cbAlloc, pXferInfo->hszItem,
            pXferInfo->wFmt, 0);
    pszItemList = (LPSTR)DdeAccessData(hData, NULL);
    if (pszItemList) {
        for (i = 0; i < cItems; i++) {
            strcpy(pszItemList, pItemList[i].pszItem);
            pszItemList += strlen(pItemList[i].pszItem);
            *pszItemList++ = '\t';
        }
        *--pszItemList = '\0';
        DdeUnaccessData(hData);
        return(hData);
    }
    return(0);
}





/***************************** Private Function ****************************\
* Gives out a 0 terminated array of dde format numbers supported by this app.
*
* This support is required for other apps to be able to
* find out about us.  This kind of support should be in every DDE
* application.
*
\***************************************************************************/
HDDEDATA sysFormatsXfer(
PXFERINFO pXferInfo,
WORD iFmt)
{
    int i, cb;
    LPSTR psz, pszT;
    HDDEDATA hData;

    if (pXferInfo->wType == XTYP_ADVSTART)
        return (HDDEDATA) (TRUE);

    if (pXferInfo->wType != XTYP_REQUEST &&
            pXferInfo->wType != XTYP_ADVREQ)
        return(0);

    for (i = 0, cb = 0; i < CFORMATS; i++)
        cb += strlen(aFormats[i].sz) + 1;

    hData = DdeCreateDataHandle(idInst, NULL, (DWORD)cb,
            0L, pXferInfo->hszItem, pXferInfo->wFmt, 0);
#ifndef WIN32
    psz = pszT = (char _far *) DdeAccessData(hData, NULL);
#else // WIN32
    psz = pszT = (char *) DdeAccessData(hData, NULL);
#endif // WIN32
    for (i = 0; i < CFORMATS; i++) {
        strcpy(pszT, aFormats[i].sz);
        pszT += strlen(pszT);
        *pszT++ = '\t';
    }
    *(--pszT) = '\0';
    DdeUnaccessData(hData);
    return(hData);
}



                      
HDDEDATA TestRandomXfer(PXFERINFO pXferInfo, WORD iFmt)
{                
    return (HDDEDATA) (0);
}

HDDEDATA TestCountXfer(PXFERINFO pXferInfo, WORD iFmt)
{

    return(0);
}

                      
HDDEDATA TestHugeXfer(PXFERINFO pXferInfo,WORD iFmt)
{                                         
    return (HDDEDATA) (0);
}


HDDEDATA HelpXfer(PXFERINFO pXferInfo,WORD iFmt)
{                                     
    return (HDDEDATA) (0);
}

// -----------------------------------------------------------------------------------    
// GSI funzione di utility crea le handles di memoria globale opportune
// -----------------------------------------------------------------------------------    
void Hszize()
{
    register ITEMLIST *pItemList;
    WORD iTopic, iItem;

    hszAppName = DdeCreateStringHandle(idInst, szServer, NULL);

    for (iTopic = 0; iTopic < CTOPICS; iTopic++) {
        topicList[iTopic].hszTopic =
                DdeCreateStringHandle(idInst, topicList[iTopic].pszTopic, NULL);
        pItemList = topicList[iTopic].pItemList;
        for (iItem = 0; iItem < topicList[iTopic].cItems; iItem++) {
            pItemList[iItem].hszItem =
                    DdeCreateStringHandle(idInst, pItemList[iItem].pszItem, NULL);
        }
    }
}

// -----------------------------------------------------------------------------------    
// GSI funzione di utility
// -----------------------------------------------------------------------------------    
void UnHszize()
{
    register ITEMLIST *pItemList;
    WORD iTopic, iItem;

    DdeFreeStringHandle(idInst, hszAppName);

    for (iTopic = 0; iTopic < CTOPICS; iTopic++) {
        DdeFreeStringHandle(idInst, topicList[iTopic].hszTopic);
        pItemList = topicList[iTopic].pItemList;
        for (iItem = 0; iItem < topicList[iTopic].cItems; iItem++) {
            DdeFreeStringHandle(idInst, pItemList[iItem].hszItem);
        }
    }
}


// -----------------------------------------------------------------------------------    
// GSI aggiungere queste variabili
// -----------------------------------------------------------------------------------    
DWORD idInst = 0;
CONVCONTEXT CCFilter = { sizeof(CONVCONTEXT), 0, 0, 0, 0L, 0L };
HANDLE hInst;                       /* current instance                      */
HWND hwndServer;
RECT rcRand;
RECT rcCount;
RECT rcComment;
RECT rcExec;
RECT rcConnCount;
RECT rcRndrDelay;
RECT rcRunaway;
RECT rcAllBlock;
RECT rcNextAction;
RECT rcHugeSize;
RECT rcAppowned;
RECT rcTopics;
BOOL fAllBlocked = FALSE;
BOOL fAllEnabled = TRUE;
BOOL fEnableOneCB = FALSE;
BOOL fBlockNextCB = FALSE;
BOOL fTermNextCB = FALSE;
BOOL fAppowned = FALSE;
WORD cRunaway = 0;
WORD RenderDelay = 0;
DWORD count = 0;
WORD seed = 0;
HSZ hszAppName = NULL;
char szClass[] = "ServerWClass";
char szTopic[MAX_TOPIC] = "Test";
char szServer[MAX_TOPIC] = "IperLogo";
char szComment[MAX_COMMENT];
char szExec[MAX_EXEC] = "";
char *pszComment;
WORD cyText;
WORD cServers = 0;
HDDEDATA hDataHelp[CFORMATS] = {0};
HDDEDATA hDataCount[CFORMATS] = {0};
HDDEDATA hDataRand[CFORMATS] = {0};
HDDEDATA hDataHuge[CFORMATS] = {0};
DWORD cbHuge = 0;

FORMATINFO aFormats[CFORMATS] = {
    { 0, "TEXT" },      // exception!  predefined format, note:
                        // we use the standard imposed by EXCEL of not including
                        // the "CF_" and use all UPPERCASE.
    { 0, "Dummy1"  },
    { 0, "Dummy2"  },
};


/*
 *          Topic and Item tables supported by this application.
 */

/*   HSZ    PROCEDURE       PSZ        */

ITEMLIST SystemTopicItemList[CSYSTEMITEMS] = {

    { 0, TopicListXfer,  SZDDESYS_ITEM_TOPICS   },
    { 0, ItemListXfer,   SZDDESYS_ITEM_SYSITEMS },
    { 0, sysFormatsXfer, SZDDESYS_ITEM_FORMATS  },
    { 0, HelpXfer,       SZDDESYS_ITEM_HELP},
  };


ITEMLIST TestTopicItemList[CTESTITEMS] = {

    { 0, TestRandomXfer, "Rand" },   // 0 index
    { 0, TestCountXfer,  "Count"},   // 1 index
    { 0, TestHugeXfer,   "Huge" },   // 2 index
    { 0, ItemListXfer,   SZDDESYS_ITEM_SYSITEMS },  // 3 index
  };


/* The system topic is always assumed to be first. */
/*   HSZ   PROCEDURE            #ofITEMS        PSZ     */
TOPICLIST topicList[CTOPICS] = {

    { 0, SystemTopicItemList,   CSYSTEMITEMS,   SZDDESYS_TOPIC},    // 0 index
    { 0, TestTopicItemList,     CTESTITEMS,     szTopic},           // 1 index
  };


// -----------------------------------------------------------------------------------    
// fine GSI --------------------------------------------------------------------------
// -----------------------------------------------------------------------------------    

/*
 * This function verifies that the incomming conversation context fits the
 * server's context filter's requirements.
 */
BOOL ValidateContext(
PCONVCONTEXT pCC)
{
    // make sure our CCFilter allows it...mock security, language support
    // old DDE app client case...pCC == NULL
    if (pCC == NULL &&
            CCFilter.dwSecurity == 0 &&      // were nonsecure
            CCFilter.iCodePage == CP_WINANSI // were normal cp
            ) {
        return(TRUE);
    }

    if (pCC &&
            pCC->wFlags == CCFilter.wFlags && // no special flags needed
            pCC->iCodePage == CCFilter.iCodePage && // codepages match
            pCC->dwSecurity == CCFilter.dwSecurity) { // security passes
        // dont care about language and country.
        return(TRUE);
    }
    return(FALSE);  // disallow no match
}



HDDEDATA EXPENTRY DdeCallback(
WORD wType,
WORD wFmt,
HCONV hConv,
HSZ hszTopic,
HSZ hszItem,
HDDEDATA hData,
DWORD lData1,
DWORD lData2)
{
    WORD i, j;
    // register ITEMLIST *pItemList;
    ITEMLIST *pItemList;
    WORD cItems, iFmt;
    HDDEDATA hDataRet;

    /*
     * Block this callback if its blockable and we are supposed to.
     */
    if (fBlockNextCB && !(wType & XTYPF_NOBLOCK)) {
        fBlockNextCB = FALSE;
        fAllEnabled = FALSE;
        return((HDDEDATA) CBR_BLOCK);
    }

    /*
     * Block this callback if its associated with a conversation and we
     * are supposed to.
     */
    if (fTermNextCB && hConv) {
        fTermNextCB = FALSE;
        DdeDisconnect(hConv);
        wType = XTYP_DISCONNECT;
    }

    /*
     * Keep a count of connections
     */
    if (wType == XTYP_CONNECT_CONFIRM) {
        cServers++;
        InvalidateRect(hwndServer, &rcConnCount, TRUE);
        return(0);
    }
    if (wType == XTYP_DISCONNECT) {
        cServers--;
        InvalidateRect(hwndServer, &rcConnCount, TRUE);
        return(0);
    }


    /*
     * only allow transactions on the formats we support if they have a format.
     */
    if (wFmt) {
        for (iFmt = 0; iFmt < CFORMATS; iFmt++) {
            if (wFmt == aFormats[iFmt].atom)
                break;
        }
        if (iFmt == CFORMATS)
            return(0);          // illegal format - ignore now.
    }

    /*
     * Executes are allowed only on the system topic.  This is a general
     * convention, not a requirement.
     *
     * Any executes received result in the execute text being shown in
     * the server client area.  No real action is taken.
     */
     // GSI al posto di scrivere le executes ne diamo una message
    if (wType == XTYP_EXECUTE) {
        if (hszTopic == topicList[0].hszTopic) {    // must be on system topic
            // Format is assumed to be CF_TEXT.
            DdeGetData(hData, (LPBYTE)szExec, MAX_EXEC, 0);
            execute(szExec);
            // GSI szExec[MAX_EXEC - 1] = '\0';
            // GSI InvalidateRect(hwndServer, &rcExec, TRUE);
            // GSI hDataRet = (HDDEDATA) TRUE;     
            // CMainFrame* pFrame = (CMainFrame*) theApp.m_pMainWnd;
            // CMy_ddeView* pView = (CMy_ddeView*) pFrame->GetActiveView();
            // pView->m_EditValue = szExec; 
            // pView->UpdateData(FALSE);
            goto ReturnSpot;
        }
        pszComment = "Execute received on non-system topic - ignored";
        // GSI 30-07-96 questo a naso e' codice GDI del' esempio e serviva farlo ridisegnare a video
        // InvalidateRect(hwndServer, &rcComment, TRUE);
        return(0);
    }

    /*
     * Process wild initiates here
     */
    if (wType == XTYP_WILDCONNECT) {
        HSZ ahsz[(CTOPICS + 1) * 2];
        /*
         * He wants a hsz list of all our available app/topic pairs
         * that conform to hszTopic and hszItem(App).
         */

        if (!ValidateContext((PCONVCONTEXT)lData1)) {
            return(FALSE);
        }

        if (hszItem != hszAppName && hszItem != NULL) {
            // we only support the hszAppName service
            return(0);
        }

        // scan the topic table and create hsz pairs
        j = 0;
        for (i = 0; i < CTOPICS; i++) {
            if (hszTopic == NULL || hszTopic == topicList[i].hszTopic) {
                ahsz[j++] = hszAppName;
                ahsz[j++] = topicList[i].hszTopic;
            }
        }

        // cap off the list with 0s
        ahsz[j++] = ahsz[j++] = 0L;

        // send it back
        return(DdeCreateDataHandle(idInst, (LPBYTE)&ahsz[0], sizeof(HSZ) * j, 0L, 0, wFmt, 0));
    }

    /*
     * Check our hsz tables and send to the apropriate proc. to process.
     * We use DdeCmpStringHandles() which is the portable case-insensitive
     * method of comparing string handles.  (this is a macro on windows so
     * there is no real speed hit.)  On WINDOWS, HSZs are case-insensitive
     * anyway, but this may not be the case on other platforms.
     */
    for (i = 0; i < CTOPICS; i++) {
        if (DdeCmpStringHandles(topicList[i].hszTopic, hszTopic) == 0) {

            /*
             * connections must be on a topic we support.
             */
            if (wType == XTYP_CONNECT) {
                return (HDDEDATA) (ValidateContext((PCONVCONTEXT)lData1));
            }

            pItemList = topicList[i].pItemList;
            cItems = topicList[i].cItems;
            for (j = 0; j < cItems; j++) {
                if (DdeCmpStringHandles(pItemList[j].hszItem, hszItem) == 0) {

                    /*
                     * Make call to worker function here...
                     */
                    hDataRet = (*pItemList[j].npfnCallback)
                            ((PXFERINFO)&lData2, iFmt);


ReturnSpot:
                    /*
                     * The table functions return a boolean or data.
                     * It gets translated here.
                     */
                    switch (wType & XCLASS_MASK) {
                    case XCLASS_DATA:
                        return(hDataRet);
                        break;
                    case XCLASS_FLAGS:
                        return((HDDEDATA) (hDataRet ? DDE_FACK : DDE_FNOTPROCESSED));
                        break;
                    case XCLASS_BOOL:
                        return((HDDEDATA) TRUE);
                    default: // XCLASS_NOTIFICATION
                        return(0);
                        break;
                    }
                    break;
                }
            }
            break;
        }
    }

    /*
     * anything else fails - DDEML is designed so that a 0 return is ALWAYS ok.
     */
    return(0);
}


BOOL CreateDdeServer ()
{     
  //---------------------------------------------------------------------------------------
  // GSI Inizializzazione server
  // --------------------------------------------------------------------------------------
    int i;

    /* Save the instance handle in static variable, which will be used in  */
    /* many subsequence calls from this application to Windows.            */
    hInst = AfxGetInstanceHandle();


    aFormats[0].atom = CF_TEXT; // exception - predefined.
    for (i = 1; i < CFORMATS; i++) {
        aFormats[i].atom = RegisterClipboardFormat(aFormats[i].sz);
    }

    /* Make the window visible; update its client area; and return "success" */
    CCFilter.iCodePage = CP_WINANSI;   // initial default codepage
    if (!DdeInitialize(&idInst, (PFNCALLBACK)MakeProcInstance((FARPROC)DdeCallback,
                AfxGetInstanceHandle()), APPCMD_FILTERINITS, NULL)) {
        Hszize();
        DdeNameService(idInst, hszAppName, NULL, DNS_REGISTER);
        return(TRUE);
    }
    return (FALSE);

}



void DestroyDdeServer ()
{
	HSZ  hszLogoExec = NULL;        
	UINT DDEError;
	
    /* Unegister the service name. */

    if (!DdeNameService(idInst, hszAppName, (HSZ) NULL, DNS_UNREGISTER)) {
      DDEError = DdeGetLastError(idInst);                                 
	  TRACE ("Failing to uninitialize Logo DDE Server");
    }  

	/* Uninitialize the application for the DDEML. */
	
	if (! DdeUninitialize(idInst)) {
	  DDEError = DdeGetLastError(idInst);
	  TRACE ("Failing to uninitialize Logo DDE Server");
	}
}
                      

#ifdef MONNEZZA_ESEMPIO


char szDdeHelp[] = "DDEML test server help:\r\n\n"\
    "The 'Server'(service) and 'Test'(topic) names may change.\r\n\n"\
    "Items supported under the 'Test' topic are:\r\n"\
    "\tCount:\tThis value increments on each data change.\r\n"\
    "\tRand:\tThis value is randomly generated each data change.\r\n"\
    "\tHuge:\tThis is randomlly generated text data >64k that the\r\n"\
    "\t\tDDEML test client can verify.\r\n"\
    "The above items change after any request if in Runaway mode and \r\n"\
    "can bo POKEed in order to change their values.  POKEed Huge data \r\n"\
    "must be in a special format to verify the correctness of the data \r\n"\
    "or it will not be accepted.\r\n"\
    "If the server is set to use app owned data handles, all data sent \r\n"\
    "uses HDATA_APPOWNED data handles."\
    ;




#endif

                      

#ifdef DOC

DdeNameService (3.1)

#include <ddeml.h>

HDDEDATA DdeNameService(idInst, hsz1, hszRes, afCmd)

DWORD idInst;	/* instance identifier	*/
HSZ hsz1;	/* handle of service-name string	*/
HSZ hszRes;	/* reserved	*/
UINT afCmd;	/* service-name flags	*/


The DdeNameService function registers or unregisters the service names that a dynamic data exchange (DDE) server supports. 
This function causes the system to send XTYP_REGISTER or XTYP_UNREGISTER transactions to other running DDE 
Management Library (DDEML) client applications. 
A server application should call this function to register each service name that it supports and to unregister names that it previously 
registered but no longer supports. A server should also call this function to unregister its service names just before terminating. 

Parameter	Description

idInst	Specifies the application-instance identifier obtained by a previous call to the DdeInitialize function. 
hsz1	Identifies the string that specifies the service name that the server is registering or unregistering. An application that 
is unregistering all of its service names should set this parameter to NULL. 
hszRes	Reserved; should be set to NULL. 
afCmd	Specifies the service-name flags. This parameter can be one of the following values: 

Value	Meaning

DNS_REGISTER	Registers the given service name. 
DNS_UNREGISTER	Unregisters the given service name. If the hsz1 parameter is NULL, all service names 
registered by the server will be unregistered. 
DNS_FILTERON	Turns on service-name initiation filtering. This filter prevents a server from receiving 
XTYP_CONNECT transactions for service names that it has not registered. This is the 
default setting for this filter. 

If a server application does not register any service names, the application cannot receive 
XTYP_WILDCONNECT transactions. 

DNS_FILTEROFF	Turns off service-name initiation filtering. If this flag is set, the server will receive an 
XTYP_CONNECT transaction whenever another DDE application calls the 
DdeConnect function, regardless of the service name. 

Returns

The return value is nonzero if the function is successful. Otherwise, it is zero. 

Errors

Use the DdeGetLastError function to retrieve the error value, which may be one of the following: 


DMLERR_DLL_NOT_INITIALIZED 
DMLERR_DLL_USAGE 
DMLERR_INVALIDPARAMETER 
DMLERR_NO_ERROR 

Comments

The service name identified by the hsz1 parameter should be a base name (that is, the name should contain no instance-specific 
information). The system generates an instance-specific name and sends it along with the base name during the XTYP_REGISTER 
and XTYP_UNREGISTER transactions. The receiving applications can then connect to the specific application instance. 

Example

The following example initializes an application with the DDEML, creates frequently used string handles, and registers the 
application's service name: 



HSZ hszClock;
HSZ hszTime;
HSZ hszNow;
HINSTANCE hinst;
DWORD idInst = 0L;
FARPROC lpDdeProc;

/* Initialize the application for the DDEML. */

lpDdeProc = MakeProcInstance((FARPROC) DdeCallback, hinst);
if (!DdeInitialize((LPDWORD) &idInst, (PFNCALLBACK) lpDdeProc,
        APPCMD_FILTERINITS | CBF_FAIL_EXECUTES, 0L)) {

    /* Create frequently used string handles. */

    hszTime = DdeCreateStringHandle(idInst, "Time", CP_WINANSI);

    hszNow = DdeCreateStringHandle(idInst, "Now", CP_WINANSI);
    hszClock = DdeCreateStringHandle(idInst, "Clock", CP_WINANSI);

    /* Register the service name. */

    DdeNameService(idInst, hszClock, (HSZ) NULL, DNS_REGISTER);

}

See Also

DdeConnect, DdeConnectList, DdeInitialize, XTYP_REGISTER, XTYP_UNREGISTER 

==========

DdeInitialize (3.1)

#include <ddeml.h>

UINT DdeInitialize(lpidInst, pfnCallback, afCmd, uRes)

DWORD FAR* lpidInst;	/* address of instance identifier	*/
PFNCALLBACK pfnCallback;	/* address of callback function	*/
DWORD afCmd;	/* array of command and filter flags	*/
DWORD uRes;	/* reserved	*/


The DdeInitialize function registers an application with the Dynamic Data Exchange Management Library (DDEML). An application 
must call this function before calling any other DDEML function. 

Parameter	Description

lpidInst	Points to the application-instance identifier. At initialization, this parameter should point to 0L. If the function is 
successful, this parameter points to the instance identifier for the application. This value should be passed as the 
idInst parameter in all other DDEML functions that require it. If an application uses multiple instances of the 
DDEML dynamic link library, the application should provide a different callback function for each instance. 

If lpidInst points to a nonzero value, this implies a reinitialization of the DDEML. In this case, lpidInst must point to a 
valid application-instance identifier. 

pfnCallback	Points to the application-defined DDE callback function. This function processes DDE transactions sent by the 
system. For more information, see the description of the DdeCallback callback function. 
afCmd	Specifies an array of APPCMD_ and CBF_ flags. The APPCMD_ flags provide special instructions to the 
DdeInitialize function. The CBF_ flags set filters that prevent specific types of transactions from reaching the 
callback function. Using these flags enhances the performance of a DDE application by eliminating unnecessary 
calls to the callback function. 

This parameter can be a combination of the following flags: 

Flag	Meaning

APPCLASS_MONITOR	Makes it possible for the application to monitor DDE activity in the 
system. This flag is for use by DDE monitoring applications. The 
application specifies the types of DDE activity to monitor by combining 
one or more monitor flags with the APPCLASS_MONITOR flag. For 
details, see the following Comments section. 
APPCLASS_STANDARD	Registers the application as a standard (nonmonitoring) DDEML 
application. 
APPCMD_CLIENTONLY	Prevents the application from becoming a server in a DDE 
conversation. The application can be only a client. This flag reduces 
resource consumption by the DDEML. It includes the functionality of the 
CBF_FAIL_ALLSVRXACTIONS flag. 

APPCMD_FILTERINITS	Prevents the DDEML from sending XTYP_CONNECT and 
XTYP_WILDCONNECT transactions to the application until the 
application has created its string handles and registered its service 
names or has turned off filtering by a subsequent call to the 
DdeNameService or DdeInitialize function. This flag is always in 
effect when an application calls DdeInitialize for the first time, 
regardless of whether the application specifies this flag. On subsequent 
calls to DdeInitialize, not specifying this flag turns off the application's 
service-name filters; specifying this flag turns on the application's 
service-name filters. 
CBF_FAIL_ALLSVRXACTIONS	Prevents the callback function from receiving server transactions. The 
system will return DDE_FNOTPROCESSED to each client that sends a 
transaction to this application. This flag is equivalent to combining all 
CBF_FAIL_ flags. 

CBF_FAIL_ADVISES	Prevents the callback function from receiving XTYP_ADVSTART and 
XTYP_ADVSTOP transactions. The system will return 
DDE_FNOTPROCESSED to each client that sends an 
XTYP_ADVSTART or XTYP_ADVSTOP transaction to the server. 
CBF_FAIL_CONNECTIONS	Prevents the callback function from receiving XTYP_CONNECT and 
XTYP_WILDCONNECT transactions. 
CBF_FAIL_EXECUTES	Prevents the callback function from receiving XTYP_EXECUTE 
transactions. The system will return DDE_FNOTPROCESSED to a 
client that sends an XTYP_EXECUTE transaction to the server. 
CBF_FAIL_POKES	Prevents the callback function from receiving XTYP_POKE 
transactions. The system will return DDE_FNOTPROCESSED to a 
client that sends an XTYP_POKE transaction to the server. 
CBF_FAIL_REQUESTS	Prevents the callback function from receiving XTYP_REQUEST 
transactions. The system will return DDE_FNOTPROCESSED to a 
client that sends an XTYP_REQUEST transaction to the server. 

CBF_FAIL_SELFCONNECTIONS	Prevents the callback function from receiving XTYP_CONNECT 
transactions from the application's own instance. This prevents an 
application from establishing a DDE conversation with its own instance. 
An application should use this flag if it needs to communicate with other 
instances of itself but not with itself. 
CBF_SKIP_ALLNOTIFICATIONS	Prevents the callback function from receiving any notifications. This flag 
is equivalent combining all CBF_SKIP_ flags. 
CBF_SKIP_CONNECT_CONFIRMS	Prevents the callback function from receiving 
XTYP_CONNECT_CONFIRM notifications. 
CBF_SKIP_DISCONNECTS	Prevents the callback function from receiving XTYP_DISCONNECT 
notifications. 
CBF_SKIP_REGISTRATIONS	Prevents the callback function from receiving XTYP_REGISTER 
notifications. 
CBF_SKIP_UNREGISTRATIONS	Prevents the callback function from receiving XTYP_UNREGISTER 
notifications. 

uRes	Reserved; must be set to 0L. 

Returns

The return value is one of the following: 


DMLERR_DLL_USAGE 
DMLERR_INVALIDPARAMETER 
DMLERR_NO_ERROR 
DMLERR_SYS_ERROR 

Comments

An application that uses multiple instances of the DDEML must not pass DDEML objects between instances. 

A DDE monitoring application should not attempt to perform DDE (establish conversations, issue transactions, and so on) within the 
context of the same application instance. 
A synchronous transaction will fail with a DMLERR_REENTRANCY error if any instance of the same task has a synchronous 
transaction already in progress. 
A DDE monitoring application can combine one or more of the following monitor flags with the APPCLASS_MONITOR flag to specify 
the types of DDE activity to monitor: 

Flag	Meaning

MF_CALLBACKS	Notifies the callback function whenever a transaction is sent to any DDE callback function in the system. 
MF_CONV	Notifies the callback function whenever a conversation is established or terminated. 
MF_ERRORS	Notifies the callback function whenever a DDE error occurs. 
MF_HSZ_INFO	Notifies the callback function whenever a DDE application creates, frees, or increments the use count of a 
string handle or whenever a string handle is freed as a result of a call to the DdeUninitialize function. 
MF_LINKS	Notifies the callback function whenever an advise loop is started or ended. 

MF_POSTMSGS	Notifies the callback function whenever the system or an application posts a DDE message. 
MF_SENDMSGS	Notifies the callback function whenever the system or an application sends a DDE message. 

Example

The following example obtains a procedure-instance address for a DDE callback function, then initializes the application with the 
DDEML. 



DWORD idInst = 0L;
FARPROC lpDdeProc;

lpDdeProc = MakeProcInstance((FARPROC) DDECallback, hInst);
if (DdeInitialize((LPDWORD) &idInst, (PFNCALLBACK) lpDdeProc,
        APPCMD_CLIENTONLY, 0L))
    return FALSE;

See Also

DdeClientTransaction, DdeConnect, DdeCreateDataHandle, DdeEnableCallback, DdeNameService, DdePostAdvise, 
DdeUninitialize 

==========

DdeCallback (3.1)

HDDEDATA CALLBACK DdeCallback(type, fmt, hconv, hsz1, hsz2, hData, dwData1, dwData2)

UINT type;	/* transaction type	*/
UINT fmt;	/* clipboard data format	*/
HCONV hconv;	/* handle of conversation	*/
HSZ hsz1;	/* handle of string	*/
HSZ hsz2;	/* handle of string	*/
HDDEDATA hData;	/* handle of global memory object	*/
DWORD dwData1;	/* transaction-specific data	*/
DWORD dwData2;	/* transaction-specific data	*/


The DdeCallback function is an application-defined dynamic data exchange (DDE) callback function that processes DDE 
transactions sent to the function as a result of DDE Management Library (DDEML) calls by other applications. 

Parameter	Description

type	Specifies the type of the current transaction. This parameter consists of a combination of transaction-class flags and 
transaction-type flags. The following table describes each of the transaction classes and provides a list of the 
transaction types in each class. 

Value	Meaning

XCLASS_BOOL	A DDE callback function should return TRUE or FALSE when it finishes 
processing a transaction that belongs to this class. Following are the 
XCLASS_BOOL transaction types: 


XTYP_ADVSTART 
XTYP_CONNECT 

XCLASS_DATA	A DDE callback function should return a DDE data handle, CBR_BLOCK, or 
NULL when it finishes processing a transaction that belongs to this class. 
Following are the XCLASS_DATA transaction types: 


XTYP_ADVREQ 
XTYP_REQUEST 
XTYP_WILDCONNECT 

XCLASS_FLAGS	A DDE callback function should return DDE_FACK, DDE_FBUSY, or 
DDE_FNOTPROCESSED when it finishes processing a transaction that 
belongs to this class. Following are the XCLASS_FLAGS transaction types: 


XTYP_ADVDATA 
XTYP_EXECUTE 
XTYP_POKE 

XCLASS_NOTIFICATION	The transaction types that belong to this class are for notification purposes only. 
The return value from the callback function is ignored. Following are the 
XCLASS_NOTIFICATION transaction types: 


XTYP_ADVSTOP 
XTYP_CONNECT_CONFIRM 
XTYP_DISCONNECT 
XTYP_ERROR 
XTYP_MONITOR 
XTYP_REGISTER 
XTYP_XACT_COMPLETE 
XTYP_UNREGISTER 

fmt	Specifies the format in which data is to be sent or received. 
hconv	Identifies the conversation associated with the current transaction. 
hsz1	Identifies a string. The meaning of this parameter depends on the type of the current transaction. For more 
information, see the description of the transaction type. 
hsz2	Identifies a string. The meaning of this parameter depends on the type of the current transaction. For more 
information, see the description of the transaction type. 
hData	Identifies DDE data. The meaning of this parameter depends on the type of the current transaction. For more 
information, see the description of the transaction type. 
dwData1	Specifies transaction-specific data. For more information, see the description of the transaction type. 

dwData2	Specifies transaction-specific data. For more information, see the description of the transaction type. 

Returns

The return value depends on the transaction class. 

Comments

The callback function is called asynchronously for transactions that do not involve creating or terminating conversations. An 
application that does not frequently accept incoming messages will have reduced DDE performance because DDEML uses 
messages to initiate transactions. 

An application must register the callback function by specifying its address in a call to the DdeInitialize function. DdeCallback is a 
placeholder for the application- or library-defined function name. The actual name must be exported by including it in an EXPORTS 
statement in the application's module-definition file. 
See Also

DdeEnableCallback, DdeInitialize 

#endif // DOC

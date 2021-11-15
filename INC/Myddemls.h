#include <windows.h>
#include <ddeml.h>
//#include "\il32\resource.h"
#include "resource.h"

#define MAX_APPNAME     64
#define MAX_TOPIC       64
#define MAX_COMMENT     100
#define MAX_EXEC        40
#define CCHARS          80
#define CLINES          40

#define UM_CHGDATA      (WM_USER + 435)

#define CSYSTEMITEMS 4
#define CTESTITEMS  4
#define CTOPICS     2

/****** Structrues ******/

// predefined format list item

typedef struct {
    ATOM atom;
    PSTR sz;
} FORMATINFO;
#define CFORMATS 3

extern FORMATINFO aFormats[];

// this structure depends on the fact that EXPENTRY is defined as
// pascal calling convention.  This allows us to merely point to
// the last parameter of DdeCallback and cast the pointer to this
// structure to save stack space. (a common hack)

typedef struct _XFERINFO {
    DWORD lData2;
    DWORD lData1;
    HDDEDATA hData;
    HSZ hszItem;
    HSZ hszTopic;
    HCONV hConv;
    WORD wFmt;
    WORD wType;
} XFERINFO;
typedef XFERINFO *PXFERINFO;    // DS==SS!

typedef struct _ITEMLIST {
    HSZ hszItem;
    HDDEDATA (*npfnCallback) (PXFERINFO, WORD);
    PSTR pszItem;
} ITEMLIST;

typedef struct _TOPICLIST {
    HSZ hszTopic;
    ITEMLIST *pItemList;
    WORD cItems;
    PSTR pszTopic;
} TOPICLIST;

typedef struct {    // used to passinfo to/from TextDataEntryDlgProc and
    HDDEDATA hDdeData;
    WORD wFmt;
    HSZ hszItem;
} XACT;



// GLOBALS

extern DWORD idInst;
extern CONVCONTEXT CCFilter;
extern HANDLE hInst;
extern HWND hwndServer;
extern RECT rcTopics;
extern RECT rcRand;
extern RECT rcCount;
extern RECT rcComment;
extern RECT rcExec;
extern RECT rcConnCount;
extern RECT rcRndrDelay;
extern RECT rcRunaway;
extern RECT rcAllBlock;
extern RECT rcNextAction;
extern RECT rcHugeSize;
extern RECT rcAppowned;
extern BOOL fAllBlocked;
extern BOOL fAllEnabled;
extern BOOL fEnableOneCB;
extern BOOL fBlockNextCB;
extern BOOL fTermNextCB;
extern BOOL fAppowned;
extern WORD cRunaway;
extern WORD RenderDelay;
extern DWORD count;
extern HSZ hszAppName;
extern char szClass[];
extern char szTopic[MAX_TOPIC];
extern char szServer[MAX_TOPIC];
extern char szComment[MAX_COMMENT];
extern char szExec[MAX_EXEC];
extern char *pszComment;
extern WORD seed;
extern WORD cyText;
extern WORD cServers;
extern HDDEDATA hDataHelp[CFORMATS];
extern HDDEDATA hDataCount[CFORMATS];
extern HDDEDATA hDataRand[CFORMATS];
extern HDDEDATA hDataHuge[CFORMATS];
extern DWORD cbHuge;

extern char szDdeHelp[];
extern FORMATINFO aFormats[CFORMATS];
extern ITEMLIST SystemTopicItemList[CSYSTEMITEMS];
extern ITEMLIST TestTopicItemList[CTESTITEMS];
extern TOPICLIST topicList[CTOPICS];


// FUNCTIONS


BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
int FAR DoDialog(LPSTR lpTemplateName, FARPROC lpDlgProc, DWORD param, BOOL fRememberFocus);
long FAR PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL FAR PASCAL About(HWND, unsigned, WORD, LONG);
BOOL FAR PASCAL RenderDelayDlgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL SetTopicDlgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL SetServerDlgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL ContextDlgProc(HWND, WORD, WORD, LONG);
VOID PaintServer(HWND hwnd);
VOID DrawTextLine(HDC hdc, RECT *prcClip, RECT *prcText, PSTR psz);
#define EXPENTRY __stdcall // 17/10/21-GT added 
HDDEDATA EXPENTRY DdeCallback(WORD wType, WORD wFmt, HCONV hConv, HSZ hszTopic,
        HSZ hszItem, HDDEDATA hData, DWORD lData1, DWORD lData2);
HDDEDATA TopicListXfer(PXFERINFO pXferInfo, WORD iFmt);
HDDEDATA ItemListXfer(PXFERINFO pXferInfo, WORD iFmt);
HDDEDATA sysFormatsXfer(PXFERINFO pXferInfo, WORD iFmt);
HDDEDATA TestRandomXfer(PXFERINFO pXferInfo, WORD iFmt);
HDDEDATA TestCountXfer(PXFERINFO pXferInfo, WORD iFmt);
HDDEDATA TestHugeXfer(PXFERINFO pXferInfo, WORD iFmt);
HDDEDATA HelpXfer(PXFERINFO pXferInfo, WORD iFmt);
void Hszize(void);
void UnHszize(void);
void Delay(DWORD delay, BOOL fModal);



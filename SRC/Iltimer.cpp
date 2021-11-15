// iltimer.cpp : Defines the class behaviors for the application.
//

/*
	980831	eliminato extern "C" per conversione a C++ dei moduli C
	980709	Aggiunto ClearAllTimers
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "ildde.h"

#include "mainfrm.h"

#include "ilcpp.h"

extern CIperlogoApp theApp;

#define MAX_TIMER 31

// extern "C" int TimerId[];
// extern "C" node TimerCallback[];

int TimerId[MAX_TIMER+1];
node TimerCallback[MAX_TIMER+1];

// extern "C"
void ini_timer ()
{
	int i;
	for (i = 0; i <= MAX_TIMER; ++i) {
    	TimerId[i] = 0;
    	TimerCallback[i] = NULLP;
	}
}

// extern "C"
void CALLBACK EXPORT TimerProc(
	HWND hWnd,		//handle of CWnd that called SetTimer
	UINT nMsg,		//WM_TIMER
	UINT nIDEvent,	//timer identification
	DWORD dwTime	//system time
)
{
	if (nMsg == WM_TIMER) {
			NestedExec(TimerCallback[nIDEvent]);
	}
}

int WinSetTimer (int nIDEvent, int nElapse)
{
	CMainFrame* mainFrame = (CMainFrame*) theApp.m_pMainWnd;
	return mainFrame->SetTimer(nIDEvent, nElapse, &TimerProc);
}

int WinClearTimer (int nIDEvent)
{
	CMainFrame* mainFrame = (CMainFrame*) theApp.m_pMainWnd;
	return mainFrame->KillTimer(TimerId[nIDEvent]);
}

// extern "C"
void SETTIMER ()
{
	int nId = num_int (args[0]);
	int nDelay = num_int (args[1]);
	node Callback = args[2];
	if ((nId > MAX_TIMER) || (TimerId[nId] != 0)) err2 (5, args[0]);
	else {
		if (TimerId[nId] = WinSetTimer (nId, nDelay))
			TimerCallback[nId] = args[2];
		else err1 (38);	 
	} 
}

// extern "C"
void CLEARTIMER ()
{
	int nId = num_int (args[0]);
	if ((nId > MAX_TIMER) || (TimerId[nId] == 0)) err2 (5, args[0]);
	else if (! WinClearTimer (TimerId[nId])) err1 (38);
	else {
		TimerId[nId] = 0;  
		TimerCallback[nId] = NULLP;
	}
}

void ClearAllTimers ()
{
	int nId;
	for (nId = 0; nId <= MAX_TIMER; ++nId)
		if (TimerId[nId] != 0)
			WinClearTimer (TimerId[nId]);
	ini_timer ();
}

#ifdef DOC

CWnd::SetTimer  

UINT SetTimer( UINT nIDEvent, UINT nElapse, void
(CALLBACK EXPORT* lpfnTimer)(HWND, UINT, UINT, DWORD) );

  

nIDEvent    Specifies a nonzero timer identifier.

nElapse    Specifies the time-out value, in milliseconds.
lpfnTimer    Specifies the address of the application-supplied TimerProc callback function that processes the WM_TIMER messages. 
If this parameter is NULL, the WM_TIMER messages are placed in the application's message queue and handled by the CWnd 
object.

Remarks

Installs a system timer. A time-out value is specified, and every time a time-out occurs, the system posts a WM_TIMER message to the 
installing application's message queue or passes the message to an application-defined TimerProc callback function. 
The lpfnTimer callback function need not be named TimerProc, but it must be defined as follows and return 0.
void CALLBACK EXPORT TimerProc(
	HWND hWnd,		//handle of CWnd that called SetTimer
	UINT nMsg,		//WM_TIMER
	UINT nIDEvent	//timer identification
	DWORD dwTime	//system time
);

Timers are a limited global resource; therefore it is important that an application check the value returned by the SetTimer member 
function to verify that a timer is actually available.

Return Value

The timer identifier of the new timer if the function is successful. An application passes this value to the KillTimer member function to kill 
the timer. Nonzero if successful; otherwise 0.

==========

#endif // DOC

// smid.cpp - sample midi program by Shane Cantrell
//
// This program plays "In the Hall of the Mountain King.rmi" over and over
// again. Notice that since this is a console program, I used 0 for the
// window handle.  In a real application, it should probably be replaced with
// your window's handle.  Another thing to remember is that the destructer in
// my class takes care of closing the device, so don't think that it isn't
// necessary.

/* MIDI.CPP - G. Toffoli

	991116	aggiunto MIDICONTA (PR_MIDIOUTNUMDEVS); controlli su MIDIAPRI (MIDIOPEN)
	990722	aggiunto MidiExit
	990702	aggiunto MIDI
	980831	eliminato extern "C" per conversione a C++ dei moduli C
*/

#include "stdafx.h"
#include "mmsystem.h"
#include "MciMidi.h"

#define MidiCmdSystemExclusive 240

#include "logo.h"
#include "ilcpp.h"

HMIDIOUT hMidiOut = 0;
MIDIOUTCAPS MidiOutCaps;

void PR_MIDIOUTNUMDEVS ()
{
	risultato = int_num (midiOutGetNumDevs());
}

void MIDIOPEN()
{
// static MciMidi music = MciMidi(0, "IHMKing.rmi");
//	pMusic = &music;
//	pMusic->Play();

	UINT deviceID = MIDIMAPPER;
	UINT result;
//	char MidiError[MAX_CHARS];
//	MIDIOUTCAPS MidiOutCaps;

	risultato = NULLP;

	if (hMidiOut) {
//>991116
//		err2 (23, S_VUOTA);	// device MIDI gia' aperto
		err1 (38);			// non riesco a fare MIDIAPRI
//<
	}
//>991116
	else if (midiOutGetNumDevs() < 1)
		{ err1 (15); return; }	// non posso fare MIDIAPRI
//<
	else {
		if (n_argomenti == 0)
			deviceID = MIDIMAPPER;
		else {
			deviceID = rnum_int (args[0]);
			if (deviceID > midiOutGetNumDevs()) {
				err2 (5, arg_1);	// device MIDI invalido
				return;
			}
		}
		result = midiOutGetDevCaps(
			deviceID,		// Identifier of a MIDI output device      
			&MidiOutCaps,	// Address of a MIDIOUTCAPS structure
			sizeof(MidiOutCaps)
		);
		if (result != MMSYSERR_NOERROR) {
//>991116
//			err2 (31, S_VUOTA);	// non riesco ad aprire
			err1 (38);			// non riesco a fare MIDIAPRI
//<
			return;
		}
		result = midiOutOpen(
			&hMidiOut,		// Address of an HMIDIOUT handle
			deviceID,		// Identifier of a MIDI output device      
			NULL,			// Address of a callback function          
			0,				// User data passed to the callback
			0				// Callback flag for opening the device   
		);
		if (result != MMSYSERR_NOERROR) {
//>991116
//			err2 (31, S_VUOTA);	// non riesco ad aprire
			err1 (38);			// non riesco a fare MIDIAPRI
//<
		}
		else {
			risultato = _parse(MidiOutCaps.szPname);
		}
	}
}

void MidiExit ()
{
	if (hMidiOut)
		midiOutClose (hMidiOut);
}

void MIDICLOSE()
{
//	pMusic->Stop();
//	delete(pMusic);

	UINT result;
//	char MidiError[MAX_CHARS];

	if (! hMidiOut) {
		err2 (22, S_VUOTA);
	}
	else {
		result = midiOutClose (hMidiOut);
		hMidiOut = 0;
		if (result != MMSYSERR_NOERROR)
		        err2 (36, S_VUOTA);
	}
}

void MIDIMESSAGE()
{
	UINT result;
	DWORD MidiMsg;
	node tail;
	UINT n;
	UINT status, data1, data2;

//	char MidiError[MAX_CHARS];

	if (! hMidiOut) {
		err2 (22, S_VUOTA);
	}
	else {
		tail = arg_1;
		n = lstlen (tail);
		if (n < 3) {
			err2 (5, arg_1);	// a . non piace . come argomento
			return;
		}
		status = rnum_int (car (tail));
		if (status == MidiCmdSystemExclusive) {
		}
		else
			while (tail != NULLP) {
				MidiMsg = 0;
				status = rnum_int (car (tail)); tail = cdr (tail);
				if (tail == NULLP) {
					err2 (5, arg_1);	// a . non piace . come argomento
					return;
				}
				data1 = rnum_int (car (tail)); tail = cdr (tail);
				if (tail == NULLP) {
					err2 (5, arg_1);	// a . non piace . come argomento
					return;
				}
				data2 = rnum_int (car (tail)); tail = cdr (tail);
				MidiMsg = status + (data1 << 8) + (data2 << 16);
				result = midiOutShortMsg(
					hMidiOut,		// A HMIDIOUT handle  
					MidiMsg			// A packed MIDI message 
				);
				if (result != MMSYSERR_NOERROR)
					err1 (38);		// non riesco a fare ..
			}
	}
}

void PR_MIDI ()
{
	if (hMidiOut)
		risultato = _parse(MidiOutCaps.szPname);
	else
		risultato = NULLP;
}

MciMidi* MidiObject = NULL;

void MIDILOAD(CWnd* pParentWnd)
{
//	MidiObject = &MciMidi (0, "IHMKing.rmi");

	if (MidiObject != NULL)
		return;

	static char BASED_CODE lpszFilter[] =
		"File Midi (*.mid;*.rmi)|*.mid;*.rmi|Tutti i file (*.*)|*.*||";
	LPCSTR lpszFileName = NULL;
	LPCSTR lpszDefExt = NULL;
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
//	CWnd* pParentWnd = m_frame;
	BOOL bOpenFileDialog = TRUE;
	CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);

	char PathName[1024+1];
	FileDialog.m_ofn.lpstrFile = &PathName[0];
	FileDialog.m_ofn.nMaxFile = 1024;

	int result = FileDialog.DoModal();
	if (result == IDOK) {
		POSITION pos = FileDialog.GetStartPosition();
		CString PathName;
		MidiObject = (MciMidi*) -1L;
		while (pos != NULL) {
			if (MidiObject == NULL)
				return;
			PathName = FileDialog.GetNextPathName(pos);
			MidiObject = new(MciMidi) (0, PathName);
		}
	} else {
		/* file not found */
	}

}

void MIDIPLAY()
{
	if (MidiObject != NULL)
		MidiObject->Play();
}

void MIDIRESET()
{
	if (MidiObject != NULL)
		MidiObject->Reset();
}

void MIDISTOP()
{
	if (MidiObject != NULL) {
		MidiObject->Stop();
		delete(MidiObject);
	}
	MidiObject = NULL;
}

#ifdef ORIGINAL

void main()
{
	DWORD query;                     // Used to determine status of midi

	// Load the music
	MciMidi music(0, "IHMKing.rmi");

	music.Play();                    // Play the music
//	while(!kbhit())                  // Keep playing until a key is hit
	while(TRUE)                      // Keep playing until done
	{
		music.IsDone(query);          // Get the music status
		if (query)                    // If done...
		{
//			music.Reset();             // Reset the position
//			music.Play();              // Begin playing again
			break;
		}
	}
	music.Stop();                    // Stop the music
	}

//	int i = 20; while (i-- > 0) {
//	int j = 100; while (j-- > 0) {
//	int k = 1000; while (k-- > 0)
//  }}

#endif // ORIGINAL

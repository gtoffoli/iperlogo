/* ILMM.C		G. Toffoli

991019	definito IL32
980828	preparazione per conversione a C++
*/

#include "stdafx.h"

#define ILWIN
#define IL32
#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#include "windows.h"
#include <time.h>                   

// HMIDIOUT hMidiOut = 0;

BOOL MyBeep(DWORD dwFreq, DWORD dwDuration)
// DWORD dwFreq;
// DWORD dwDuration;
  {
   unsigned char count_lo;
   unsigned char count_hi;
   WORD count;
   clock_t NumTicksToWait;
   
   count = 1193180L / dwFreq;
   count_lo = LOBYTE(count);
   count_hi = HIBYTE(count);
   
   _asm
      {
      mov al, 0xB6
      out 0x43, al
      mov al, count_lo
      out 0x42, al
      mov al, count_hi
      out 0x42, al
      xor al, al
      in al, 0x61
      or al, 0x03
      out 0x61, al
      }
   
   NumTicksToWait = dwDuration + clock();
   while (NumTicksToWait > clock());
   
   _asm
      {
      xor al, al
      in al, 0x61
      xor al, 0x03
      out 0x61, al
      }
   
   return TRUE;
   }

void  SOUND ()
   {
   long duration;
   long hertz;
   int odd;
   node tail;
   long frequency;
   
   /* open sound and get arg list */

#ifdef IL32   
   OSVERSIONINFO VersionInformation;
   memset(&VersionInformation, 0, sizeof(OSVERSIONINFO));
   VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
#endif // IL32   
   
   /* must be a list that contains something */
   if (arg_1 != NULLP) {
      
      /* count items in list and check that they are pairs */
      tail = arg_1;
      odd=0;
      
		while (tail != NULLP) {
			if (tail != NULLP) tail = cdr (tail);
			if (tail != NULLP) tail = cdr (tail); else odd = 1;
		}
      
      /* if sound creation ok and we have pairs continue */
      
		if (!odd) {
         
         tail = arg_1;
         
         /* fill queue with freq/duration pairs */
         
		while (tail != NULLP) {
			hertz = num_long (car (tail));
			tail = cdr (tail);
            if (tail != NULLP) {
				duration = num_long (car (tail));
            	tail = cdr (tail);
            }
            frequency = hertz;
            if (frequency < 37L) frequency = 37L;
#ifdef IL32   
            GetVersionEx(&VersionInformation);
            if (VersionInformation.dwPlatformId == VER_PLATFORM_WIN32_NT)
               {
               Beep(frequency, duration);
               }
            else
               {
#endif // IL32   
               MyBeep(frequency, duration);
#ifdef IL32   
               }
#endif // IL32   
            }
         /* play it */
         }
      else
         {
//         MainWindowx->CommandWindow->MessageBox("Not paired", "Sound Error");
         }
      }
   else
      {
//      MainWindowx->CommandWindow->MessageBox("Bad argument", "Sound Error");
      }
   
//   return (/*UNBOUND*/ NULLP);
   }
   
#ifdef MM_MIDI

node MIDIOPEN (args)
node  args;
   {
   UINT id;
   UINT MidiError;
   
   MIDIOUTCAPS moc;
   
   node targ;
   node val;
   
   char MidiErrorBuffer[MAX_BUFFER_SIZE];
   
   /* if not open open it */
   if (!hMidiOut)
      {
      
      id = MIDIMAPPER;
      
      if (args != NULLP)
         {
         id = int_arg(args);
         if (id > midiOutGetNumDevs())
            {
//            MainWindowx->CommandWindow->MessageBox("Invalid Midi device", "Midi Error");
            }
         }
      
      MidiError = midiOutGetDevCaps(id, &moc, sizeof(moc));
      
      if (!MidiError) MidiError = midiOutOpen(&hMidiOut, id, NULL, 0L, 0L);
      
      if (MidiError)
         {
//         midiOutGetErrorText(MidiError, MidiErrorBuffer, MAX_BUFFER_SIZE);
//         MainWindowx->CommandWindow->MessageBox(MidiErrorBuffer, "Midi Error");
         }
      else
         {
         targ = make_strnode(moc.szPname, NULL, strlen(moc.szPname), STRING, strnzcpy);
         val = parser(targ, FALSE);
         return(val);
         }
      }
   else
      {
//      MainWindowx->CommandWindow->MessageBox("Already Open", "Midi Error");
      }
   
   return (/*UNBOUND*/ NULLP);
   }

node MIDICLOSE(node /*args*/)
   {
   UINT MidiError;
   char MidiErrorBuffer[MAX_BUFFER_SIZE];
   
   /* if open close it */
   
   if (hMidiOut)
      {
      
      MidiError = midiOutClose(hMidiOut);
      
      hMidiOut = 0;
      
      if (MidiError)
         {
//         midiOutGetErrorText(MidiError, MidiErrorBuffer, MAX_BUFFER_SIZE);
//         MainWindowx->CommandWindow->MessageBox(MidiErrorBuffer, "Midi Error");
         }
      }
   else
      {
//      MainWindowx->CommandWindow->MessageBox("Already closed", "Midi Error");
      }
   
   return (/*UNBOUND*/ NULLP);
   }

node MIDIMESSAGE (node arg)
   {
   node args;
   UINT MidiError;
   char MidiErrorBuffer[MAX_BUFFER_SIZE];
   
   int i;
   int j;
   
   union
      {
      long mylong;
      char mybyte[4];
      }
   bytetolong;
   
   MIDIHDR *MidiOutHdr;
   BYTE *MidiOutData;
   
   HANDLE DataHandle;
   HANDLE HdrHandle;
   
   /* if midi open continue */
   
   if (hMidiOut)
      {
      args = car(arg);
      
      /* if a list with something in it continue */
      
      if (is_list(args) && (args != NULLP))
         {
         
         /* if not system exclusive then use shortmsg else use longmsg */
         
         if (int_arg(args) != 0xF0)
            {
            
            /* pack 3 bytes at a time and send them as short messages */
            
            arg = args;
            
            while (arg != NULLP)
               {
               bytetolong.mylong = 0L;
               bytetolong.mybyte[0] = int_arg(arg);
               if (cdr(arg) != NULLP) bytetolong.mybyte[1] = int_arg(arg=cdr(arg));
               if (cdr(arg) != NULLP) bytetolong.mybyte[2] = int_arg(arg=cdr(arg));
               MidiError = midiOutShortMsg(hMidiOut, bytetolong.mylong);
               if (MidiError) break;
               if (arg != NULLP) arg=cdr(arg);
               }
            
            }
         else
            {
            
            /* count elements in list so we can allocate buffer */
            
            i=0;
            arg = args;
            
            while (arg != NULLP)
               {
               arg=cdr(arg);
               i++;
               }
            
            /* allocate structure buffer */
            
            HdrHandle = (HANDLE)GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE | GMEM_ZEROINIT, sizeof(MIDIHDR));
            MidiOutHdr = (MIDIHDR *)GlobalLock((HGLOBAL)HdrHandle);
            
            DataHandle = (HANDLE)GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE | GMEM_ZEROINIT, i);
            MidiOutData = (BYTE *)GlobalLock((HGLOBAL)DataHandle);
            
            /* pack the buffer array and set size */
            
            arg = args;
            
            for (j=0;j<i;j++)
               {
               MidiOutData[j] = int_arg(arg);
               arg=cdr(arg);
               }
            
            MidiOutHdr->dwBufferLength = i;
            
            MidiOutHdr->lpData = MidiOutData;
            
            /* prepare it, send it out, and unprepare it */
            
            MidiError = midiOutPrepareHeader(hMidiOut, MidiOutHdr, sizeof(MIDIHDR));
            if (!MidiError) MidiError = midiOutLongMsg(hMidiOut, MidiOutHdr, sizeof(MIDIHDR));
            if (!MidiError) MidiError = midiOutUnprepareHeader(hMidiOut, MidiOutHdr, sizeof(MIDIHDR));
            
            /* free buffer and struct */
            
            GlobalUnlock(DataHandle);
            GlobalFree(DataHandle);
            
            GlobalUnlock(HdrHandle);
            GlobalFree(HdrHandle);
            }
         
         /* if midi error let 'em know */
         
         if (MidiError)
            {
//            midiOutGetErrorText(MidiError, MidiErrorBuffer, MAX_BUFFER_SIZE);
//            MainWindowx->CommandWindow->MessageBox(MidiErrorBuffer, "Midi Error");
            }
         }
      else
         {
//         MainWindowx->CommandWindow->MessageBox("Bad argument", "Midi Error");
         }
      }
   else
      {
//      MainWindowx->CommandWindow->MessageBox("Not Open", "Midi Error");
      }
   
   return (/*UNBOUND*/ NULLP);
   }

node MCI (args)
node args;
{
   node targ;
   node val;
   
   DWORD MciError;
   
   char textbuf[MAX_BUFFER_SIZE];
   char MciReturnBuffer[MAX_BUFFER_SIZE];
   char MciErrorBuffer[MAX_BUFFER_SIZE];
   char callback[MAX_BUFFER_SIZE];
   
   /* get mci command string */
   
   cnv_strnode_string(textbuf,args);
   
   /* check for optional callback routine */
   
   if (cdr(args) != NULLP)
      {
      cnv_strnode_string(callback, cdr(args));
      strcpy(mci_callback,callback);
      }
   
   MciReturnBuffer[0] = '\0';
   
   /* send out command */
   
   MciError = mciSendString(textbuf, MciReturnBuffer, MAX_BUFFER_SIZE, MainHWindow);
   
   /* if error let user know else continue */
   
   if (MciError)
      {
//      mciGetErrorString(MciError, MciErrorBuffer, MAX_BUFFER_SIZE);
//      MainWindowx->CommandWindow->MessageBox(MciErrorBuffer, "MCI Error");
      }
   else
      {
      
      /* if something was returned then return it to user */
      
      if (strlen(MciReturnBuffer))
         {
         targ = make_strnode(MciReturnBuffer, NULL, strlen(MciReturnBuffer), STRING, strnzcpy);
         val = parser(targ, FALSE);
         return(val);
         }
      }
   
   return (/*UNBOUND*/ NULLP);
   }

node SETTIMER (args)
node args;
   {
   char callback[MAX_BUFFER_SIZE];
   WORD delay;
   int id;
   
   // get id and if valid continue
   
   id = int_arg(args);
   
   if ((id > 0) && (id < 32))
      {
      
      // get delay callback
      
      delay = int_arg(args=cdr(args));
      
      cnv_strnode_string(callback, args=cdr(args));
      
      if (timer_callback[id] == NULL) timer_callback[id] = (char *)malloc(MAX_BUFFER_SIZE);
      strcpy(timer_callback[id],callback);
      
      // if not set sucessfully error
      
      if (!::SetTimer(MainHWindow, id, delay, NULL))
         {
         MainWindowx->CommandWindow->MessageBox("Too Many Timers", "Error");
         }
      }
   else
      {
      MainWindowx->CommandWindow->MessageBox("Bad Timer Id", "Error");
      }
   return (/*UNBOUND*/ NULLP);
   }

node CLEARTIMER (node args)
   {
   int id;
   
   // get args
   
   id = int_arg(args);
   
   // if timer was not set let user know
   
   if (!::KillTimer(MainHWindow, id))
   MainWindowx->CommandWindow->MessageBox("Timer not found", "Error");
   
   return (/*UNBOUND*/ NULLP);
   }

node PLAYWAVE (args)
node args;
   {
   char name[MAX_BUFFER_SIZE];
   int flag;
   
   cnv_strnode_string(name, args);
   flag = getint(pos_int_arg(args=cdr(args)));
   
   if (strlen(name) == 0)
      {
      sndPlaySound(NULL, flag);
      }
   else
      {
      sndPlaySound(name, flag);
      }
   
   return (/*UNBOUND*/ NULLP);
   }
   
#endif // MM_MIDI


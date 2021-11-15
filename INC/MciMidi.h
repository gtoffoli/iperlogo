//
// Copyright 1995-97 by Shane Cantrell
// All Rights Reserved.
//

#ifndef _MIDI_H
#define _MIDI_H

// #include <windows.h>
// #include <mmsystem.h>

const char *GetError(WORD error);

class MciMidi
   {
   private:
      UINT wDeviceID;
      DWORD dwError;
      HANDLE hWndNotify;

      DWORD dwPosition;
      DWORD dwLength;

   public:
      MciMidi(HANDLE hWnd, const char *filename);
      ~MciMidi();
      char const *GetErrorString();
      int Play();          // Plays the MIDI file
      int Pause();         // Pauses the playing
      int Stop();          // Stops the playing, another MIDI device can be used
      void Reset();        // Resets the position
      int IsDone(DWORD &query);     // Returns 1 if finished playing
      int GetPosition(DWORD &dwPos);// Gets the current playing location
   };

#endif

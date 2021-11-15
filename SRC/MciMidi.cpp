//
// Copyright 1995-97 by Shane Cantrell
// All Rights Reserved.
//

#include <string.h>
#include "stdafx.h"
#include "mmsystem.h"
#include "MciMidi.h"

const char *GetError(WORD error)
   {
   switch(error)
      {
      case MCIERR_BAD_TIME_FORMAT :
         return("The specified value for the time format is invalid.");
      case MCIERR_CANNOT_LOAD_DRIVER :
         return("The specified device driver will not load properly");
      case MCIERR_CANNOT_USE_ALL :
         return("The device name \"all\" is not allowed for this command.");
      case MCIERR_CREATEWINDOW :
         return("Could not create or use window.");
      case MCIERR_DEVICE_LENGTH :
         return("The device or driver name is too long.");
      case MCIERR_DEVICE_LOCKED :
         return("The device is now being closed.  Wait a few seconds.");
      case MCIERR_DEVICE_NOT_INSTALLED :
         return("The specified device is not installed on the system.");
      case MCIERR_DEVICE_NOT_READY :
         return("The device driver is not ready.");
      case MCIERR_DEVICE_OPEN :
         return("The device name is already being used as an alias.");
      case MCIERR_DEVICE_ORD_LENGTH :
         return("The device or driver name is too long.");
      case MCIERR_DEVICE_TYPE_REQUIRED :
         return("The specified device cannot be found on the system.");
      case MCIERR_DRIVER :
         return("The device driver exhibits a problem.");
      case MCIERR_DRIVER_INTERNAL :
         return("The device driver exhibits a problem. (internal)");
      case MCIERR_DUPLICATE_ALIAS :
         return("The specified alias is already used in this application.");
      case MCIERR_EXTENSION_NOT_FOUND :
         return("The specified extension has no device type associated with it.");
      case MCIERR_EXTRA_CHARACTERS :
         return("You must enclose a string with quotation marks.");
      case MCIERR_FILE_NOT_FOUND :
         return("The requested file was not found.");
      case MCIERR_FILE_NOT_SAVED :
         return("The file was not saved.");
      case MCIERR_FILE_READ :
         return("A read from the file failed.");
      case MCIERR_FILE_WRITE :
         return("A write to the file failed.");
      case MCIERR_FILENAME_REQUIRED :
         return("The filename is invalid.");
      case MCIERR_FLAGS_NOT_COMPATIBLE :
         return("The specified parameters cannot be used together.");
      case MCIERR_GET_CD :
         return("The requested file OR MCI device was not found.");
      case MCIERR_HARDWARE :
         return("The specified device exhibits a problem.");
      case MCIERR_ILLEGAL_FOR_AUTO_OPEN :
         return("MCI will not perform the specified command on an automatically opened device.");
      case MCIERR_INTERNAL :
         return("A problem occurred in initializing.");
      case MCIERR_INVALID_DEVICE_ID :
         return("Invalid device ID.");
      case MCIERR_INVALID_DEVICE_NAME :
         return("The specified device is not open nor recognized by MCI.");
      case MCIERR_INVALID_FILE :
         return("The specified file cannot be played on the specified MCI device.");
      case MCIERR_MISSING_PARAMETER :
         return("The specified command requires a missing parameter.");
      case MCIERR_MULTIPLE :
         return("Errors occurred in more than one device.");
      case MCIERR_MUST_USE_SHAREABLE :
         return("The device driver is already in use.  Specify \"shareable\".");
      case MCIERR_NO_ELEMENT_ALLOWED :
         return("The specified device does not use a filename.");
      case MCIERR_NO_INTEGER :
         return("The parameter for this MCI command must be an integer value.");
      case MCIERR_NO_WINDOW :
         return("There is no display window.");
      case MCIERR_NONAPPLICABLE_FUNCTION :
         return("The specified MCI command sequence cannot be performed in the given order.");
      case MCIERR_NULL_PARAMETER_BLOCK :
         return("A null parameter block (structure) was passed to MCI.");
      case MCIERR_OUT_OF_MEMORY :
         return("Your system does not have enough memory for this task.");
      case MCIERR_OUTOFRANGE :
         return("The specified parameter value is out of rage for the specified MCI command.");
      case MCIERR_SET_CD :
         return("The application cannot change directories");
      case MCIERR_SET_DRIVE :
         return("The application cannot change drives.");
      case MCIERR_UNNAMED_RESOURCE :
         return("You cannot store an unnamed file.");
      case MCIERR_UNRECOGNIZED_COMMAND :
         return("The driver cannot recognize the specified command.");
      case MCIERR_UNSUPPORTED_FUNCTION :
         return("The MCI device driver the system is using does not support the command.");
      }
   return("Unknown error code.");
   }

MciMidi::MciMidi(HANDLE hWnd, const char *filename)
   {
   MCI_OPEN_PARMS mciOpenParms;                 // Used to store parameters
   MCI_STATUS_PARMS mciStatusParams;            //

   wDeviceID = 0;                               // Initialize the variables
   dwPosition = 0;                              //
   hWndNotify = hWnd;                           // Remember this window

   mciOpenParms.lpstrDeviceType = "sequencer";  // Use the sequencer device
   mciOpenParms.lpstrElementName = filename;    // Select the file's name

      // Open the midi file, load it in for playing
   dwError = mciSendCommand(NULL,
                            MCI_OPEN,
                            MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
                            (DWORD)(LPVOID)&mciOpenParms);

   if (!dwError)                                // If successful
      {
      wDeviceID = mciOpenParms.wDeviceID;       // Get the device's ID
      }

   mciStatusParams.dwItem = MCI_STATUS_LENGTH;  // Command to get music length

      // Get the length of the music
   mciSendCommand(wDeviceID,
                  MCI_STATUS,
                  MCI_STATUS_ITEM,
                  (DWORD)(LPVOID) &mciStatusParams);
   dwLength = mciStatusParams.dwReturn;         // Save the music length
   }

MciMidi::~MciMidi()
   {
   mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);  // Close the device
   }

const char *MciMidi::GetErrorString()
   {
   return(GetError(LOWORD(dwError)));           // Get the error string
   }

int MciMidi::Play()
   {
   MCI_PLAY_PARMS mciPlayParms;                 // Used to store MCI parameters

   mciPlayParms.dwCallback = (DWORD)hWndNotify; // Set window to notify
   mciPlayParms.dwFrom = dwPosition;            // Set position to start at

      // Send PLAY command
   dwError = mciSendCommand(wDeviceID,
                            MCI_PLAY,
                            MCI_NOTIFY | MCI_FROM,
                            (DWORD)(LPVOID) &mciPlayParms);

   if (dwError)                                 // If there is an error
      {                                         //   close device
      mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
      }

   return(dwError);                             // Return the error code
   }

int MciMidi::Pause()
   {
   MCI_GENERIC_PARMS mciGenericParms;     // Used to store MCI parameters

      // Set the window to notify of the change
   mciGenericParms.dwCallback = (DWORD)hWndNotify;

      // Send the stop command
   dwError = mciSendCommand(wDeviceID,
                            MCI_STOP,
                            MCI_NOTIFY,
                            (DWORD)(LPVOID) &mciGenericParms);

   GetPosition(dwPosition);               // Save the current position
   return(dwError);                       // Return any errors
   }

int MciMidi::Stop()
   {
   MCI_GENERIC_PARMS mciGenericParms;     // Used to store MCI parameters

      // Set the window to notify of the change
   mciGenericParms.dwCallback = (DWORD)hWndNotify;

      // Send the stop command
   dwError = mciSendCommand(wDeviceID,
                            MCI_STOP,
                            MCI_NOTIFY,
                            (DWORD)(LPVOID) &mciGenericParms);

   GetPosition(dwPosition);               // Save the current position
   return(dwError);                       // Return any errors
   }

void MciMidi::Reset()
   {
   dwPosition = 0;                     // Reset the playing location
   }

int MciMidi::IsDone(DWORD &query)
   {
   dwError = GetPosition(query);       // Get the current location

   if (query >= dwLength) query = 1;   // Return 1 if done
   else query = 0;                     // Return 0 if still playing

   return(dwError);                    // Return any error codes
   }

int MciMidi::GetPosition(DWORD &dwPos)
   {
   MCI_STATUS_PARMS mciStatusParams;            // Used to store return values

   mciStatusParams.dwItem = MCI_STATUS_POSITION;// Set the status request

      // Send the query to the device driver
   dwError = mciSendCommand(wDeviceID,
                            MCI_STATUS,
                            MCI_STATUS_ITEM,
                            (DWORD)(LPVOID) &mciStatusParams);

   dwPos = mciStatusParams.dwReturn;            // Make dwPos the return value
   return(dwError);                             // Return any errors
   }

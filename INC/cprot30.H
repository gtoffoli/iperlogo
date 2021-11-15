/************************************************************
               Include file cProtect

               ©   Thomas Lichtneckert 1995, 1996, 1997
               email:  tl@royal.net
************************************************************/

#ifndef _CPROTECT_
#define _CPROTECT_

#include <windows.h>

#define IS_REGISTERED 				0

#define FILE_NOT_FOUND				-10
#define PATH_NOT_VALID				-11
#define NO_PATH						-12
#define NOT_REMOVABLE				-13
#define CFG_FAILED					-14
#define COULD_NOT_WRITE_WININI 	-15
#define COULD_NOT_WRITE_PRIVINI 	-16

#define NOT_REGISTERED				-20
#define NO_REGINFO					-21
#define NO_SERNUMBER					-22
#define FEATURES_NOT_AVAILABLE	-23
#define WRONG_CODE					-24
#define INI_CONTROL_FAILED			-25
#define INI_CONTROL_MISSING		-26
#define COULDNOT_GET_DISKSERNUM	-31
#define DISKSERNUM_FAULT			-32

#define DRIVE_OK						0
#define NO_ARGUMENTS					1
#define DRIVE_NOT_FLOPPY			2
#define DRIVE_NOT_READY				3
#define DRIVE_NOT_VALID				4
#define DRIVE_ERROR					5
#define WRONG_DISKETTE				6
#define DRIVE_WRPROT					7
#define DRIVE_WRPROT_A				8
#define DRIVE_WRPROT_B				9
#define DISK_ISIN_A					10
#define DISK_ISIN_B					11

#define DLLSPEC __declspec(dllimport) __cdecl

#if __cplusplus
extern "C" {
#endif
	/* cpMakeRegCode should be commented out when using cprt332u.dll */
//	int DLLSPEC cpMakeRegCode( int features, char * serialnum, char * regcode );
	int DLLSPEC cpMakeRegCode( int features, const char * serialnum, char * regcode );

	int DLLSPEC cpCFGMake( const char * path, unsigned long magic, unsigned long filever );
	int DLLSPEC cpGetSernum( const char * path, char * serialnum );
	int DLLSPEC cpRegisterProgram( const char * path, const char * regcode );
	int DLLSPEC cpCheckRegCode( const char * path, char * regcode );
	int DLLSPEC cpGetRegisteredName( const char * path, char * namn );
	int DLLSPEC cpGetRegisteredCompany( const char * path, char * namn);
	int DLLSPEC cpSetRegisteredNames( const char * path, const char * namn, const char * firma );
	int DLLSPEC cpIsRegistered( const char * path );
	int DLLSPEC cpGetUParams( const char * path, unsigned long *paraml, int *param1, int *param2, int *param3, int *param4, unsigned int *param5 );
	int DLLSPEC cpSetUParams( const char * path, unsigned long paraml, int param1, int param2, int param3, int param4, unsigned int param5 );
	int DLLSPEC cpSetFTime( const char * path, unsigned long datum, unsigned int tid );
	int DLLSPEC cpStartProgram( const char * path );
	int DLLSPEC cpEndProgram( const char * path );
	int DLLSPEC cpGetUStatistics( const char * path, unsigned int * antggr, unsigned int *stim, unsigned int *smin, unsigned int *ssek, unsigned int *atim, unsigned int *amin, unsigned int *asek );
	long DLLSPEC cpGetFileinfo( const char * path, long * version );
	long DLLSPEC cpDaysSinceInstallation( const char * path );
	int DLLSPEC cpMakeIniNote( const char * path, char* section, char* entry, char* filename );

#if __cplusplus
}
#endif
#endif	/* _CPROTECT_ */

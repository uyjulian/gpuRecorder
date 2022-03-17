/***************************************************************************
      File Name:	Externals.h
   File Created:	Thursday, October 9th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
#ifndef _EXTERNALS_H_
#define _EXTERNALS_H_
#include "stdafx.h"

extern char*			(CALLBACK *REDIRECT_PSEgetLibName)(void);
extern unsigned long	(CALLBACK *REDIRECT_PSEgetLibType)(void);
extern unsigned long	(CALLBACK *REDIRECT_PSEgetLibVersion)(void);	
extern long				(CALLBACK *REDIRECT_SPUtest)(void);
extern long				(CALLBACK *REDIRECT_SPUconfigure)(void);
extern void				(CALLBACK *REDIRECT_SPUabout)(void);
extern long				(CALLBACK *REDIRECT_SPUinit)(void);
extern long				(CALLBACK *REDIRECT_SPUshutdown)(void);
extern long				(CALLBACK *REDIRECT_SPUopen)(HWND);
extern long				(CALLBACK *REDIRECT_SPUclose)(void);
extern unsigned short	(CALLBACK *REDIRECT_SPUreadRegister)(unsigned long);
extern void				(CALLBACK *REDIRECT_SPUwriteRegister)(unsigned long, unsigned short);
extern unsigned short	(CALLBACK *REDIRECT_SPUreadDMA)(void);
extern void				(CALLBACK *REDIRECT_SPUwriteDMA)(unsigned short);
extern void				(CALLBACK *REDIRECT_SPUreadDMAMem)(unsigned short*, int);
extern void				(CALLBACK *REDIRECT_SPUwriteDMAMem)(unsigned short*, int);
extern void				(CALLBACK *REDIRECT_SPUregisterCallback)(void (CALLBACK *callback)(void));
extern void				(CALLBACK *REDIRECT_SPUregisterCDDAVolume)(void (CALLBACK *CDDAVcallback)(unsigned short, unsigned short));
extern long				(CALLBACK *REDIRECT_SPUfreeze)(unsigned long, void*);
extern void				(CALLBACK *REDIRECT_SPUplayADPCMchannel)(void*);
extern unsigned short	(CALLBACK *REDIRECT_SPUgetOne)(unsigned long);
extern void				(CALLBACK *REDIRECT_SPUputOne)(unsigned long, unsigned short);
extern void				(CALLBACK *REDIRECT_SPUplaySample)(unsigned char);
extern void				(CALLBACK *REDIRECT_SPUsetAddr)(unsigned char, unsigned short);
extern void				(CALLBACK *REDIRECT_SPUsetPitch)(unsigned char, unsigned short);
extern void				(CALLBACK *REDIRECT_SPUsetVolumeL)(unsigned char, short);
extern void				(CALLBACK *REDIRECT_SPUsetVolumeR)(unsigned char, short);
extern void				(CALLBACK *REDIRECT_SPUstartChannels1)(unsigned short);
extern void				(CALLBACK *REDIRECT_SPUstartChannels2)(unsigned short);
extern void				(CALLBACK *REDIRECT_SPUstopChannels1)(unsigned short);
extern void				(CALLBACK *REDIRECT_SPUstopChannels2)(unsigned short);
extern void				(CALLBACK *REDIRECT_SPUplaySector)(unsigned long, unsigned char*);

// Function from gpuRecorder.dll
extern void (CALLBACK *RecordSound)(unsigned char, unsigned long, void*);

BOOL LoadPlugins();
void FreePlugins();

#endif
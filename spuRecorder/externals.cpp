/***************************************************************************
      File Name:	Externals.cpp
   File Created:	Thursday, October 9th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
#include "config.h"
#include "externals.h"
#include "reg.h"

char*			(CALLBACK *REDIRECT_PSEgetLibName)(void)						= NULL;
unsigned long	(CALLBACK *REDIRECT_PSEgetLibType)(void)						= NULL;
unsigned long	(CALLBACK *REDIRECT_PSEgetLibVersion)(void)						= NULL;
long			(CALLBACK *REDIRECT_SPUtest)(void)								= NULL;
long			(CALLBACK *REDIRECT_SPUconfigure)(void)							= NULL;
void			(CALLBACK *REDIRECT_SPUabout)(void)								= NULL;
long			(CALLBACK *REDIRECT_SPUinit)(void)								= NULL;
long			(CALLBACK *REDIRECT_SPUshutdown)(void)							= NULL;
long			(CALLBACK *REDIRECT_SPUopen)(HWND)								= NULL;
long			(CALLBACK *REDIRECT_SPUclose)(void)								= NULL;
unsigned short	(CALLBACK *REDIRECT_SPUreadRegister)(unsigned long) 			= NULL;
void			(CALLBACK *REDIRECT_SPUwriteRegister)(unsigned long, unsigned short)	 	= NULL;
unsigned short	(CALLBACK *REDIRECT_SPUreadDMA)(void)										= NULL;
void			(CALLBACK *REDIRECT_SPUwriteDMA)(unsigned short)							= NULL;
void			(CALLBACK *REDIRECT_SPUreadDMAMem)(unsigned short*, int)					= NULL;
void			(CALLBACK *REDIRECT_SPUwriteDMAMem)(unsigned short*, int)					= NULL;
void			(CALLBACK *REDIRECT_SPUregisterCallback)(void (CALLBACK *callback)(void))	= NULL;
void			(CALLBACK *REDIRECT_SPUregisterCDDAVolume)(void (CALLBACK *CDDAVcallback)(unsigned short, unsigned short)) = NULL;
long			(CALLBACK *REDIRECT_SPUfreeze)(unsigned long, void*)				= NULL;
void			(CALLBACK *REDIRECT_SPUplayADPCMchannel)(void*)						= NULL;

unsigned short	(CALLBACK *REDIRECT_SPUgetOne)(unsigned long)						= NULL;
void			(CALLBACK *REDIRECT_SPUputOne)(unsigned long, unsigned short)		= NULL;
void			(CALLBACK *REDIRECT_SPUplaySample)(unsigned char)					= NULL;
void			(CALLBACK *REDIRECT_SPUsetAddr)(unsigned char, unsigned short)		= NULL;
void			(CALLBACK *REDIRECT_SPUsetPitch)(unsigned char, unsigned short)		= NULL;
void			(CALLBACK *REDIRECT_SPUsetVolumeL)(unsigned char, short)			= NULL;
void			(CALLBACK *REDIRECT_SPUsetVolumeR)(unsigned char, short)			= NULL;
void			(CALLBACK *REDIRECT_SPUstartChannels1)(unsigned short)				= NULL;
void			(CALLBACK *REDIRECT_SPUstartChannels2)(unsigned short)				= NULL;
void			(CALLBACK *REDIRECT_SPUstopChannels1)(unsigned short)				= NULL;
void			(CALLBACK *REDIRECT_SPUstopChannels2)(unsigned short)				= NULL;
void			(CALLBACK *REDIRECT_SPUplaySector)(unsigned long, unsigned char*)	= NULL;

// Function from gpuRecorder.dll
void (CALLBACK *RecordSound)(unsigned char, unsigned long, void*) = NULL;

HMODULE Externals_hModule = NULL;
HMODULE Externals_hGPUModule = NULL;

//==========================================================================
// Load Plugins
//==========================================================================

BOOL LoadPlugins()
{
	FreePlugins();
	if(strlen(cfg.SoundPlugin) == 0) return FALSE;
	Externals_hModule = LoadLibrary(cfg.SoundPlugin);
	if(Externals_hModule == NULL) return FALSE;

	(FARPROC&)REDIRECT_PSEgetLibName			= GetProcAddress(Externals_hModule,"PSEgetLibName");
	(FARPROC&)REDIRECT_PSEgetLibType			= GetProcAddress(Externals_hModule,"PSEgetLibType");
	(FARPROC&)REDIRECT_PSEgetLibVersion			= GetProcAddress(Externals_hModule,"PSEgetLibVersion");

	(FARPROC&)REDIRECT_SPUtest					= GetProcAddress(Externals_hModule, "SPUtest");
	(FARPROC&)REDIRECT_SPUconfigure				= GetProcAddress(Externals_hModule, "SPUconfigure");
	(FARPROC&)REDIRECT_SPUabout					= GetProcAddress(Externals_hModule, "SPUabout");

	(FARPROC&)REDIRECT_SPUinit					= GetProcAddress(Externals_hModule, "SPUinit");
	(FARPROC&)REDIRECT_SPUshutdown				= GetProcAddress(Externals_hModule, "SPUshutdown");
	(FARPROC&)REDIRECT_SPUopen					= GetProcAddress(Externals_hModule, "SPUopen");
	(FARPROC&)REDIRECT_SPUclose					= GetProcAddress(Externals_hModule, "SPUclose");

	(FARPROC&)REDIRECT_SPUreadRegister			= GetProcAddress(Externals_hModule, "SPUreadRegister");
	(FARPROC&)REDIRECT_SPUwriteRegister			= GetProcAddress(Externals_hModule, "SPUwriteRegister");
	(FARPROC&)REDIRECT_SPUreadDMA				= GetProcAddress(Externals_hModule, "SPUreadDMA");
	(FARPROC&)REDIRECT_SPUwriteDMA				= GetProcAddress(Externals_hModule, "SPUwriteDMA");
	(FARPROC&)REDIRECT_SPUreadDMAMem			= GetProcAddress(Externals_hModule, "SPUreadDMAMem");
	(FARPROC&)REDIRECT_SPUwriteDMAMem			= GetProcAddress(Externals_hModule, "SPUwriteDMAMem");
	(FARPROC&)REDIRECT_SPUregisterCallback		= GetProcAddress(Externals_hModule, "SPUregisterCallback");
	(FARPROC&)REDIRECT_SPUregisterCDDAVolume	= GetProcAddress(Externals_hModule, "SPUregisterCDDAVolume");
	(FARPROC&)REDIRECT_SPUfreeze				= GetProcAddress(Externals_hModule, "SPUfreeze");
	(FARPROC&)REDIRECT_SPUplayADPCMchannel		= GetProcAddress(Externals_hModule, "SPUplayADPCMchannel");

	(FARPROC&)REDIRECT_SPUgetOne				= GetProcAddress(Externals_hModule, "SPUgetOne");
	(FARPROC&)REDIRECT_SPUputOne				= GetProcAddress(Externals_hModule, "SPUputOne");
	(FARPROC&)REDIRECT_SPUplaySample			= GetProcAddress(Externals_hModule, "SPUplaySample");
	(FARPROC&)REDIRECT_SPUsetAddr				= GetProcAddress(Externals_hModule, "SPUsetAddr");
	(FARPROC&)REDIRECT_SPUsetPitch				= GetProcAddress(Externals_hModule, "SPUsetPitch");
	(FARPROC&)REDIRECT_SPUsetVolumeL			= GetProcAddress(Externals_hModule, "SPUsetVolumeL");
	(FARPROC&)REDIRECT_SPUsetVolumeR			= GetProcAddress(Externals_hModule, "SPUsetVolumeR");
	(FARPROC&)REDIRECT_SPUstartChannels1		= GetProcAddress(Externals_hModule, "SPUstartChannels1");
	(FARPROC&)REDIRECT_SPUstartChannels2		= GetProcAddress(Externals_hModule, "SPUstartChannels2");
	(FARPROC&)REDIRECT_SPUstopChannels1			= GetProcAddress(Externals_hModule, "SPUstopChannels1");
	(FARPROC&)REDIRECT_SPUstopChannels2			= GetProcAddress(Externals_hModule, "SPUstopChannels2");
	(FARPROC&)REDIRECT_SPUplaySector			= GetProcAddress(Externals_hModule, "SPUplaySector");

	// Try to load gpuRecorder.dll
	Externals_hGPUModule = LoadLibrary("gpuRecorder.dll");
	if(Externals_hGPUModule) {
		(FARPROC&)RecordSound = GetProcAddress(Externals_hGPUModule, "RecordSound");
	}
	return TRUE;
}

//==========================================================================
// Free Plugins
//==========================================================================

void FreePlugins()
{	
	if(Externals_hModule != NULL) {
		FreeLibrary(Externals_hModule);
		Externals_hModule = NULL;
	}

	REDIRECT_PSEgetLibName			= NULL;
	REDIRECT_PSEgetLibType			= NULL;
	REDIRECT_PSEgetLibVersion		= NULL;

	REDIRECT_SPUtest				= NULL;
	REDIRECT_SPUconfigure			= NULL;
	REDIRECT_SPUabout				= NULL;

	REDIRECT_SPUinit				= NULL;
	REDIRECT_SPUshutdown			= NULL;
	REDIRECT_SPUopen				= NULL;
	REDIRECT_SPUclose				= NULL;

	REDIRECT_SPUreadRegister		= NULL;
	REDIRECT_SPUwriteRegister		= NULL;
	REDIRECT_SPUreadDMA				= NULL;
	REDIRECT_SPUwriteDMA			= NULL;
	REDIRECT_SPUreadDMAMem			= NULL;
	REDIRECT_SPUwriteDMAMem			= NULL;
	REDIRECT_SPUregisterCallback	= NULL;
	REDIRECT_SPUregisterCDDAVolume	= NULL;
	REDIRECT_SPUfreeze				= NULL;
	REDIRECT_SPUplayADPCMchannel	= NULL;

	REDIRECT_SPUgetOne				= NULL;
	REDIRECT_SPUputOne				= NULL;
	REDIRECT_SPUplaySample			= NULL;
	REDIRECT_SPUsetAddr				= NULL;
	REDIRECT_SPUsetPitch			= NULL;
	REDIRECT_SPUsetVolumeL			= NULL;
	REDIRECT_SPUsetVolumeR			= NULL;
	REDIRECT_SPUstartChannels1		= NULL;
	REDIRECT_SPUstartChannels2		= NULL;
	REDIRECT_SPUstopChannels1		= NULL;
	REDIRECT_SPUstopChannels2		= NULL;
	REDIRECT_SPUplaySector			= NULL;

	if(Externals_hGPUModule) {
		FreeLibrary(Externals_hGPUModule);
		Externals_hGPUModule = NULL;
	}
	RecordSound = NULL;
}

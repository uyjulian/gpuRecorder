/***************************************************************************
      File Name:	Externals.cpp
   File Created:	Sunday, September 28rd 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
#include "externals.h"
#include "config.h"
#include "reg.h"

char*			(CALLBACK *REDIRECT_PSEgetLibName)(void)						= NULL;
unsigned long	(CALLBACK *REDIRECT_PSEgetLibType)(void)						= NULL;
unsigned long	(CALLBACK *REDIRECT_PSEgetLibVersion)(void)						= NULL;
long			(CALLBACK *REDIRECT_GPUinit)(void)								= NULL;
long			(CALLBACK *REDIRECT_GPUshutdown)(void)							= NULL;
void			(CALLBACK *REDIRECT_GPUmakeSnapshot)(void)						= NULL;
long			(CALLBACK *REDIRECT_GPUopen)(HWND)								= NULL;
long			(CALLBACK *REDIRECT_GPUclose)(void)								= NULL;
void			(CALLBACK *REDIRECT_GPUupdateLace)(void)						= NULL;
unsigned long	(CALLBACK *REDIRECT_GPUreadStatus)(void)						= NULL;
void			(CALLBACK *REDIRECT_GPUwriteStatus)(unsigned long)				= NULL;
unsigned long	(CALLBACK *REDIRECT_GPUreadData)(void)							= NULL;
void			(CALLBACK *REDIRECT_GPUreadDataMem)(unsigned long*,int)			= NULL;
void			(CALLBACK *REDIRECT_GPUwriteData)(unsigned long)				= NULL;
void			(CALLBACK *REDIRECT_GPUwriteDataMem)(unsigned long*,int)		= NULL;
void			(CALLBACK *REDIRECT_GPUsetMode)(unsigned long)					= NULL;
long			(CALLBACK *REDIRECT_GPUgetMode)(void)							= NULL;
long			(CALLBACK *REDIRECT_GPUdmaChain)(unsigned long*,unsigned long)	= NULL;
long			(CALLBACK *REDIRECT_GPUconfigure)(void)							= NULL;
void			(CALLBACK *REDIRECT_GPUabout)(void)								= NULL;
long			(CALLBACK *REDIRECT_GPUtest)(void)								= NULL;
long			(CALLBACK *REDIRECT_GPUfreeze)(unsigned long,void*)				= NULL;
void			(CALLBACK *REDIRECT_GPUdisplayText)(char * pText)				= NULL;
void			(CALLBACK *REDIRECT_GPUdisplayFlags)(unsigned long dwFlags)		= NULL;
void			(CALLBACK *REDIRECT_GPUgetScreenPic)(unsigned char*)			= NULL;
void			(CALLBACK *REDIRECT_GPUshowScreenPic)(unsigned char*)			= NULL;

HMODULE Externals_hModule = NULL;
HWND GPU_HWND = NULL;

//==========================================================================
// Load Plugin
//==========================================================================

BOOL LoadPlugin()
{
	FreePlugin();
	if(strlen(cfg.VideoPlugin) == 0) return FALSE;
	Externals_hModule = LoadLibrary(cfg.VideoPlugin);
	if(Externals_hModule == NULL) return FALSE;

	(FARPROC&)REDIRECT_PSEgetLibName		= GetProcAddress(Externals_hModule,"PSEgetLibName");
	(FARPROC&)REDIRECT_PSEgetLibType		= GetProcAddress(Externals_hModule,"PSEgetLibType");
	(FARPROC&)REDIRECT_PSEgetLibVersion	= GetProcAddress(Externals_hModule,"PSEgetLibVersion");
	
	(FARPROC&)REDIRECT_GPUinit			= GetProcAddress(Externals_hModule,"GPUinit");
	(FARPROC&)REDIRECT_GPUshutdown		= GetProcAddress(Externals_hModule,"GPUshutdown");
	(FARPROC&)REDIRECT_GPUmakeSnapshot	= GetProcAddress(Externals_hModule,"GPUmakeSnapshot");
	(FARPROC&)REDIRECT_GPUopen			= GetProcAddress(Externals_hModule,"GPUopen");
	(FARPROC&)REDIRECT_GPUclose			= GetProcAddress(Externals_hModule,"GPUclose");
	
	(FARPROC&)REDIRECT_GPUupdateLace		= GetProcAddress(Externals_hModule,"GPUupdateLace");
	(FARPROC&)REDIRECT_GPUreadStatus		= GetProcAddress(Externals_hModule,"GPUreadStatus");
	(FARPROC&)REDIRECT_GPUwriteStatus	= GetProcAddress(Externals_hModule,"GPUwriteStatus");
	(FARPROC&)REDIRECT_GPUreadData		= GetProcAddress(Externals_hModule,"GPUreadData");
	(FARPROC&)REDIRECT_GPUreadDataMem	= GetProcAddress(Externals_hModule,"GPUreadDataMem");
	(FARPROC&)REDIRECT_GPUwriteData		= GetProcAddress(Externals_hModule,"GPUwriteData");
	(FARPROC&)REDIRECT_GPUwriteDataMem	= GetProcAddress(Externals_hModule,"GPUwriteDataMem");

	(FARPROC&)REDIRECT_GPUsetMode		= GetProcAddress(Externals_hModule,"GPUsetMode");
	(FARPROC&)REDIRECT_GPUgetMode		= GetProcAddress(Externals_hModule,"GPUgetMode");
	(FARPROC&)REDIRECT_GPUdmaChain		= GetProcAddress(Externals_hModule,"GPUdmaChain");
	
	(FARPROC&)REDIRECT_GPUconfigure		= GetProcAddress(Externals_hModule,"GPUconfigure");
	(FARPROC&)REDIRECT_GPUabout			= GetProcAddress(Externals_hModule,"GPUabout");
	(FARPROC&)REDIRECT_GPUtest			= GetProcAddress(Externals_hModule,"GPUtest");

	(FARPROC&)REDIRECT_GPUfreeze			= GetProcAddress(Externals_hModule,"GPUfreeze");
	(FARPROC&)REDIRECT_GPUdisplayText	= GetProcAddress(Externals_hModule,"GPUdisplayText");
	(FARPROC&)REDIRECT_GPUdisplayFlags	= GetProcAddress(Externals_hModule,"GPUdisplayFlags");
	(FARPROC&)REDIRECT_GPUgetScreenPic	= GetProcAddress(Externals_hModule,"GPUgetScreenPic");
	(FARPROC&)REDIRECT_GPUshowScreenPic	= GetProcAddress(Externals_hModule,"GPUshowScreenPic");

	return TRUE;
}

//==========================================================================
// Free Plugin
//==========================================================================

void FreePlugin()
{
	if(Externals_hModule != NULL) {
		FreeLibrary(Externals_hModule);
		Externals_hModule = NULL;
	}

	REDIRECT_PSEgetLibName		= NULL;
	REDIRECT_PSEgetLibType		= NULL;
	REDIRECT_PSEgetLibVersion	= NULL;
	REDIRECT_GPUinit			= NULL;
	REDIRECT_GPUshutdown		= NULL;
	REDIRECT_GPUmakeSnapshot	= NULL;
	REDIRECT_GPUopen			= NULL;
	REDIRECT_GPUclose			= NULL;

	REDIRECT_GPUupdateLace		= NULL;
	REDIRECT_GPUreadStatus		= NULL;
	REDIRECT_GPUwriteStatus		= NULL;
	REDIRECT_GPUreadData		= NULL;
	REDIRECT_GPUreadDataMem		= NULL;
	REDIRECT_GPUwriteData		= NULL;
	REDIRECT_GPUwriteDataMem	= NULL;

	REDIRECT_GPUsetMode			= NULL;
	REDIRECT_GPUgetMode			= NULL;
	REDIRECT_GPUdmaChain		= NULL;

	REDIRECT_GPUconfigure		= NULL;
	REDIRECT_GPUabout			= NULL;
	REDIRECT_GPUtest			= NULL;

	REDIRECT_GPUfreeze			= NULL;
	REDIRECT_GPUdisplayText		= NULL;
	REDIRECT_GPUdisplayFlags	= NULL;
	REDIRECT_GPUgetScreenPic	= NULL;
	REDIRECT_GPUshowScreenPic	= NULL;

}

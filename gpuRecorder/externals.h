/***************************************************************************
      File Name:	Externals.h
   File Created:	Sunday, September 28rd 2003
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
extern long				(CALLBACK *REDIRECT_GPUinit)(void);
extern long				(CALLBACK *REDIRECT_GPUshutdown)(void);
extern void				(CALLBACK *REDIRECT_GPUmakeSnapshot)(void);
extern long				(CALLBACK *REDIRECT_GPUopen)(HWND);
extern long				(CALLBACK *REDIRECT_GPUclose)(void);
extern void				(CALLBACK *REDIRECT_GPUupdateLace)(void);
extern unsigned long	(CALLBACK *REDIRECT_GPUreadStatus)(void);
extern void				(CALLBACK *REDIRECT_GPUwriteStatus)(unsigned long);
extern unsigned long	(CALLBACK *REDIRECT_GPUreadData)(void);
extern void				(CALLBACK *REDIRECT_GPUreadDataMem)(unsigned long*,int);
extern void				(CALLBACK *REDIRECT_GPUwriteData)(unsigned long);
extern void				(CALLBACK *REDIRECT_GPUwriteDataMem)(unsigned long*,int);
extern void				(CALLBACK *REDIRECT_GPUsetMode)(unsigned long);
extern long				(CALLBACK *REDIRECT_GPUgetMode)(void);
extern long				(CALLBACK *REDIRECT_GPUdmaChain)(unsigned long*,unsigned long);
extern long				(CALLBACK *REDIRECT_GPUconfigure)(void);
extern void				(CALLBACK *REDIRECT_GPUabout)(void);
extern long				(CALLBACK *REDIRECT_GPUtest)(void);
extern long				(CALLBACK *REDIRECT_GPUfreeze)(unsigned long,void*);
extern void				(CALLBACK *REDIRECT_GPUdisplayText)(char * pText);
extern void				(CALLBACK *REDIRECT_GPUdisplayFlags)(unsigned long dwFlags);
extern void				(CALLBACK *REDIRECT_GPUgetScreenPic)(unsigned char*);
extern void				(CALLBACK *REDIRECT_GPUshowScreenPic)(unsigned char*);

extern HWND GPU_HWND;

BOOL LoadPlugin();
void FreePlugin();

#endif
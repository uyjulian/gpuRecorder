/***************************************************************************
      File Name:	About.h
   File Created:	Thursday, September 28th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#ifndef _ABOUT_H_
#define _ABOUT_H_
#include "stdafx.h"

#define APP_BUILD     __DATE__ " , " __TIME__
#define APP_VERSION   "Version 2.0"

BOOL CALLBACK About_DlgProc(HWND hW, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif

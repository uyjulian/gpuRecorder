/***************************************************************************
      File Name:	Setup.cpp
   File Created:	Thursday, October 9th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
#include "about.h"
#include "config.h"
#include "externals.h"

long CALLBACK SPUtest(void)
{
	if(!LoadPlugins()) return -1;
	FreePlugins();
	return 0;
}

long CALLBACK SPUconfigure(void)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_CONFIG), GetActiveWindow(), (DLGPROC)ConfigDlgProc);
	return 0;
}

void CALLBACK SPUabout(void)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT), GetActiveWindow(), (DLGPROC)About_DlgProc);	
	return;
}
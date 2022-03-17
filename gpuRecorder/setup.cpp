/***************************************************************************
      File Name:	Setup.cpp
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
#include "about.h"
#include "config.h"
#include "externals.h"

//==========================================================================
// 
//==========================================================================

long CALLBACK GPUconfigure(void)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_CONFIG), GetActiveWindow(), (DLGPROC)ConfigDlgProc);
	return 0;
}

//==========================================================================
// 
//==========================================================================

void CALLBACK GPUabout(void)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT), GetActiveWindow(), (DLGPROC)About_DlgProc);	
	return;
}

//==========================================================================
// 
//==========================================================================

long CALLBACK GPUtest(void)
{
	if(!LoadPlugin()) return -1;
	FreePlugin();
	return 0;
}
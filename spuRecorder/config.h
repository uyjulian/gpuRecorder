/***************************************************************************
      File Name:	Config.h
   File Created:	Thursday, October 9th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "stdafx.h"

#define REG_SPU_PLUGIN	"Sound Plugin"

typedef struct tagCFG {
	CHAR  SoundPlugin[MAX_PATH];
	} CFG, *LPCFG;

extern CFG cfg;

BOOL CALLBACK ConfigDlgProc(HWND hW, UINT uMsg, WPARAM wParam, LPARAM lParam);

void LoadConfiguration();
void SaveConfiguration();

#endif
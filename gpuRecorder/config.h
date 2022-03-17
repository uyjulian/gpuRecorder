/***************************************************************************
      File Name:	Config.h
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "stdafx.h"

#define REG_GPU_PLUGIN      "Video Plugin"
#define REG_START			"Start"
#define REG_START_KEY		"Start Key"
#define REG_STOP			"Stop"
#define REG_STOP_KEY		"Stop Key"
#define REG_CUSTOM_PATH     "Custom Path"
#define REG_PATH            "Path"
#define REG_STATUS_TITLEBAR	"Status Title Bar"
#define REG_STATUS_GPU		"Status GPU"
#define REG_STATUS_DRAW		"Status Draw"
#define REG_RECORD_SOUND    "Record Sound"
#define REG_COMPRESS        "Compress"
#define REG_DEMO_MAKER		"Demo Maker"

typedef struct tagCFG {
	CHAR  VideoPlugin[MAX_PATH];
	DWORD Start;
	DWORD StartKey;
	DWORD Stop;
	DWORD StopKey;
	DWORD CustomPath;
	CHAR  Path[MAX_PATH];
	DWORD StatusTitleBar;
	DWORD StatusGPU;
	DWORD StatusDraw;
	DWORD RecordSound;
	DWORD Compress;
	DWORD DemoMaker;
	} CFG, *LPCFG;

extern CFG cfg;

BOOL CALLBACK ConfigDlgProc(HWND hW, UINT uMsg, WPARAM wParam, LPARAM lParam);

void LoadConfiguration();
void SaveConfiguration();


#endif
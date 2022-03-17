/***************************************************************************
      File Name:	Keys.h
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#ifndef _KEYS_H_
#define _KEYS_H_
#include "stdafx.h"

typedef struct tagKeySet {
	char szName[32];
	int  cCode;
	} KEYSET;

extern WNDPROC OldWndProc;
extern KEYSET KeyList[];

LRESULT CALLBACK KeyWndProc(HWND hW, UINT Msg, WPARAM wParam, LPARAM lParam);
void SetKeyHandler(HWND hW);
void ReleaseKeyHandler(HWND hW);
void FillComboBox(HWND hW,int Key);

#endif
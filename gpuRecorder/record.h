/***************************************************************************
      File Name:	Record.h
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#ifndef _RECORD_H_
#define _RECORD_H_

#include "stdafx.h"
#include "externals.h"

typedef struct tagHeader {
	DWORD Compression;
	DWORD Frames;
	DWORD FPS;
	DWORD VideoDataSize;
	DWORD SoundDataSize;
	DWORD CompressedDataSize;
	DWORD IndexOffset;
} HEADER, *LPHEADER;

void RECORD_GPUopen();
void RECORD_GPUclose();

void RECORD_DisplayStatus();
void RECORD_GPUwriteData(unsigned long gdata);
void RECORD_GPUwriteDataMem(unsigned long * pMem, int iSize);
void RECORD_GPUreadData(void);
void RECORD_GPUreadDataMem(unsigned long * pMem, int iSize);
void RECORD_GPUdmaChain(unsigned long * baseAddrL, unsigned long addr);
void RECORD_GPUsetMode(unsigned long gdata);
void RECORD_GPUwriteStatus(unsigned long gdata);
void RECORD_GPUupdateLace(void);
void RECORD_GPUfreeze(unsigned long ulGetFreezeData, GPUFreeze_t * pF);

#endif
/***************************************************************************
      File Name:	Record.h
   File Created:	Thursday, October 9th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#ifndef _RECORD_H_
#define _RECORD_H_

#include "stdafx.h"
#include "externals.h"

void RECORD_SPUreadRegister(unsigned long reg);
void RECORD_SPUwriteRegister(unsigned long reg, unsigned short val);
void RECORD_SPUreadDMA(void);
void RECORD_SPUwriteDMA(unsigned short val);
void RECORD_SPUreadDMAMem(unsigned short * pusPSXMem, int iSize);
void RECORD_SPUwriteDMAMem(unsigned short * pusPSXMem, int iSize);
void RECORD_SPUregisterCallback(void (CALLBACK *callback)(void));
void RECORD_SPUregisterCDDAVolume(void (CALLBACK *CDDAVcallback)(unsigned short, unsigned short));
void RECORD_SPUfreeze(unsigned long ulFreezeMode, SPUFreeze_t * pF);
void RECORD_SPUplayADPCMchannel(xa_decode_t *xap);

void RECORD_SPUgetOne(unsigned long val);
void RECORD_SPUputOne(unsigned long val, unsigned short data);
void RECORD_SPUplaySample(unsigned char ch);
void RECORD_SPUsetAddr(unsigned char ch, unsigned short waddr);
void RECORD_SPUsetPitch(unsigned char ch, unsigned short pitch);
void RECORD_SPUsetVolumeL(unsigned char ch, short vol);
void RECORD_SPUsetVolumeR(unsigned char ch, short vol);
void RECORD_SPUstartChannels1(unsigned short channels);
void RECORD_SPUstartChannels2(unsigned short channels);
void RECORD_SPUstopChannels1(unsigned short channels);
void RECORD_SPUstopChannels2(unsigned short channels);
void RECORD_SPUplaySector(unsigned long mode, unsigned char * p);

#endif
/***************************************************************************
      File Name:	Spu.cpp
   File Created:	Thursday, October 9th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
#include "config.h"
#include "externals.h"
#include "record.h"

//==========================================================================
// New Interface
//==========================================================================
         
long CALLBACK SPUinit(void)
{
	LoadConfiguration();
	if(!LoadPlugins()) { 
		MessageBox(NULL,"Plugin not configured!","Error",MB_ICONERROR); 
		return -1;
	}
	if(REDIRECT_SPUinit) 
		return REDIRECT_SPUinit();
	else
		return 0;
}
long CALLBACK SPUshutdown(void)
{
	long ret;
	if(REDIRECT_SPUshutdown)
		ret = REDIRECT_SPUshutdown();
	else
		ret = 0;
	FreePlugins();
	return ret;
}
long CALLBACK SPUopen(HWND hW)
{
	if(REDIRECT_SPUopen) 
		return REDIRECT_SPUopen(hW);
	else
		return -1;
}
long CALLBACK SPUclose(void)
{
	if(REDIRECT_SPUclose)
		return REDIRECT_SPUclose();
	else
		return -1;
}

unsigned short CALLBACK SPUreadRegister(unsigned long reg) 
{
	RECORD_SPUreadRegister(reg);
	if(REDIRECT_SPUreadRegister) 
		return REDIRECT_SPUreadRegister(reg);
	else
		return 0;
}
void CALLBACK SPUwriteRegister(unsigned long reg, unsigned short val)
{
	RECORD_SPUwriteRegister(reg, val);
	if(REDIRECT_SPUwriteRegister) REDIRECT_SPUwriteRegister(reg, val);
}
unsigned short CALLBACK SPUreadDMA(void)
{
	RECORD_SPUreadDMA();
	if(REDIRECT_SPUreadDMA) 
		return REDIRECT_SPUreadDMA();
	else
		return 0;
}
void CALLBACK SPUwriteDMA(unsigned short val)
{
	RECORD_SPUwriteDMA(val);
	if(REDIRECT_SPUwriteDMA) REDIRECT_SPUwriteDMA(val);	
}
void CALLBACK SPUreadDMAMem(unsigned short * pusPSXMem, int iSize)
{
	RECORD_SPUreadDMAMem(pusPSXMem, iSize);
	if(REDIRECT_SPUreadDMAMem)
		REDIRECT_SPUreadDMAMem(pusPSXMem, iSize);
	else
		if(REDIRECT_SPUreadDMA)
			for(int i = 0; i < iSize; i++) pusPSXMem[i] = REDIRECT_SPUreadDMA();
}
void CALLBACK SPUwriteDMAMem(unsigned short * pusPSXMem, int iSize)
{
	RECORD_SPUwriteDMAMem(pusPSXMem, iSize);
	if(REDIRECT_SPUwriteDMAMem)
		REDIRECT_SPUwriteDMAMem(pusPSXMem, iSize);
	else
		if(REDIRECT_SPUwriteDMA)
			for(int i = 0; i < iSize; i++) REDIRECT_SPUwriteDMA(pusPSXMem[i]);
}
void CALLBACK SPUregisterCallback(void (CALLBACK *callback)(void))
{
	RECORD_SPUregisterCallback(callback);
	if(REDIRECT_SPUregisterCallback) REDIRECT_SPUregisterCallback(callback);
}
void CALLBACK SPUregisterCDDAVolume(void (CALLBACK *CDDAVcallback)(unsigned short, unsigned short))
{
	RECORD_SPUregisterCDDAVolume(CDDAVcallback);
	if(REDIRECT_SPUregisterCDDAVolume) REDIRECT_SPUregisterCDDAVolume(CDDAVcallback);
}
long CALLBACK SPUfreeze(unsigned long ulFreezeMode, SPUFreeze_t * pF)
{
	RECORD_SPUfreeze(ulFreezeMode, pF);
	if(REDIRECT_SPUfreeze)
		return REDIRECT_SPUfreeze(ulFreezeMode, pF);
	else
		return 0;
}
void CALLBACK SPUplayADPCMchannel(xa_decode_t *xap)
{
	RECORD_SPUplayADPCMchannel(xap);
	if(REDIRECT_SPUplayADPCMchannel) REDIRECT_SPUplayADPCMchannel(xap);
}

//==========================================================================
//  Newer interface (not needed)
//==========================================================================
/*
void CALLBACK SPUupdate(void)						{ SPUasync(0); }
void CALLBACK SPUasync(unsigned long cycle)			{ }
*/

//==========================================================================
// Old Interface
//==========================================================================

unsigned short CALLBACK SPUgetOne(unsigned long val)
{
	RECORD_SPUgetOne(val);
	if(REDIRECT_SPUgetOne) 
		return REDIRECT_SPUgetOne(val);
	else
		return 0;
}
void CALLBACK SPUputOne(unsigned long val, unsigned short data)
{
	RECORD_SPUputOne(val, data);
	if(REDIRECT_SPUputOne) REDIRECT_SPUputOne(val, data);
}
void CALLBACK SPUplaySample(unsigned char ch)
{
	RECORD_SPUplaySample(ch);
	if(REDIRECT_SPUplaySample) REDIRECT_SPUplaySample(ch);
}
void CALLBACK SPUsetAddr(unsigned char ch, unsigned short waddr)
{
	RECORD_SPUsetAddr(ch, waddr);
	if(REDIRECT_SPUsetAddr) REDIRECT_SPUsetAddr(ch, waddr);
}
void CALLBACK SPUsetPitch(unsigned char ch, unsigned short pitch)
{
	RECORD_SPUsetPitch(ch, pitch);
	if(REDIRECT_SPUsetPitch) REDIRECT_SPUsetPitch(ch, pitch);
}
void CALLBACK SPUsetVolumeL(unsigned char ch, short vol)
{
	RECORD_SPUsetVolumeL(ch, vol);
	if(REDIRECT_SPUsetVolumeL) REDIRECT_SPUsetVolumeL(ch, vol);
}
void CALLBACK SPUsetVolumeR(unsigned char ch, short vol)
{
	RECORD_SPUsetVolumeR(ch, vol);
	if(REDIRECT_SPUsetVolumeR) REDIRECT_SPUsetVolumeR(ch, vol);
}
void CALLBACK SPUstartChannels1(unsigned short channels)
{
	RECORD_SPUstartChannels1(channels);
	if(REDIRECT_SPUstartChannels1) REDIRECT_SPUstartChannels1(channels);
}
void CALLBACK SPUstartChannels2(unsigned short channels)
{
	RECORD_SPUstartChannels2(channels);
	if(REDIRECT_SPUstartChannels2) REDIRECT_SPUstartChannels2(channels);
}
void CALLBACK SPUstopChannels1(unsigned short channels)
{
	RECORD_SPUstopChannels1(channels);
	if(REDIRECT_SPUstopChannels1) REDIRECT_SPUstopChannels1(channels);
}
void CALLBACK SPUstopChannels2(unsigned short channels)
{
	RECORD_SPUstopChannels2(channels);
	if(REDIRECT_SPUstopChannels2) REDIRECT_SPUstopChannels2(channels);
}
void CALLBACK SPUplaySector(unsigned long mode, unsigned char * p)
{
	RECORD_SPUplaySector(mode, p);
	if(REDIRECT_SPUplaySector) REDIRECT_SPUplaySector(mode, p);
}

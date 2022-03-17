/***************************************************************************
      File Name:	Gpu.cpp
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
#include "config.h"
#include "externals.h"
#include "keys.h"
#include "record.h"

//==========================================================================
// Data transfers
//==========================================================================

void CALLBACK GPUwriteData(unsigned long gdata)
{
	RECORD_GPUwriteData(gdata);
	if(REDIRECT_GPUwriteData) REDIRECT_GPUwriteData(gdata);
}

void CALLBACK GPUwriteDataMem(unsigned long * pMem, int iSize)
{
	RECORD_GPUwriteDataMem(pMem,iSize);
	if(REDIRECT_GPUwriteDataMem)
		REDIRECT_GPUwriteDataMem(pMem,iSize);
	else
		if(REDIRECT_GPUwriteData)
			for(int i = 0; i < iSize; i++) REDIRECT_GPUwriteData(pMem[i]);
}

unsigned long CALLBACK GPUreadData(void)
{
	RECORD_GPUreadData();
	if(REDIRECT_GPUreadData)
		return REDIRECT_GPUreadData();
	else
		return 0;
}

void CALLBACK GPUreadDataMem(unsigned long * pMem, int iSize)
{
	RECORD_GPUreadDataMem(pMem,iSize);
	if(REDIRECT_GPUreadDataMem)
		REDIRECT_GPUreadDataMem(pMem,iSize);
	else
		if(REDIRECT_GPUreadData)
			for(int i =0; i < iSize; i++) pMem[i] = REDIRECT_GPUreadData();
}

unsigned long CALLBACK GPUdmaChain(unsigned long * baseAddrL, unsigned long addr)
{
	RECORD_GPUdmaChain(baseAddrL,addr);
	if(REDIRECT_GPUdmaChain)
		return REDIRECT_GPUdmaChain(baseAddrL,addr);
	else
		return 0;
}

unsigned long CALLBACK GPUgetMode(void)
{
	if(REDIRECT_GPUgetMode)
		return REDIRECT_GPUgetMode();
	else
		return 0;
}

void CALLBACK GPUsetMode(unsigned long gdata)
{
	RECORD_GPUsetMode(gdata);
	if(REDIRECT_GPUsetMode) REDIRECT_GPUsetMode(gdata);
}

//==========================================================================
// 
//==========================================================================

void CALLBACK GPUwriteStatus(unsigned long gdata)
{
	RECORD_GPUwriteStatus(gdata);
	if(REDIRECT_GPUwriteStatus) REDIRECT_GPUwriteStatus(gdata);
}

unsigned long CALLBACK GPUreadStatus(void)
{
	if(REDIRECT_GPUreadStatus)
		return REDIRECT_GPUreadStatus();
	else
		return 0;
}

//==========================================================================
// 
//==========================================================================

long CALLBACK GPUinit()
{
	LoadConfiguration();
	if(!LoadPlugin()) { 
		MessageBox(NULL, "Plugin not configured!", "Error", MB_ICONERROR); 
		return -1;
	}
	if(REDIRECT_GPUinit)
		return REDIRECT_GPUinit();
	else
		return 0;
}

long CALLBACK GPUshutdown()
{
	long ret;
	if(REDIRECT_GPUshutdown)
		ret = REDIRECT_GPUshutdown();
	else
		ret = 0;
	FreePlugin();
	return ret;
}

long CALLBACK GPUopen(HWND hwndGPU)
{
	if(REDIRECT_GPUopen == NULL) return -1;
	GPU_HWND = hwndGPU;
	if(cfg.Start == 1) RECORD_GPUopen();
	if(cfg.Start == 3 || cfg.Stop == 3) SetKeyHandler(GPU_HWND);
	if(REDIRECT_GPUopen)
		return REDIRECT_GPUopen(hwndGPU);
	else
		return 0;
}

long CALLBACK GPUclose()
{
	long result;
	RECORD_GPUclose();
	if(REDIRECT_GPUclose)
		result = REDIRECT_GPUclose();
	else
		result = 0;
	if(cfg.Start == 3 || cfg.Stop == 3) ReleaseKeyHandler(GPU_HWND);
	return result;
}

//==========================================================================
// 
//==========================================================================

void CALLBACK GPUgetScreenPic(unsigned char * pMem)
{
	if(REDIRECT_GPUgetScreenPic) REDIRECT_GPUgetScreenPic(pMem);
}

void CALLBACK GPUshowScreenPic(unsigned char * pMem)
{
	if(REDIRECT_GPUshowScreenPic) REDIRECT_GPUshowScreenPic(pMem);
}

//==========================================================================
// 
//==========================================================================

void CALLBACK GPUupdateLace(void)
{	
	RECORD_GPUupdateLace();
	if(REDIRECT_GPUupdateLace) REDIRECT_GPUupdateLace();
	RECORD_DisplayStatus();
}

void CALLBACK GPUmakeSnapshot(void)
{
	if(REDIRECT_GPUmakeSnapshot) REDIRECT_GPUmakeSnapshot();
}

long CALLBACK GPUfreeze(unsigned long ulGetFreezeData,GPUFreeze_t * pF)
{
	RECORD_GPUfreeze(ulGetFreezeData,pF);
	if(REDIRECT_GPUfreeze)
		return REDIRECT_GPUfreeze(ulGetFreezeData,pF);
	else
		return 0;
}

void CALLBACK GPUdisplayFlags(unsigned long dwFlags)
{
	if(REDIRECT_GPUdisplayFlags) REDIRECT_GPUdisplayFlags(dwFlags);
}
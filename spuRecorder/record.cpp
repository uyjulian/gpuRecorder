/***************************************************************************
      File Name:	Record.cpp
   File Created:	Thursday, October 9th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "record.h"
#include "externals.h"
#include "folder.h"
#include "log.h"

// void RECORD_WriteData(BYTE flags, DWORD iSize = 0, LPVOID pMem = NULL)
// 
// flags:  first 6 bits - data type
//         last 2 bits  - additional data flag
//

// data type
#define SPU_READREGISTER		0x10
#define SPU_WRITEREGISTER		0x11
#define SPU_READDMA				0x12
#define SPU_WRITEDMA			0x13
#define SPU_READDMAMEM			0x14
#define SPU_WRITEDMAMEM			0x15
#define SPU_REGISTERCALLBACK	0x16
#define SPU_REGISTERCDDAVOLUME	0x17
#define SPU_PLAYADPCMCHANNEL	0x18
#define SPU_FREEZE				0x19
#define SPU_GETONE				0x1A
#define SPU_PUTONE				0x1B
#define SPU_PLAYSAMPLE			0x1C
#define SPU_SETADDR				0x1D
#define SPU_SETPITCH			0x1E
#define SPU_SETVOLUMEL			0x1F
#define SPU_SETVOLUMER			0x20
#define SPU_STARTCHANNELS1		0x21
#define SPU_STARTCHANNELS2		0x22
#define SPU_STOPCHANNELS1		0x23
#define SPU_STOPCHANNELS2		0x24
#define SPU_PLAYSECTOR			0x25
// 0x01 - 0x0f reserved for video recording

// additional data flag
#define RF_WRITENUMBER		0x40
#define RF_WRITEBUFFER		0x80

DWORD buffer[2];

//==========================================================================
//
// Sound recording
//
//==========================================================================

//==========================================================================
// New interface
//==========================================================================

void RECORD_SPUreadRegister(unsigned long reg) 
{
	LogAppend("SPUreadRegister: 0x%08lX", reg);
	if(RecordSound == NULL) return;
	RecordSound(SPU_READREGISTER | RF_WRITENUMBER, reg, NULL );
}
void RECORD_SPUwriteRegister(unsigned long reg, unsigned short val)
{
	LogAppend("SPUwriteRegister: 0x%08lX, 0x%04lX", reg, val);
	if(RecordSound == NULL) return;
	buffer[0] = reg;
	buffer[1] = val;
	RecordSound(SPU_WRITEREGISTER | RF_WRITEBUFFER, 8, buffer);
}
void RECORD_SPUreadDMA(void)
{
	LogAppend("SPUreadDMA");
	if(RecordSound == NULL) return;
	RecordSound(SPU_READDMA, 0, NULL);
}
void RECORD_SPUwriteDMA(unsigned short val)
{
	LogAppend("SPUwriteDMA: 0x%04lX", val);
	if(RecordSound == NULL) return;
	RecordSound(SPU_WRITEDMA | RF_WRITENUMBER, val, NULL );
}
void RECORD_SPUreadDMAMem(unsigned short * pusPSXMem, int iSize)
{
	LogAppend("SPUreadDMAMem: 0x%08lX", iSize);
	if(RecordSound == NULL) return;
	RecordSound(SPU_READDMAMEM | RF_WRITENUMBER, (iSize<<1), NULL);
}
void RECORD_SPUwriteDMAMem(unsigned short * pusPSXMem, int iSize)
{
	LogAppend("SPUwriteDMAMem: 0x%08lX", iSize);
	if(RecordSound == NULL) return;
	RecordSound(SPU_WRITEDMAMEM | RF_WRITEBUFFER, (iSize<<1), pusPSXMem);
}
void RECORD_SPUregisterCallback(void (CALLBACK *callback)(void))
{
	LogAppend("SPUregisterCallback");
	if(RecordSound == NULL) return;
	RecordSound(SPU_REGISTERCALLBACK, 0, NULL);
}
void RECORD_SPUregisterCDDAVolume(void (CALLBACK *CDDAVcallback)(unsigned short, unsigned short))
{
	LogAppend("SPUregisterCDDAVolume");
	if(RecordSound == NULL) return;
	RecordSound(SPU_REGISTERCDDAVOLUME, 0, NULL);
}
void RECORD_SPUfreeze(unsigned long ulFreezeMode, SPUFreeze_t * pF)
{
	if(ulFreezeMode != 0) return;
	LogAppend("SPUfreeze");
	if(RecordSound == NULL) return;
	RecordSound(SPU_FREEZE | RF_WRITEBUFFER, sizeof(SPUFreeze_t), pF);
}
void RECORD_SPUplayADPCMchannel(xa_decode_t *xap)
{
	LogAppend("SPUplayADPCMchannel");
	if(RecordSound == NULL) return;
	RecordSound(SPU_PLAYADPCMCHANNEL | RF_WRITEBUFFER, sizeof(xa_decode_t), xap);
}

//==========================================================================
// Old interface
//==========================================================================

void RECORD_SPUgetOne(unsigned long val)
{
	LogAppend("SPUgetOne: 0x%08lX", val);
	if(RecordSound == NULL) return;
	RecordSound(SPU_GETONE | RF_WRITENUMBER, val, NULL);
}
void RECORD_SPUputOne(unsigned long val, unsigned short data)
{
	LogAppend("SPUputOne: 0x%08lX, 0x%04lX", val, data);
	if(RecordSound == NULL) return;
	buffer[0] = val;
	buffer[1] = data;
	RecordSound(SPU_PUTONE | RF_WRITEBUFFER, 8, buffer);
}
void RECORD_SPUplaySample(unsigned char ch)
{
	LogAppend("SPUplaySample: 0x%02lX", ch);
	if(RecordSound == NULL) return;
	RecordSound(SPU_PLAYSAMPLE | RF_WRITENUMBER, ch, NULL);
}
void RECORD_SPUsetAddr(unsigned char ch, unsigned short waddr)
{
	LogAppend("SPUsetAddr: 0x%02lX, 0x%04lX", ch, waddr);
	if(RecordSound == NULL) return;
	buffer[0] = ch;
	buffer[1] = waddr;
	RecordSound(SPU_SETADDR | RF_WRITEBUFFER, 8, buffer);
}
void RECORD_SPUsetPitch(unsigned char ch, unsigned short pitch)
{
	LogAppend("SPUsetPitch: 0x%02lX, 0x%04lX", ch, pitch);
	if(RecordSound == NULL) return;
	buffer[0] = ch;
	buffer[1] = pitch;
	RecordSound(SPU_SETPITCH | RF_WRITEBUFFER, 8, buffer);
}
void RECORD_SPUsetVolumeL(unsigned char ch, short vol)
{
	LogAppend("SPUsetVolumeL: 0x%02lX, 0x%04lX", ch, vol);
	if(RecordSound == NULL) return;
	buffer[0] = ch;
	buffer[1] = vol;
	RecordSound(SPU_SETVOLUMEL | RF_WRITEBUFFER, 8, buffer);
}
void RECORD_SPUsetVolumeR(unsigned char ch, short vol)
{
	LogAppend("SPUsetVolumeR: 0x%02lX, 0x%04lX", ch, vol);
	if(RecordSound == NULL) return;
	buffer[0] = ch;
	buffer[1] = vol;
	RecordSound(SPU_SETVOLUMER | RF_WRITEBUFFER, 8, buffer);
}
void RECORD_SPUstartChannels1(unsigned short channels)
{
	LogAppend("SPUstartChanells1: 0x%04lX", channels);
	if(RecordSound == NULL) return;
	RecordSound(SPU_STARTCHANNELS1 | RF_WRITENUMBER, channels, NULL);
}
void RECORD_SPUstartChannels2(unsigned short channels)
{
	LogAppend("SPUstartChannels2: 0x%04lX", channels);
	if(RecordSound == NULL) return;
	RecordSound(SPU_STARTCHANNELS2 | RF_WRITENUMBER, channels, NULL);
}
void RECORD_SPUstopChannels1(unsigned short channels)
{
	LogAppend("SPUstopChannels1: 0x%04lX", channels);
	if(RecordSound == NULL) return;
	RecordSound(SPU_STOPCHANNELS1 | RF_WRITENUMBER, channels, NULL);
}
void RECORD_SPUstopChannels2(unsigned short channels)
{
	LogAppend("SPUstopChannels2: 0x%04lX", channels);
	if(RecordSound == NULL) return;
	RecordSound(SPU_STOPCHANNELS2 | RF_WRITENUMBER, channels, NULL);
}
void RECORD_SPUplaySector(unsigned long mode, unsigned char * p)
{
	LogAppend("SPUplaySector: 0x%08lX", mode);
	if(RecordSound == NULL) return;
}

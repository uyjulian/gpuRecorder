/***************************************************************************
      File Name:	Record.cpp
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "record.h"
#include "config.h"
#include "externals.h"
#include "folder.h"
#include "log.h"
#include "zlib.h"

// void RECORD_WriteData(BYTE flags, DWORD iSize = 0, LPVOID pMem = NULL)
// 
// flags:  first 6 bits - data type
//         last 2 bits  - additional data flag
//

// data type
#define GPU_WRITEDATA		0x01
#define GPU_WRITEDATAMEM	0x02
#define GPU_READDATA		0x03
#define GPU_READDATAMEM		0x04
#define GPU_DMACHAIN		0x05
#define GPU_SETMODE			0x06
#define GPU_WRITESTATUS		0x07
#define GPU_UPDATELACE		0x08
#define GPU_FREEZE			0x09
// 0x10 - 0x26 reserved for sound recording

// additional data flag
#define RF_WRITENUMBER		0x40
#define RF_WRITEBUFFER		0x80

char PrimTableCount[256] = {
	0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 00
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 10
	4,4,4,4,7,7,7,7,5,5,5,5,9,9,9,9,		// 20 (PF3,PFT3,PF4,PFT4)
	6,6,6,6,9,9,9,9,8,8,8,8,12,12,12,12,	// 30 (PG3,PGT3,PG4,PGT4)
	3,3,3,3,3,3,3,3,-1,-1,-1,-1,-1,-1,-1,-1,// 40 (LF2,LFN)
	4,4,4,4,4,4,4,4,-1,-1,-1,-1,-1,-1,-1,-1,// 50 (LG2,LGN)
	3,3,3,3,4,4,4,4,2,2,2,2,3,3,3,3,		// 60 (R,S,R1,S1)
	2,2,2,2,3,3,3,3,2,2,2,2,3,3,3,3,		// 70 (R8,S8,R16,S16)
	4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 80 (MoveImage)
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// 90
	3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// A0 (LoadImage)
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// B0
	3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// C0 (StoreImage)
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		// D0
	0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,		// E0 (commands)
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

BYTE FileID[8] = {
	'R', 'E', 'C',  0 , // ID TAG
	 0 ,  2 ,  0 ,  0 , // Version TAG
	 };

char FileName[MAX_PATH];
FILE *data = NULL;
DWORD frames = 0;						// Total number of frames
DWORD data_offset = 0;					// Data offset into output file
DWORD max_file_size = 0;				// Peak file size while recording
DWORD last_file_pos = 0;				// Position to write data
DWORD last_save_pos = 0;				// Position on last save (freeze)
DWORD last_save_frames = 0;				// Number of frames on last save
DWORD index_data[0x80000];				// index data (file offset of every frame for seeking)

DWORD dma_buffer[0x80000];				// Buffer for dma chain recording
BYTE buffer[0x00200000];				// Buffer for compression
BYTE buffer_compress[0x00210000];		// Buffer for compression
DWORD buffer_pos = 0;					// Position in buffer
DWORD max_buffer_size = 0;				// Buffer size

DWORD data_written = 0;					// total bytes written
DWORD data_compressed = 0;				// size of compressed data
DWORD last_save_data_written = 0;
DWORD last_save_data_compressed = 0;

DWORD sound_data_written = 0;			// total bytes written for sound data
DWORD last_save_sound_data_written = 0;

GPUFreeze_t freeze;						// Freeze structure


//==========================================================================
// Status Display
//==========================================================================

void RECORD_DisplayStatus()
{
	if(data == NULL) return;
	if(cfg.StatusTitleBar || cfg.StatusGPU || cfg.StatusDraw) {
		char s[255];
		if(cfg.RecordSound)
			sprintf(s,"Recording - Frame: %d , VideoData: %.2f MB, SoundData: %.2f MB", frames, (float)data_written / (float)1000000, (float)sound_data_written / (float)1000000);
		else
			sprintf(s,"Recording - Frame: %d , VideoData: %.2f MB", frames, (float)data_written / (float)1000000);		
		if(cfg.Compress) {
			char t[255];
			float ratio = (float)data_compressed / (float)data_written;
			sprintf(t, ",  Compressed: %.2f MB (%d %%)", (float)data_compressed / (float)1000000, 100 - (int)(ratio*100));
			strcat(s, t);			
		}
		if(cfg.StatusTitleBar) SetWindowText(GPU_HWND, s);
		if(cfg.StatusGPU && REDIRECT_GPUdisplayText) REDIRECT_GPUdisplayText(s);
		if(cfg.StatusDraw) {
			RECT rc; SIZE size;
			GetClientRect(GPU_HWND, &rc);
			HDC hdc = GetDC(GPU_HWND);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, 0x00ffffff);
			GetTextExtentPoint32(hdc, s, strlen(s), &size);
			TextOut(hdc, 0, rc.bottom - size.cy, s, strlen(s));
			ReleaseDC(GPU_HWND, hdc);
		}
	}
}

//==========================================================================
// Sound recording
//==========================================================================

void RECORD_WriteData(BYTE flags, DWORD iSize, LPVOID pMem);

void CALLBACK RecordSound(BYTE flags, DWORD iSize, LPVOID pMem)
{
	if(cfg.RecordSound) {
		// calculate total size to write
		DWORD Total = 1;
		if(flags & RF_WRITENUMBER) 
			Total += 4;
		else if(flags & RF_WRITEBUFFER) 
			Total += 4 + iSize;
		sound_data_written += Total;
		RECORD_WriteData(flags, iSize, pMem);
	}
}

//==========================================================================
// Data writting functions
//==========================================================================

//
// Compress buffer and write to file
//
void RECORD_WriteBuffer()
{
	if(data==NULL) return;
	if(buffer_pos > 0) {
		DWORD buffer_compress_size = sizeof(buffer_compress);
		if(compress((Bytef*)buffer_compress, &buffer_compress_size, (Bytef*)buffer, buffer_pos) != Z_OK) {
			MessageBox(GPU_HWND, "Error compressing data", "Error", MB_ICONERROR);
			buffer_pos = 0;
			return;
		}
		fwrite(&buffer_compress_size, 4, 1, data);
		fwrite(buffer_compress, buffer_compress_size, 1, data);
		data_compressed += buffer_compress_size;
		buffer_pos = 0;		
		last_file_pos = ftell(data);
		if(last_file_pos > max_file_size) max_file_size = last_file_pos;
	}
}

//
// Write data to file / buffer
//
void RECORD_WriteData(BYTE flags, DWORD iSize = 0, LPVOID pMem = NULL)
{
	if(data==NULL) return;
	if(cfg.Compress) {
	//
	// write compressed data
	//
		if((flags & 0x0f)== GPU_UPDATELACE) {
			RECORD_WriteBuffer();
			index_data[frames++] = last_file_pos;
		}
		
		// calculate total size to write
		DWORD Total = 1;
		if(flags & RF_WRITENUMBER) 
			Total += 4;
		else if(flags & RF_WRITEBUFFER) 
			Total += 4 + iSize;
		data_written += Total;

		if((buffer_pos + Total) > max_buffer_size)
			RECORD_WriteBuffer();
		
		if(Total > max_buffer_size) {
			MessageBox(GPU_HWND, "Data size larger than buffer size", "Error", MB_ICONERROR);
			return;
		}

		buffer[buffer_pos++] = flags;
		if(flags & RF_WRITENUMBER) {
			memcpy(&buffer[buffer_pos], &iSize, 4);
			buffer_pos += 4;
		} else if(flags & RF_WRITEBUFFER) {
			memcpy(&buffer[buffer_pos], &iSize, 4); buffer_pos += 4;
			memcpy(&buffer[buffer_pos], pMem, iSize); buffer_pos += iSize;
		}

	} else {
	//
	// Write uncompressed data
	//		

		if((flags & 0x0f)== GPU_UPDATELACE) {
			index_data[frames++] = last_file_pos;
		}

		// calculate total size to write
		DWORD Total = 1;
		if(flags & RF_WRITENUMBER) 
			Total += 4;
		else if(flags & RF_WRITEBUFFER) 
			Total += 4 + iSize;
		data_written += Total;
				
		fwrite(&flags, 1, 1, data);
		if(flags & RF_WRITENUMBER) {
			fwrite(&iSize, 4, 1, data);
		} else if(flags & RF_WRITEBUFFER) {
			fwrite(&iSize, 4, 1, data);
			fwrite(pMem, iSize, 1, data);
		}
		
		last_file_pos = ftell(data);
		if(last_file_pos > max_file_size) max_file_size = last_file_pos;
	}
}

//
// Remember recording position
//
void RECORD_SavePosition()
{
	if(data==NULL) return;
	if(cfg.Compress) RECORD_WriteBuffer();
	last_save_pos = ftell(data);
	last_save_frames = frames;
	last_save_data_written = data_written;
	last_save_data_compressed = data_compressed;
	last_save_sound_data_written = sound_data_written;
}

//
// Restore recording postions
//
void RECORD_RestorePosition()
{
	if(data==NULL) return;
	fseek(data, last_save_pos, SEEK_SET);
	frames = last_save_frames;
	data_written = last_save_data_written;
	data_compressed = last_save_data_compressed;
	sound_data_written = last_save_sound_data_written;
}

//==========================================================================
// Initialization 
//==========================================================================

void RECORD_GPUopen()
{
	if(data) { fclose(data); data = NULL; }
	//
	// Create output file
	//
	char Path[MAX_PATH];
	short Index = 0;
	if(cfg.CustomPath && strlen(cfg.Path) > 0) {
		int i = strlen(cfg.Path);
		if(i > 0 && cfg.Path[i-1] != '\\') { cfg.Path[i] = '\\'; cfg.Path[i+1] = 0; }
		if(CreatePath(cfg.Path) == FALSE) {
			MessageBox(GetActiveWindow(), "Error creating output directory!", "Error", MB_ICONERROR);
			return;
		}
		strcpy(Path, cfg.Path);
	} else {
		mkdir("Record");
		strcpy(Path,"Record\\");
	}
	while(true) {
		sprintf(FileName,"%sRecord%04d.rec", Path, Index++);
		if((data=fopen(FileName,"rb"))==NULL) break;
		fclose(data);
		data = NULL;
		if(Index>9999) return;
	}
	//
	// Initialze
	//
	data = fopen(FileName,"wb");
	if(data == NULL) {
		MessageBox(GPU_HWND, "Error creating output file!", "Error", MB_ICONERROR);
		return;
	}

	HEADER header;
	memset(&header, 0, sizeof(header));
	header.Compression = cfg.Compress;
	fwrite(&FileID, 8, 1, data);
	fwrite(&header, sizeof(HEADER), 1, data);
		
	max_buffer_size = sizeof(buffer) - 1;
	memset(buffer, 0, sizeof(buffer));
	memset(buffer_compress, 0, sizeof(buffer_compress));
	
	frames = 0;
	data_offset = last_file_pos = ftell(data);
	if(last_file_pos > max_file_size) max_file_size = last_file_pos;
	last_save_pos = 0;
	last_save_frames = 0;
	data_written = 0;
	data_compressed = 0;
	last_save_data_written = 0;
	last_save_data_compressed = 0;
	sound_data_written = 0;
	last_save_sound_data_written = 0;

	if(cfg.Start == 3 && REDIRECT_GPUfreeze) {
		memset(&freeze,0,sizeof(freeze));
		freeze.ulFreezeVersion = 1;
		REDIRECT_GPUfreeze(1,&freeze);
		RECORD_GPUfreeze(0,&freeze);
	}

	LogStart();
}

//==========================================================================
// DeInitialization function
//==========================================================================

void RECORD_GPUclose()
{
	if(data == NULL) return;

	// Empty buffer if in compression mode
	if(cfg.Compress) RECORD_WriteBuffer();		
		
	// Write header at begining of file
	HEADER header;
	memset(&header, 0, sizeof(HEADER));
	header.Compression = cfg.Compress;
	header.Frames = frames;
	header.FPS = ((REDIRECT_GPUreadStatus()>>0x14)&0x1)?50:60;
	header.VideoDataSize = data_written - sound_data_written;
	header.SoundDataSize = sound_data_written;
	header.CompressedDataSize = data_compressed;
	header.IndexOffset = last_file_pos;
	fseek(data, 8, SEEK_SET);
	fwrite(&header, sizeof(HEADER), 1, data);
	
	// Write index data at end of file
	fseek(data, last_file_pos, SEEK_SET);
	for(int i=0; i<(int)frames; i++)
		fwrite(&index_data[i], 4, 1, data);
	last_file_pos = ftell(data);
	if(last_file_pos > max_file_size) max_file_size = last_file_pos;
	
	fclose(data);
	data = NULL;
		
	//
	// Trim file (resize)
	//
	if(max_file_size > last_file_pos) {
//		char s[255];
		int fh;
		if((fh = _open(FileName, _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE)) != -1) {
//			sprintf(s,"File length before: %ld", _filelength(fh));MessageBox(NULL,s,"",0);
//			if(_chsize(fh,last_file_pos)==0)
//				MessageBox(NULL,"Size successfully changed\n","",0);
//			else
//				MessageBox(NULL,"Problem in changing the size\n","",0);
//			sprintf(s,"File length after:  %ld\n", _filelength(fh));MessageBox(NULL,s,"",0);
			_chsize(fh, last_file_pos);
			_close(fh);
		}
	}

	LogEnd();
}

//==========================================================================
//
// GPU Recording functions
//
//==========================================================================


//==========================================================================
// Data transfers
//==========================================================================

void RECORD_GPUwriteData(unsigned long gdata)
{
	if(data==NULL) return;
	LogAppend("GPUwriteData: 0x%08lX", gdata);
	RECORD_WriteData(GPU_WRITEDATA | RF_WRITENUMBER, gdata);
}

void RECORD_GPUwriteDataMem(unsigned long * pMem, int iSize)
{
	if(data==NULL) return;
	if(iSize <= 0) return;
	LogAppend("GPUwriteDataMem: 0x%08lX", iSize);
	unsigned long size = iSize<<2;
	RECORD_WriteData(GPU_WRITEDATAMEM | RF_WRITEBUFFER, size, pMem);
}

void RECORD_GPUreadData(void)
{ 
	if(data==NULL) return;
	LogAppend("GPUreadData");
	RECORD_WriteData(GPU_READDATA);
}

void RECORD_GPUreadDataMem(unsigned long * pMem, int iSize)
{
	if(data==NULL) return;
	if(iSize <= 0) return;
	unsigned long size = iSize<<2;
	LogAppend("GPUreadDataMem: 0x%08lX", iSize);
	RECORD_WriteData(GPU_READDATAMEM | RF_WRITENUMBER, iSize);
}

void RECORD_GPUdmaChain(unsigned long * baseAddrL, unsigned long addr)
{
	if(data==NULL) return;
	unsigned long base,curr,basecount,size;

	unsigned char *baseAddrB;
	unsigned long dmaMem;
	unsigned char cmd;
	int count,cnt,counter,i;

	baseAddrB = (unsigned char*) baseAddrL;
	counter = 32768; 
	base = 0; curr = 1;
	do {
		count = baseAddrB[addr+3];
		dmaMem = (addr>>2) + 1;
		basecount = 0;
		while(count>0) {
/*  Skipping image transfers
			if(GPU_TRANSFER.IMAGE==TRANSFER_MODE_CPU2GPU) {
				WriteData(baseAddrL[dmaMem/4]);
				dmaMem+=4;count--;
			} else {
*/
			cmd = baseAddrB[(dmaMem<<2)+3];
			cnt = PrimTableCount[cmd];
			if(cnt==0) { dmaMem++; count--; }
			else if(cnt<0) {
				do {
					dma_buffer[curr++] = baseAddrL[dmaMem];
					basecount++; count--;
				} while((baseAddrL[dmaMem++]&0xf0000000)!=0x50000000 && count>0);
			} else {
				for(i=0;i<cnt;i++) dma_buffer[curr++] = baseAddrL[dmaMem++];
				basecount += cnt; count -= cnt;
			}
		}
		if(!(counter--)) { dma_buffer[base] = (basecount<<24)|0xffffff; break; }
		addr = baseAddrL[addr>>2]&0x1fffff;
		if (addr==0 || addr==0x1fffff) { dma_buffer[base] = (basecount<<24)|0xffffff; break; }
		if(basecount>0) { dma_buffer[base] = (basecount<<24)|((curr<<2)&0xffffff); base = curr; curr++; }
	} while(true);

	size = curr<<2;
	if(size > 0) {
		RECORD_WriteData(GPU_DMACHAIN | RF_WRITEBUFFER, size, dma_buffer);
		LogAppend("GPUdmaChain: 0x%08lX", size);
	}
}

void RECORD_GPUsetMode(unsigned long gdata)
{
	if(data==NULL) return;
	LogAppend("GPUsetMode: 0x%08lX", gdata);
	RECORD_WriteData(GPU_SETMODE | RF_WRITENUMBER, gdata);
}

//==========================================================================
// Status read/write
//==========================================================================

void RECORD_GPUwriteStatus(unsigned long gdata)
{
	if(data==NULL) return;
	LogAppend("GPUwriteStatus: 0x%08lX", gdata);
	RECORD_WriteData(GPU_WRITESTATUS | RF_WRITENUMBER, gdata);
}

//==========================================================================
// Frame advance
//==========================================================================

void RECORD_GPUupdateLace(void)
{
	if(data == NULL) return;
	LogAppend("GPUupdateLace (frame %d)", frames);
	RECORD_WriteData(GPU_UPDATELACE);
}

//==========================================================================
// Save state
//==========================================================================

void RECORD_GPUfreeze(unsigned long ulGetFreezeData, GPUFreeze_t * pF)
{
	if(ulGetFreezeData == 1) RECORD_SavePosition();
	if(ulGetFreezeData != 0) return;
	if(cfg.Start == 2 && data == NULL) RECORD_GPUopen();
	if(data == NULL) return;
	LogAppend("GPUfreeze");	
	if(cfg.DemoMaker && last_save_pos > 0) {
		RECORD_RestorePosition();
		return;
	}
	if(last_save_pos == 0) RECORD_SavePosition();
	RECORD_WriteData(GPU_FREEZE | RF_WRITEBUFFER, sizeof(GPUFreeze_t), pF);
}
/***************************************************************************
      File Name:	StdAfx.h
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include <tchar.h>
#include <stdio.h>
#include <direct.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include "resource.h"

#define LIBRARY_NAME	"DarkMan's GPU Recorder"

typedef struct tagGPUFreeze {
	unsigned long ulFreezeVersion;      // should be always 1 for now (set by main emu)
	unsigned long ulStatus;             // current gpu status
	unsigned long ulControl[256];       // latest control register values
	unsigned char psxVRam[1024*512*2];  // current VRam image
	} GPUFreeze_t;

extern HINSTANCE hInstance;

/****** Helper macros *****************************************/

#define IDC_HAND MAKEINTRESOURCE(32649)

#define randomize(x) if(x==0) srand(GetTickCount()); else srand(x);
#define rnd(x) rand()*(x)/RAND_MAX

#define Limit(val,min,max) if(val<min) val = min; if(val>max) val = max;

#define Del(hObj) if(hObj) { DeleteObject(hObj); hObj = NULL; }

#define SetFont(hW,hFont) SendMessage(hW,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(TRUE, 0))

/****** ListBox control message APIs *****************************************/

#define Slider_Init(hW, nItem, From, To, Freq, Page)            SendMessage(GetDlgItem(hW,nItem),TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(From,To)); SendMessage(GetDlgItem(hW,nItem),TBM_SETTICFREQ,(WPARAM)Freq,(LPARAM)0); SendMessage(GetDlgItem(hW,nItem),TBM_SETPAGESIZE,(WPARAM)0,(LPARAM)Page)
#define Slider_GetPosition(hW, nItem)                           SendMessage(GetDlgItem(hW,nItem),TBM_GETPOS,0,0)
#define Slider_SetPosition(hW, nItem, lPosition)                SendMessage(GetDlgItem(hW,nItem),TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lPosition)
#define Slider_InitPosition(Value, hW, SliderID, Min, Max)      Slider_Init(hW,SliderID,Min,Max,1,4); Limit(Value,Min,Max); Slider_SetPosition(hW,SliderID,Value);
#define Slider_UpdatePosition(Value, hW, SliderID, DisplayID)   Value = Slider_GetPosition(hW,SliderID); if(DisplayID>0) { char s[255]; sprintf(s,"%d",Value); Static_SetText(GetDlgItem(hW,DisplayID),s); }
#define Slider_UpdateTime(Value, hW, SliderID, DisplayID)       Value = Slider_GetPosition(hW,SliderID); if(DisplayID>0) { char s[255]; int t = Value*10; sprintf(s,"%02d:%02d",t/60,t%60); Static_SetText(GetDlgItem(hW,DisplayID),s); }

/****** ToolBox control message APIs *****************************************/

#define ToolBar_AddBitmap(hwndCtl, nButtons, lptbab)            (int)SNDMSG(hwndCtl, TB_ADDBITMAP, (WPARAM)nButtons, (LPARAM)(LPTBADDBITMAP)lptbab)
#define ToolBar_AddButtons(hwndCtl, uNumButtons, lpButtons)     (BOOL)SNDMSG(hwndCtl, TB_ADDBUTTONS, (WPARAM)(UINT)uNumButtons, (LPARAM)(LPTBBUTTON)lpButtons)
#define ToolBar_AddString(hwndCtl, hinst, idString)             (int)SNDMSG(hwndCtl, TB_ADDSTRING, (WPARAM) (HINSTANCE) hinst, ((LPARAM) MAKELONG(idString, 0))
#define ToolBar_AutoSize(hwndCtl)                               (void)SNDMSG(hwndCtl, TB_AUTOSIZE, 0L, 0L)
#define ToolBar_ButtonCount(hwndCtl)                            (int)SNDMSG(hwndCtl, TB_BUTTONCOUNT, 0L, 0L)
#define ToolBar_ButtonStructSize(hwndCtl, cb)                   (void)SNDMSG(hwndCtl, TB_BUTTONSTRUCTSIZE, (WPARAM) cb, 0L)
#define ToolBar_ChangeBitmap(hwndCtl, idButton, iBitmap)        (BOOL)SNDMSG(hwndCtl, TB_CHANGEBITMAP, (WPARAM)idButton, (LPARAM)MAKELPARAM(iBitmap, 0))
#define ToolBar_CheckButton(hwndCtl, idButton, fCheck)          (BOOL)SNDMSG(hwndCtl, TB_CHECKBUTTON, (WPARAM)idButton, (LPARAM)MAKELONG(fCheck, 0))
#define ToolBar_CommandToIndex(hwndCtl, idButton)               (int)SNDMSG(hwndCtl, TB_COMMANDTOINDEX, (WPARAM)idButton, 0L)
#define ToolBar_Customize(hwndCtl)                              (void)SNDMSG(hwndCtl, TB_CUSTOMIZE, 0L, 0L)
#define ToolBar_DeleteButton(hwndCtl, iButton)                  (BOOL)SNDMSG(hwndCtl, TB_DELETEBUTTON, (WPARAM)iButton, 0L)
#define ToolBar_EnableButton(hwndCtl, idButton, fEnable)        (BOOL)SNDMSG(hwndCtl, TB_ENABLEBUTTON, (WPARAM)idButton, (LPARAM)MAKELONG(fEnable, 0))
#define ToolBar_InsertButton(hwndCtl, iButton, lpButton)        (BOOL)SNDMSG(hwndCtl, TB_INSERTBUTTON, (WPARAM) iButton, (LPARAM) (LPTBBUTTON) lpButton)
#define ToolBar_IsButtonChecked(hwndCtl, idButton)              (int)SNDMSG(hwndCtl, TB_ISBUTTONCHECKED, (WPARAM) idButton, 0L)
#define ToolBar_IsButtonEnabled(hwndCtl, idButton)              (int)SNDMSG(hwndCtl, TB_ISBUTTONENABLED, (WPARAM) idButton, 0L)
#define ToolBar_IsButtonHidden(hwndCtl, idButton)               (int)SNDMSG(hwndCtl, TB_ISBUTTONHIDDEN, (WPARAM) idButton, 0L)
#define ToolBar_IsButtonHighlited(hwndCtl, idButton)            (int)SNDMSG(hwndCtl, TB_ISBUTTONHIGHLIGHTED, (WPARAM) idButton, 0L)
#define ToolBar_IsButtonIndeterminate(hwndCtl, idButton)        (int)SNDMSG(hwndCtl, TB_ISBUTTONINDETERMINATE, (WPARAM) idButton, 0L)
#define ToolBar_IsButtonPressed(hwndCtl, idButton)              (int)SNDMSG(hwndCtl, TB_ISBUTTONPRESSED, (WPARAM) idButton, 0L)
#define ToolBar_LoadImages(hwndCtl, iBitmapID, hinst)           (int)SNDMSG(hwndCtl, TB_LOADIMAGES, (WPARAM)(INT) iBitmapID, (LPARAM)(HINSTANCE) hinst)
#define ToolBar_SetBitmapSize(hwndCtl, dxBitmap, dyBitmap)      (BOOL)SNDMSG(hwndCtl, TB_SETBITMAPSIZE, 0L, (LPARAM) MAKELONG(dxBitmap, dyBitmap))
#define ToolBar_SetButtonInfo(hwndCtl, iID, lptbbi)             (int)SNDMSG(hwndCtl, TB_SETBUTTONINFO, (WPARAM)(INT) iID, (LPARAM)(LPTBBUTTONINFO) lptbbi)
#define ToolBar_SetButtonSize(hwndCtl, dxButton, dyButton)      (BOOL)SNDMSG(hwndCtl, TB_SETBUTTONSIZE, 0L, (LPARAM) MAKELONG(dxButton, dyButton))
#define ToolBar_SetButtonWidth(hwndCtl, cxMin, cxMax)           (int)SNDMSG(hwndCtl, TB_SETBUTTONWIDTH, 0L, (LPARAM)(DWORD) MAKELONG(cxMin,cxMax))
#define ToolBar_SetColorScheme(hwndCtl, lpcs)                   (void)SNDMSG(hwndCtl, TB_SETCOLORSCHEME, 0L, (LPARAM)(LPCOLORSCHEME) lpcs)
#define ToolBar_SetDisabledImageList(hwndCtl, himlNewDisabled)  (HIMAGELIST)SNDMSG(hwndCtl, TB_SETDISABLEDIMAGELIST, 0L, (LPARAM)(HIMAGELIST) himlNewDisabled)
#define ToolBar_SetHotImageList(hwndCtl, himlNewHot)            (HIMAGELIST)SNDMSG(hwndCtl, TB_SETHOTIMAGELIST, 0L, (LPARAM)(HIMAGELIST) himlNewHot)
#define ToolBar_SetImageList(hwndCtl, himlNew)                  (HIMAGELIST)SNDMSG(hwndCtl, TB_SETIMAGELIST, 0L, (LPARAM)(HIMAGELIST) himlNew)
#define ToolBar_SetIndent(hwndCtl, iIndent)                     (int)SNDMSG(hwndCtl, TB_SETINDENT, (WPARAM)(INT) iIndent, 0L)
#define ToolBar_SetPadding(hwndCtl, cx, cy)                     (DWORD)SNDMSG(hwndCtl, TB_SETPADDING, 0L, lParam = MAKELPARAM(cx, cy))
#define ToolBar_SetParent(hwndCtl, hwndParent)                  (HWND)SNDMSG(hwndCtl, TB_SETPARENT, (WPARAM) (HWND) hwndParent, 0L)
#define ToolBar_SetRows(hwndCtl, cRows, fLarger, lprc)          (void)SNDMSG(hwndCtl, TB_SETROWS, (WPARAM) MAKEWPARAM(cRows, fLarger), (LPARAM) (LPRECT) lprc)
#define ToolBar_SetState(hwndCtl, idButton, fState)             (BOOL)SNDMSG(hwndCtl, TB_SETSTATE, (WPARAM) idButton, (LPARAM) MAKELONG(fState, 0))
#define ToolBar_SetStyle(hwndCtl, dwStyle)                      (void)SNDMSG(hwndCtl, TB_SETSTYLE, 0L,(LPARAM)(DWORD) dwStyle)
#define ToolBar_SetUnicodeFormat(hwndCtl, fUnicode)             (DWORD)SNDMSG(hwndCtl, TB_SETUNICODEFORMAT, (WPARAM)(BOOL)fUnicode, 0L)
#define ToolBar_SetWindowTheme(hwndCtl, pwStr)                  (LRESULT)SNDMSG(hwndCtl, TB_SETWINDOWTHEME, 0L, (LPARAM) (LPWSTR) pwStr)

/****** Last error description *****************************************/

#define DescribeLastError { \
	LPVOID lpMsgBuf; \
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
		(LPTSTR)&lpMsgBuf, 0, NULL ); \
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION ); \
	LocalFree( lpMsgBuf ); }

#define DescribeSystemError(ErrorCode) { \
	LPVOID lpMsgBuf; \
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
		NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
		(LPTSTR)&lpMsgBuf, 0, NULL ); \
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION ); \
	LocalFree( lpMsgBuf ); }

/****** Window information *****************************************/

#define DisplayWindowInfo(hwnd) { \
	WINDOWINFO wi; char s[1024]; \
	memset(&wi,0,sizeof(WINDOWINFO)); wi.cbSize = sizeof(wi); \
	GetWindowInfo(hwnd,&wi); sprintf(s, \
	"cbSize          : %d\nrcWindow        : (%d,%d)(%d,%d)\nrcClient        : (%d,%d)(%d,%d)\n" \
	"dwStyle         : %d\ndwExStyle       : %d\ndwWindowStatus  : %d\ncxWindowBorders : %d\n" \
	"cyWindowBorders : %d\natomWindowType  : %d\nwCreatorVersion : %d",(int)wi.cbSize, \
	(int)wi.rcWindow.left,(int)wi.rcWindow.top,(int)wi.rcWindow.right,(int)wi.rcWindow.bottom, \
	(int)wi.rcClient.left,(int)wi.rcClient.top,(int)wi.rcClient.right,(int)wi.rcClient.bottom, \
	(int)wi.dwStyle,(int)wi.dwExStyle,(int)wi.dwWindowStatus,(int)wi.cxWindowBorders, \
	(int)wi.cyWindowBorders,(int)wi.atomWindowType,(int)wi.wCreatorVersion); \
	MessageBox(hwnd,s,"WindiowInfo",0); }

/****** Draw value on window *****************************************/

#define OutInt(hW,Value) { \
		HDC hdc = GetDC(hW); char s[255]; \
		SetBkMode(hdc,OPAQUE); SetBkColor(hdc,0xffffff); SetTextColor(hdc,0); \
		sprintf(s,"   0x%lX - %d   ",Value,Value); TextOut(hdc,0,0,s,strlen(s)); \
		ReleaseDC(hW,hdc); }

#endif
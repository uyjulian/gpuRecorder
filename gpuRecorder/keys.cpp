/***************************************************************************
      File Name:	Keys.cpp
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "keys.h"
#include "config.h"
#include "externals.h"
#include "record.h"

WNDPROC	OldWndProc = NULL;
KEYSET KeyList[] = {
	{"BACKSPACE",	VK_BACK},
	{"TAB",			VK_TAB},
	{"CLEAR",		VK_CLEAR},
	{"ENTER",		VK_RETURN},
	{"SHIFT",		VK_SHIFT},
	{"CTRL",		VK_CONTROL},
	{"ALT",			VK_MENU},
	{"PAUSE",		VK_PAUSE},
	{"CAPS LOCK",	VK_CAPITAL},
	{"ESC",			VK_ESCAPE},
	{"SPACEBAR",	VK_SPACE},
	{"PAGE UP",		VK_PRIOR},
	{"PAGE DOWN",	VK_NEXT},
	{"END",			VK_END},
	{"HOME",		VK_HOME},
	{"LEFT ARROW",	VK_LEFT},
	{"UP ARROW",	VK_UP},
	{"RIGHT ARROW",	VK_RIGHT},
	{"DOWN ARROW",	VK_DOWN},
	{"SELECT",		VK_SELECT},
	{"PRINT",		VK_PRINT},
	{"EXECUTE",		VK_EXECUTE},
	{"PRINT SCREEN",VK_SNAPSHOT},
	{"INS",			VK_INSERT},
	{"DEL",			VK_DELETE},
	{"HELP",		VK_HELP},
	{"0",			0x30},
	{"1",			0x31},
	{"2",			0x32},
	{"3",			0x33},
	{"4",			0x34},
	{"5",			0x35},
	{"6",			0x36},
	{"7",			0x37},
	{"8",			0x38},
	{"9",			0x39},
	{"A",			0x41},
	{"B",			0x42},
	{"C",			0x43},
	{"D",			0x44},
	{"E",			0x45},
	{"F",			0x46},
	{"G",			0x47},
	{"H",			0x48},
	{"I",			0x49},
	{"J",			0x4A},
	{"K",			0x4B},
	{"L",			0x4C},
	{"M",			0x4D},
	{"N",			0x4E},
	{"O",			0x4F},
	{"P",			0x50},
	{"Q",			0x51},
	{"R",			0x52},
	{"S",			0x53},
	{"T",			0x54},
	{"U",			0x55},
	{"V",			0x56},
	{"W",			0x57},
	{"X",			0x58},
	{"Y",			0x59},
	{"Z",			0x5A},
	{"Numpad 0",	VK_NUMPAD0},
	{"Numpad 1",	VK_NUMPAD1},
	{"Numpad 2",	VK_NUMPAD2},
	{"Numpad 3",	VK_NUMPAD3},
	{"Numpad 4",	VK_NUMPAD4},
	{"Numpad 5",	VK_NUMPAD5},
	{"Numpad 6",	VK_NUMPAD6},
	{"Numpad 7",	VK_NUMPAD7},
	{"Numpad 8",	VK_NUMPAD8},
	{"Numpad 9",	VK_NUMPAD9},
	{"Multiply",	VK_MULTIPLY},
	{"Add",			VK_ADD},
	{"Separator",	VK_SEPARATOR},
	{"Subtract",	VK_SUBTRACT},
	{"Decimal",		VK_DECIMAL},
	{"Divide",		VK_DIVIDE},
	{"F1",			VK_F1},
	{"F2",			VK_F2},
	{"F3",			VK_F3},
	{"F4",			VK_F4},
	{"F5",			VK_F5},
	{"F6",			VK_F6},
	{"F7",			VK_F7},
	{"F8",			VK_F8},
	{"F9",			VK_F9},
	{"F10",			VK_F10},
	{"F11",			VK_F11},
	{"F12",			VK_F12},
	{"F13",			VK_F13},
	{"F14",			VK_F14},
	{"F15",			VK_F15},
	{"F16",			VK_F16},
	{"F17",			VK_F17},
	{"F18",			VK_F18},
	{"F19",			VK_F19},
	{"F20",			VK_F20},
	{"F21",			VK_F21},
	{"F22",			VK_F22},
	{"F23",			VK_F23},
	{"F24",			VK_F24},
	{"NUM LOCK",	VK_NUMLOCK},
	{"SCROLL LOCK",	VK_SCROLL},
	{"",			0x00}};

//==========================================================================
// 
//==========================================================================

LRESULT CALLBACK KeyWndProc(HWND hW, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg) {
		case WM_KEYDOWN:
			if(cfg.Start == 3 && wParam == cfg.StartKey) { RECORD_GPUopen(); return TRUE; }
			if(cfg.Stop == 3 && wParam == cfg.StopKey) { RECORD_GPUclose(); return TRUE; }
			break;
	}
	return OldWndProc(hW,Msg,wParam,lParam);
}

//==========================================================================
// 
//==========================================================================

void SetKeyHandler(HWND hW)
{
	OldWndProc = (WNDPROC)GetWindowLong(hW, GWL_WNDPROC);
	SetWindowLong(hW, GWL_WNDPROC, (LONG)KeyWndProc);
}

//==========================================================================
// 
//==========================================================================

void ReleaseKeyHandler(HWND hW)
{
	if(OldWndProc) {
		SetWindowLong(hW, GWL_WNDPROC, (LONG)OldWndProc);
		OldWndProc = NULL;
	}
}

//==========================================================================
// 
//==========================================================================

void FillComboBox(HWND hW, int Key)
{
	int i = 0,selected = 0;
	ComboBox_ResetContent(hW);
	while(KeyList[i].cCode) {
		if(KeyList[i].cCode == Key) selected = i;
		ComboBox_AddString(hW, KeyList[i++].szName);
	}
	ComboBox_SetCurSel(hW, selected);
}
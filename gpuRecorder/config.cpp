/***************************************************************************
      File Name:	Config.cpp
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
#include "config.h"
#include "externals.h"
#include "folder.h"
#include "keys.h"
#include "reg.h"

CFG cfg;

#define MAX_PLUGINS 100
char VideoPlugins[MAX_PATH][MAX_PLUGINS];

BOOL OnInitConfigDialog(HWND hW);
void OnOK(HWND hW);
void OnTest(HWND hW);
void OnConfig(HWND hW);
void OnAbout(HWND hW);

//==========================================================================
// Config dialog procedure
//==========================================================================

BOOL CALLBACK ConfigDlgProc(HWND hW, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
		case WM_INITDIALOG:
			return OnInitConfigDialog(hW);
		case WM_COMMAND: {
			switch(LOWORD(wParam)) {
				case IDC_GPU_TEST:		OnTest(hW);			return TRUE;
				case IDC_GPU_CONFIG:	OnConfig(hW);		return TRUE;
				case IDC_GPU_ABOUT:		OnAbout(hW);		return TRUE;
				case IDC_CUSTOM_PATH: {
					BOOL Checked = Button_GetCheck(GetDlgItem(hW, IDC_CUSTOM_PATH));
					EnableWindow(GetDlgItem(hW, IDC_PATH), Checked);
					EnableWindow(GetDlgItem(hW, IDC_BROWSE), Checked);
					break;
				}
				case IDC_BROWSE: {
					char buffer[MAX_PATH];
					if(BrowseDirectory("Select output directory", buffer, sizeof(buffer), hW))
						Edit_SetText(GetDlgItem(hW, IDC_PATH), buffer);
					break;
				}
				case IDCANCEL:		EndDialog(hW,FALSE);return TRUE;
				case IDOK:			OnOK(hW);			return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////

void SearchPlugins(HWND hWC, char *SearchPath, int &SelectedVideoPlugin)
{
	WIN32_FIND_DATA FindData;
	HANDLE hFindFile;
	HMODULE hModule;
	char SearchFile[255];
	memset(&FindData, 0, sizeof(FindData));
	sprintf(SearchFile, "%s*.dll", SearchPath);
	if((hFindFile = FindFirstFile(SearchFile, &FindData)) != NULL) {
		do {
			char FileName[MAX_PATH];
			sprintf(FileName, "%s%s", SearchPath, FindData.cFileName);
			if((hModule = LoadLibrary(FileName)) != NULL) {
				if(((FARPROC&)REDIRECT_PSEgetLibType = GetProcAddress(hModule, "PSEgetLibType")) != NULL)
					if(((FARPROC&)REDIRECT_PSEgetLibName = GetProcAddress(hModule, "PSEgetLibName")) != NULL)
						if(((FARPROC&)REDIRECT_PSEgetLibVersion = GetProcAddress(hModule, "PSEgetLibVersion")) != NULL)
							if(REDIRECT_PSEgetLibType() == 2)
								if(strcmp(REDIRECT_PSEgetLibName(),LIBRARY_NAME)) {
									unsigned long version = REDIRECT_PSEgetLibVersion();
									char buffer[MAX_PATH];
									int Index;
									sprintf(buffer,"%s %d.%d", REDIRECT_PSEgetLibName(),(version>>8)&0xff,version&0xff);
									if(ComboBox_GetCount(hWC) < MAX_PLUGINS) {
										ComboBox_AddString(hWC, buffer);
										Index = ComboBox_GetCount(hWC) - 1;
										sprintf(VideoPlugins[Index], "%s%s", SearchPath, FindData.cFileName);
										if(stricmp(VideoPlugins[Index], cfg.VideoPlugin)==0) SelectedVideoPlugin = Index;
									}
								}
				FreeLibrary(hModule);
			}
		} while(FindNextFile(hFindFile, &FindData));
		FindClose(hFindFile);
	}
}

////////////////////////////////////////////////////////////////////////////

BOOL OnInitConfigDialog(HWND hW)
{
	HWND hWC;
	int LastSel;
	char buffer[MAX_PATH];
	LoadConfiguration();
	//--------------- PLUGIN -------------------------
	hWC = GetDlgItem(hW, IDC_GPU_PLUGIN); LastSel = -1;
	AppPath(buffer, sizeof(buffer));
	ComboBox_ResetContent(hWC);
	SearchPlugins(hWC, buffer, LastSel);
	strcat(buffer, "plugins\\");
	SearchPlugins(hWC, buffer, LastSel);
	if(ComboBox_GetCount(hWC) > 0) {
		if(LastSel >= 0 && LastSel < ComboBox_GetCount(hWC))
			ComboBox_SetCurSel(hWC, LastSel);
		else
			ComboBox_SetCurSel(hWC, 0);
	} else
		EnableWindow(GetDlgItem(hW,IDOK), FALSE);
	//--------------- RECORDING START -------------------------
	Button_SetCheck(GetDlgItem(hW, IDC_START1), (cfg.Start == 1) ? 1 : 0);
	Button_SetCheck(GetDlgItem(hW, IDC_START2), (cfg.Start == 2) ? 1 : 0);
	Button_SetCheck(GetDlgItem(hW, IDC_START3), (cfg.Start == 3) ? 1 : 0);
	FillComboBox(GetDlgItem(hW, IDC_START_KEY), cfg.StartKey);
	//--------------- RECORDING STOP -------------------------
	ShowWindow(GetDlgItem(hW, IDC_STOP2), SW_HIDE);
	Button_SetCheck(GetDlgItem(hW, IDC_STOP1), (cfg.Stop == 1) ? 1 : 0);
	Button_SetCheck(GetDlgItem(hW, IDC_STOP2), (cfg.Stop == 2) ? 1 : 0);
	Button_SetCheck(GetDlgItem(hW, IDC_STOP3), (cfg.Stop == 3) ? 1 : 0);
	FillComboBox(GetDlgItem(hW, IDC_STOP_KEY), cfg.StopKey);
	//--------------- OUTPUT -------------------------
	Button_SetCheck(GetDlgItem(hW, IDC_CUSTOM_PATH), cfg.CustomPath);
	Edit_SetText(GetDlgItem(hW, IDC_PATH), cfg.Path);
	BOOL Checked = Button_GetCheck(GetDlgItem(hW, IDC_CUSTOM_PATH));
	EnableWindow(GetDlgItem(hW, IDC_PATH), Checked);
	EnableWindow(GetDlgItem(hW, IDC_BROWSE), Checked);
	//--------------- STATUS -------------------------
	Button_SetCheck(GetDlgItem(hW, IDC_STATUS_TITLEBAR), cfg.StatusTitleBar);
	Button_SetCheck(GetDlgItem(hW, IDC_STATUS_GPU), cfg.StatusGPU);
	Button_SetCheck(GetDlgItem(hW, IDC_STATUS_DRAW), cfg.StatusDraw);
	//--------------- OPTIONS -------------------------
	Button_SetCheck(GetDlgItem(hW, IDC_RECORD_SOUND), cfg.RecordSound);
	Button_SetCheck(GetDlgItem(hW, IDC_COMPRESS), cfg.Compress);
	Button_SetCheck(GetDlgItem(hW, IDC_DEMO_MAKER), cfg.DemoMaker);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////

void OnOK(HWND hW)
{
	HWND hWC;
	int i, Index;
	//--------------- PLUGIN -------------------------
	hWC = GetDlgItem(hW, IDC_GPU_PLUGIN);
	Index = ComboBox_GetCurSel(hWC);
	if(Index<0) { MessageBox(hW, "Must select plugin !", "Error", MB_ICONERROR); return; }
	strcpy(cfg.VideoPlugin, VideoPlugins[Index]);
	//--------------- RECORDING START -------------------------
	if(Button_GetCheck(GetDlgItem(hW, IDC_START1))) cfg.Start = 1;
	if(Button_GetCheck(GetDlgItem(hW, IDC_START2))) cfg.Start = 2;
	if(Button_GetCheck(GetDlgItem(hW, IDC_START3))) cfg.Start = 3;
	cfg.StartKey = KeyList[ComboBox_GetCurSel(GetDlgItem(hW, IDC_START_KEY))].cCode;
	//--------------- RECORDING STOP -------------------------
	if(Button_GetCheck(GetDlgItem(hW, IDC_STOP1))) cfg.Stop = 1;
	if(Button_GetCheck(GetDlgItem(hW, IDC_STOP2))) cfg.Stop = 2;
	if(Button_GetCheck(GetDlgItem(hW, IDC_STOP3))) cfg.Stop = 3;
	cfg.StopKey = KeyList[ComboBox_GetCurSel(GetDlgItem(hW,IDC_STOP_KEY))].cCode;
	//--------------- OUTPUT -------------------------
	cfg.CustomPath = Button_GetCheck(GetDlgItem(hW, IDC_CUSTOM_PATH));
	Edit_GetText(GetDlgItem(hW, IDC_PATH), cfg.Path, sizeof(cfg.Path));
	i = strlen(cfg.Path);
	if(i > 0 && cfg.Path[i-1] != '\\') { cfg.Path[i] = '\\'; cfg.Path[i+1] = 0; }
	//--------------- STATUS -------------------------
	cfg.StatusTitleBar = Button_GetCheck(GetDlgItem(hW, IDC_STATUS_TITLEBAR));
	cfg.StatusGPU = Button_GetCheck(GetDlgItem(hW, IDC_STATUS_GPU));
	cfg.StatusDraw = Button_GetCheck(GetDlgItem(hW, IDC_STATUS_DRAW));
	//--------------- OPTIONS -------------------------
	cfg.RecordSound = Button_GetCheck(GetDlgItem(hW, IDC_RECORD_SOUND));
	cfg.Compress = Button_GetCheck(GetDlgItem(hW, IDC_COMPRESS));
	cfg.DemoMaker = Button_GetCheck(GetDlgItem(hW, IDC_DEMO_MAKER));
	
	SaveConfiguration();
	EndDialog(hW, TRUE);
}

////////////////////////////////////////////////////////////////////////////

void OnTest(HWND hW) 
{
	int Index = ComboBox_GetCurSel(GetDlgItem(hW, IDC_GPU_PLUGIN));
	if(Index >= 0) {
		HMODULE hModule;
		if((hModule = LoadLibrary(VideoPlugins[Index])) != NULL) {
			if(((FARPROC&)REDIRECT_GPUtest = GetProcAddress(hModule, "GPUtest")) != NULL) {
				long ret = REDIRECT_GPUtest();
				if(ret==0)
					MessageBox(hW, "Plugin should be working fine !", "Test", MB_ICONINFORMATION);
				else if(ret<0)
					MessageBox(hW, "Plugin returned an error !", "Test", MB_ICONINFORMATION);
				else
					MessageBox(hW, "Plugin returned a warning !", "Test", MB_ICONINFORMATION);
			}
			FreeLibrary(hModule);
		}
	}
}

////////////////////////////////////////////////////////////////////////////

void OnConfig(HWND hW) 
{
	int Index = ComboBox_GetCurSel(GetDlgItem(hW, IDC_GPU_PLUGIN));
	if(Index >= 0) {
		HMODULE hModule;
		if((hModule = LoadLibrary(VideoPlugins[Index])) != NULL) {
			if(((FARPROC&)REDIRECT_GPUconfigure = GetProcAddress(hModule, "GPUconfigure")) != NULL)
				REDIRECT_GPUconfigure();
			FreeLibrary(hModule);
		}
	}
}

////////////////////////////////////////////////////////////////////////////

void OnAbout(HWND hW) 
{
	int Index = ComboBox_GetCurSel(GetDlgItem(hW, IDC_GPU_PLUGIN));
	if(Index >= 0) {
		HMODULE hModule;
		if((hModule = LoadLibrary(VideoPlugins[Index])) != NULL) {
			if(((FARPROC&)REDIRECT_GPUabout = GetProcAddress(hModule, "GPUabout")) != NULL)
				REDIRECT_GPUabout();
			FreeLibrary(hModule);
		}
	}
}

//==========================================================================
// Load Configuration
//==========================================================================

void LoadConfiguration()
{
	memset(&cfg, 0, sizeof(cfg));
	REG_GetSTRING(REG_GPU_PLUGIN,     cfg.VideoPlugin);
	REG_GetDWORD(REG_START,           cfg.Start);
	REG_GetDWORD(REG_START_KEY,       cfg.StartKey);
	REG_GetDWORD(REG_STOP,            cfg.Stop);
	REG_GetDWORD(REG_STOP_KEY,        cfg.StopKey);
	REG_GetDWORD(REG_CUSTOM_PATH,     cfg.CustomPath);
	REG_GetSTRING(REG_PATH,           cfg.Path);
	REG_GetDWORD(REG_STATUS_TITLEBAR, cfg.StatusTitleBar);
	REG_GetDWORD(REG_STATUS_GPU,      cfg.StatusGPU);
	REG_GetDWORD(REG_STATUS_DRAW,     cfg.StatusDraw);
	REG_GetDWORD(REG_RECORD_SOUND,    cfg.RecordSound);
	REG_GetDWORD(REG_COMPRESS,        cfg.Compress);
	REG_GetDWORD(REG_DEMO_MAKER,      cfg.DemoMaker);

	if(cfg.Start < 1 || cfg.Start > 3) cfg.Start = 1;
	if(cfg.Stop < 1 || cfg.Stop > 3) cfg.Stop = 1;
}

//==========================================================================
// Save Configuration
//==========================================================================

void SaveConfiguration()
{
	REG_SetSTRING(REG_GPU_PLUGIN,     cfg.VideoPlugin);
	REG_SetDWORD(REG_START,           cfg.Start);
	REG_SetDWORD(REG_START_KEY,       cfg.StartKey);
	REG_SetDWORD(REG_STOP,            cfg.Stop);
	REG_SetDWORD(REG_STOP_KEY,        cfg.StopKey);
	REG_SetDWORD(REG_CUSTOM_PATH,     cfg.CustomPath);
	REG_SetSTRING(REG_PATH,           cfg.Path);
	REG_SetDWORD(REG_STATUS_TITLEBAR, cfg.StatusTitleBar);
	REG_SetDWORD(REG_STATUS_GPU,      cfg.StatusGPU);
	REG_SetDWORD(REG_STATUS_DRAW,     cfg.StatusDraw);
	REG_SetDWORD(REG_RECORD_SOUND,    cfg.RecordSound);
	REG_SetDWORD(REG_COMPRESS,        cfg.Compress);
	REG_SetDWORD(REG_DEMO_MAKER,      cfg.DemoMaker);
}
/***************************************************************************
      File Name:	Config.cpp
   File Created:	Thursday, October 9th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
#include "externals.h"
#include "config.h"
#include "folder.h"
#include "reg.h"

CFG cfg;

#define MAX_PLUGINS 100
char SoundPlugins[MAX_PATH][MAX_PLUGINS];

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
				case IDC_SPU_TEST:		OnTest(hW);		return TRUE;
				case IDC_SPU_CONFIG:	OnConfig(hW);	return TRUE;
				case IDC_SPU_ABOUT:		OnAbout(hW);	return TRUE;
				case IDCANCEL:		EndDialog(hW,FALSE);return TRUE;
				case IDOK:			OnOK(hW);			return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////

void SearchPlugins(HWND hWC, char *SearchPath, int &SelectedSoundPlugin)
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
							if(REDIRECT_PSEgetLibType()==4)
								if(strcmp(REDIRECT_PSEgetLibName(), LIBRARY_NAME)) {
									unsigned long version = REDIRECT_PSEgetLibVersion();
									char buffer[MAX_PATH];
									int Index;
									sprintf(buffer,"%s %d.%d", REDIRECT_PSEgetLibName(),(version>>8)&0xff,version&0xff);
									if(ComboBox_GetCount(hWC) < MAX_PLUGINS) {
										ComboBox_AddString(hWC, buffer);
										Index = ComboBox_GetCount(hWC) - 1;
										sprintf(SoundPlugins[Index], "%s%s", SearchPath, FindData.cFileName);
										if(stricmp(SoundPlugins[Index], cfg.SoundPlugin)==0) SelectedSoundPlugin = Index;
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
	hWC = GetDlgItem(hW, IDC_SPU_PLUGIN); LastSel = -1;
	AppPath(buffer, sizeof(buffer));
	ComboBox_ResetContent(hWC);
	SearchPlugins(hWC, buffer, LastSel);
	strcat(buffer,"plugins\\");
	SearchPlugins(hWC, buffer, LastSel);
	if(ComboBox_GetCount(hWC)>0) {
		if(LastSel >= 0 && LastSel < ComboBox_GetCount(hWC))
			ComboBox_SetCurSel(hWC, LastSel);
		else
			ComboBox_SetCurSel(hWC, 0);
	} else
		EnableWindow(GetDlgItem(hW,IDOK),FALSE);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////

void OnOK(HWND hW)
{
	HWND hWC;
	int Index;
	//--------------- PLUGIN -------------------------
	hWC = GetDlgItem(hW, IDC_SPU_PLUGIN);
	Index = ComboBox_GetCurSel(hWC);
	if(Index<0) { MessageBox(hW, "Must select plugin !", "Error", MB_ICONERROR); return; }
	strcpy(cfg.SoundPlugin, SoundPlugins[Index]);

	SaveConfiguration();
	EndDialog(hW, TRUE);
}

////////////////////////////////////////////////////////////////////////////

void OnTest(HWND hW) 
{
	int Index = ComboBox_GetCurSel(GetDlgItem(hW, IDC_SPU_PLUGIN));
	if(Index >= 0) {
		HMODULE hModule;
		if((hModule = LoadLibrary(SoundPlugins[Index])) != NULL) {
			if(((FARPROC&)REDIRECT_SPUtest = GetProcAddress(hModule, "SPUtest")) != NULL) {
				long ret = REDIRECT_SPUtest();
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
	int Index = ComboBox_GetCurSel(GetDlgItem(hW, IDC_SPU_PLUGIN));
	if(Index >= 0) {
		HMODULE hModule;
		if((hModule = LoadLibrary(SoundPlugins[Index])) != NULL) {
			if(((FARPROC&)REDIRECT_SPUconfigure = GetProcAddress(hModule, "SPUconfigure")) != NULL)
				REDIRECT_SPUconfigure();
			FreeLibrary(hModule);
		}
	}
}

////////////////////////////////////////////////////////////////////////////

void OnAbout(HWND hW) 
{
	int Index = ComboBox_GetCurSel(GetDlgItem(hW, IDC_SPU_PLUGIN));
	if(Index >= 0) {
		HMODULE hModule;
		if((hModule = LoadLibrary(SoundPlugins[Index])) != NULL) {
			if(((FARPROC&)REDIRECT_SPUabout = GetProcAddress(hModule, "SPUabout")) != NULL)
				REDIRECT_SPUabout();
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
	REG_GetSTRING(REG_SPU_PLUGIN,     cfg.SoundPlugin);
}

//==========================================================================
// Save Configuration
//==========================================================================

void SaveConfiguration()
{
	REG_SetSTRING(REG_SPU_PLUGIN,     cfg.SoundPlugin);
}
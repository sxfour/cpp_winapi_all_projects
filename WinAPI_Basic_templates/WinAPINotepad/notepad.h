#pragma once
#include "header.h"

class NotePadDlg
{
public:
	NotePadDlg(void);

	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static NotePadDlg* ptr;

	BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);

	void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void OnClose(HWND hwnd);
	void OnSize(HWND hwnd, UINT state, int cx, int cy);
	void OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu);
	void OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);

	HWND hDialog, hStatus, hEdit;
	BOOL bShowStatusBar, bIsOpenF, SAVE;
	HMENU hMenuRU, hMenuEN;

	TCHAR FullPathTo[MAX_PATH];

	void openFile(HWND hwnd);
	void saveFile(HWND hwnd, int i);
};

#include "notepad.h"

NotePadDlg* NotePadDlg::ptr = nullptr;

NotePadDlg::NotePadDlg(void) {
	ptr = this;
	bShowStatusBar = TRUE;
	bIsOpenF = FALSE;
}

void NotePadDlg::OnClose(HWND hWnd) {
	if (::SendMessage(hEdit, EM_GETMODIFY, 0, 0) && !SAVE) {
		if (::MessageBox(hWnd, L"Вы хотите сохранить файл?", L"Текстовый редактор на WinAPI", MB_YESNO) == 6) {
			saveFile(hWnd, 0);
			::SendMessage(hEdit, WM_SETTEXT, 0, 0);
		}
		else {
			::EndDialog(hWnd, 0);
		}
	}
	else {
		::EndDialog(hWnd, 0);
	}
}

BOOL NotePadDlg::OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {
	hDialog = hWnd;
	hEdit = ::GetDlgItem(hDialog, IDC_EDIT1);
	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_TOOLTIPS | SBARS_SIZEGRIP, 0, hDialog, WM_USER);
	hMenuRU = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	hMenuEN = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2));

	::SetMenu(hDialog, hMenuRU);
	static bool isClearEdit = TRUE, isSave = FALSE;
	SAVE = FALSE;

	return TRUE;
}

void NotePadDlg::OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {
	switch (id) {
	case ID_NEW:
		if (::SendMessage(hEdit, EM_GETMODIFY, 0, 0) && !SAVE) {
			if (::MessageBox(hWnd, L"ID_NEW", L"Notepad", MB_YESNO) == 6) {
				saveFile(hWnd, 0);
				::SendMessage(hEdit, WM_SETTEXT, 0, 0);
			}
			else {
				::SendMessage(hEdit, WM_SETTEXT, 0, 0);
			}
		}
		break;

	case ID_OPEN:
		if (::SendMessage(hEdit, EM_GETMODIFY, 0, 0) && !SAVE) {
			if (::MessageBox(hWnd, L"ID_OPEN", L"Notepad", MB_YESNO) == 6) {
				saveFile(hWnd, 0);
				::SendMessage(hEdit, WM_SETTEXT, 0, 0);
				openFile(hWnd);
				bIsOpenF = TRUE;
			}
			else {
				openFile(hWnd);
				bIsOpenF = TRUE;
			}
		}
		else {
			openFile(hWnd);
			bIsOpenF = TRUE;
		}
		break;

	case ID_SAVE:
		if (::SendMessage(hEdit, EM_GETMODIFY, 0, 0) && SAVE) {
			saveFile(hWnd, 1);
		}
		else {
			saveFile(hWnd, 0);
		}
		break;

	case ID_SAVEAS:
		saveFile(hWnd, 0);
		break;

	case ID_RU:
		::SetMenu(hDialog, hMenuRU);
		break;

	case ID_EN:
		::SetMenu(hDialog, hMenuEN);
		break;

	case ID_UNDO:
		::SendMessage(hEdit, WM_UNDO, 0, 0);
		break;

	case ID_CUT:
		::SendMessage(hEdit, WM_CUT, 0, 0);
		break;

	case ID_COPY:
		::SendMessage(hEdit, WM_COPY, 0, 0);
		break;

	case ID_PASTE:
		::SendMessage(hEdit, WM_PASTE, 0, 0);
		break;

	case ID_DEL:
		::SendMessage(hEdit, WM_CLEAR, 0, 0);
		break;

	case ID_SELECTALL:
		::SendMessage(hEdit, EM_SETSEL, 0, 0);
		break;

	case ID_CLOSE:
		OnClose(hDialog);
		break;

	case ID_STATUSBAR:
		if (bShowStatusBar) {
			HMENU hMenu = ::GetMenu(hDialog);
			CheckMenuItem(hMenu, ID_STATUSBAR, MF_BYCOMMAND | MF_UNCHECKED);
			::ShowWindow(hStatus, SW_HIDE);
		}
		else {
			HMENU hMenu = ::GetMenu(hDialog);
			CheckMenuItem(hMenu, ID_STATUSBAR, MF_BYCOMMAND | MF_CHECKED);
			::ShowWindow(hStatus, SW_SHOW);
		}
		bShowStatusBar = !bShowStatusBar;
	}
}

void NotePadDlg::OnSize(HWND hWnd, UINT state, int cx, int cy) {
	RECT rect1, rect2;
	::GetClientRect(hDialog, &rect1);
	::SendMessage(hStatus, SB_GETRECT, 0, (LPARAM)&rect2);
	::MoveWindow(hEdit, rect1.left, rect1.top, rect1.right, rect1.bottom - (rect2.bottom - rect2.top), 1);
	::SendMessage(hStatus, WM_SIZE, 0, 0);
}

void NotePadDlg::OnInitMenuPopup(HWND hWnd, HMENU hMenu, UINT item, BOOL fSystemMenu) {
	if (item == 0) {
		DWORD dwPosition = ::SendMessage(hEdit, EM_GETSEL, 0, 0);
		WORD wBeginPosition = LOWORD(dwPosition);
		WORD wEndPosition = HIWORD(dwPosition);

		if (wEndPosition != wBeginPosition) {
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_ENABLED);
		}
		else {
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, ID_DEL, MF_BYCOMMAND | MF_GRAYED);
		}

		if (IsClipboardFormatAvailable(CF_TEXT)) {
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_ENABLED);
		}
		else {
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_GRAYED);
		}
		if (::SendMessage(hEdit, EM_CANUNDO, 0, 0)) {
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_ENABLED);
		}
		else {
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_GRAYED);
		}

		int length = ::SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);

		if (length != wEndPosition - wBeginPosition) {
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_ENABLED);
		}
		else {
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_GRAYED);
		}
	}
}

void NotePadDlg::OnMenuSelect(HWND hWnd, HMENU hMenu, int item, HMENU hMenuPopup, UINT flags) {
	if (flags & MF_POPUP) {
		::SendMessage(hStatus, SB_SETTEXT, 0, 0);
	}
	else {
		TCHAR buff[200];
		HINSTANCE hInstance = GetModuleHandle(NULL);
		LoadString(hInstance, item, buff, 200);
		::SendMessage(hStatus, SB_SETTEXT, 0, LPARAM(buff));
	}
}

BOOL CALLBACK NotePadDlg::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hWnd, WM_CLOSE, ptr->OnClose);
		HANDLE_MSG(hWnd, WM_INITDIALOG, ptr->OnInitDialog);
		HANDLE_MSG(hWnd, WM_COMMAND, ptr->OnCommand);
		HANDLE_MSG(hWnd, WM_SIZE, ptr->OnSize);
		HANDLE_MSG(hWnd, WM_INITMENUPOPUP, ptr->OnInitMenuPopup);
		HANDLE_MSG(hWnd, WM_MENUSELECT, ptr->OnMenuSelect);
	default:
		break;
	}

	return FALSE;
}

void NotePadDlg::saveFile(HWND hwnd, int i)
{
	if (i) {

		TCHAR str[300] = TEXT("Open the file:\n");
		_tcscat_s(str, FullPathTo);

		std::wofstream fin;
		fin.open(FullPathTo);
		fin.imbue(std::locale(""));
		TCHAR text[65536];
		::GetWindowText(hEdit, text, 200);
		if (!fin.is_open())
		{
			std::cerr << "File not open" << std::endl;
			return;
		}
		fin << text;
		fin.close();

	}
	else {
		TCHAR FullPath[MAX_PATH] = { 0 };
		OPENFILENAME open = { sizeof(OPENFILENAME) };
		open.hwndOwner = hwnd;
		open.lpstrFilter = TEXT("Text Files(*.txt)\0*.txt\0");
		open.lpstrFile = FullPath;
		open.nMaxFile = MAX_PATH;
		open.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
		open.lpstrDefExt = TEXT("txt");
		if (GetSaveFileName(&open)) {
			SAVE = TRUE;
			TCHAR str[300] = TEXT("Open the file:\n");
			_tcscat_s(str, FullPath);
			wsprintf(FullPathTo, FullPath);
		}

		std::wofstream fin;
		fin.open(FullPathTo);
		fin.imbue(std::locale(""));
		TCHAR text[65536] = { 0 };
		::GetWindowText(hEdit, text, 200);

		if (!fin.is_open()) {
			std::cerr << "File not open" << std::endl;
			return;
		}
		fin << text;
		fin.close();
	}
}

void NotePadDlg::openFile(HWND hwnd) {
	TCHAR FullPath[MAX_PATH] = { 0 };
	OPENFILENAME open = { sizeof(OPENFILENAME) };

	open.hwndOwner = hwnd;
	open.lpstrFilter = TEXT("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0");
	open.lpstrFile = FullPath;
	open.nMaxFile = MAX_PATH;
	open.lpstrInitialDir = TEXT("C:\\");
	open.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	if (GetOpenFileName(&open)) {
		TCHAR str[300] = TEXT("Open the file:\n");
		_tcscat_s(str, FullPath);
		wsprintf(FullPathTo, FullPath);
	}

	std::wifstream fon;

	TCHAR text[100] = { 0 };
	TCHAR text2[65536] = { 0 };

	fon.open(FullPathTo);
	fon.imbue(std::locale(""));

	if (!fon.is_open()) {
		std::cerr << "File not open" << std::endl;

		return;
	}

	while (1) {
		fon.getline(text, 200);

		if (!lstrcmp(text, TEXT("\0")))
			break;

		_tcscat_s(text2, text);
		_tcscat_s(text2, TEXT("\r\n"));
	}

	::SetWindowText(hEdit, text2);

	fon.close();
}
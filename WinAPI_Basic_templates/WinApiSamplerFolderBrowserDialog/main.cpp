#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <map>
#include <string>
#include <Windows.h>
#include <CommCtrl.h>
#include <ShlObj.h>

using namespace std;
using namespace std::literals;

HWND window = nullptr;
HWND button1 = nullptr;
HWND staticText1 = nullptr;
WNDPROC defWndProc = nullptr;
wstring selectedPath;

LRESULT OnWindowClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PostQuitMessage(0);

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

int CALLBACK OnBrowserCallback(HWND hWnd, UINT message, LPARAM lParam, LPARAM data) {
	if (message == BFFM_INITIALIZED && !wstring(reinterpret_cast<wchar_t*>(data)).empty()) {
		SendMessage(hWnd, BFFM_SETSELECTION, 1, data);
	}

	return 0;
}

LRESULT OnButtonClick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BROWSEINFO browserInfo = { 0 };
	browserInfo.hwndOwner = hWnd;
	PIDLIST_ABSOLUTE pidlRoot;
	SHGetSpecialFolderLocation(hWnd, CSIDL_DESKTOP, &pidlRoot);
	browserInfo.lpfn = OnBrowserCallback;
	browserInfo.lParam = reinterpret_cast<LPARAM>(selectedPath.c_str());
	browserInfo.lpszTitle = L"Select a folder";

	browserInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_NEWDIALOGSTYLE;

	PCIDLIST_ABSOLUTE result = SHBrowseForFolder(&browserInfo);
	if (result) {
		wchar_t path[MAX_PATH];
		SHGetPathFromIDList(result, path);
		selectedPath = path;
		SendMessage(staticText1, WM_SETTEXT, 0, reinterpret_cast<LPARAM>((L"Path = "s + selectedPath).c_str()));
	}

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_CLOSE && hWnd == window) {
		return OnWindowClose(hWnd, message, wParam, lParam);
	}
	if (message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED && reinterpret_cast<HWND>(lParam) == button1) {
		return OnButtonClick(hWnd, message, wParam, lParam);
	}

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

int main() {
	window = CreateWindowEx(0, WC_DIALOG, L"FolderBrowserDialog example", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, nullptr, nullptr, nullptr, nullptr);
	button1 = CreateWindowEx(0, WC_BUTTON, L"Folder...", WS_CHILD | WS_VISIBLE, 10, 10, 80, 25, window, nullptr, nullptr, nullptr);
	staticText1 = CreateWindowEx(0, WC_STATIC, L"Path = ", WS_CHILD | WS_VISIBLE, 10, 45, 274, 215, window, nullptr, nullptr, nullptr);

	defWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));
	ShowWindow(window, SW_SHOW);

	MSG message = {0};
	while (GetMessage(&message, nullptr, 0, 0)) {
		DispatchMessage(&message);
	}

	return 0;
}

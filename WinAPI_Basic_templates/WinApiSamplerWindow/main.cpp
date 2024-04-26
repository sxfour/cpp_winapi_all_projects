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
WNDPROC defWndProc = nullptr;

LRESULT OnWindowClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (MessageBox(window, L"Вы уверены что хотите выйти?", L"Закрыть окно", MB_ICONQUESTION | MB_YESNO) == IDYES) {
		PostQuitMessage(0);
	}

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

LRESULT OnButtonClick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	SendMessage(window, WM_CLOSE, 0, 0);

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
	window = CreateWindowEx(0, WC_DIALOG, L"Окно", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, nullptr, nullptr, nullptr, nullptr);
	button1 = CreateWindowEx(0, WC_BUTTON, L"Выход", WS_CHILD | WS_VISIBLE, 10, 10, 75, 25, window, nullptr, nullptr, nullptr);

	defWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
	ShowWindow(window, SW_SHOW);

	MSG message = { 0 };
	while (GetMessage(&message, nullptr, 0, 0)) {
		DispatchMessage(&message);
	}

	return 0;
}
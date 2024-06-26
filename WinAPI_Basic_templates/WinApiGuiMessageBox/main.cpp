#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <CommCtrl.h>

HWND window = nullptr;
HWND button = nullptr;
WNDPROC defWndProc = nullptr;

LRESULT OnWindowClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PostQuitMessage(0);

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

LRESULT OnButtonClick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	MessageBox(window, L"Hello, Wolrd!", L"", MB_OK);

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_CLOSE && hWnd == window) {
		return OnWindowClose(hWnd, message, wParam, lParam);
	}
	if (message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED && (HWND)lParam == button) {
		return OnButtonClick(hWnd, message, wParam, lParam);
	}

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

int main() {
	window = CreateWindowEx(0, WC_DIALOG, L"Hello World (MessageBox)", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 316, 340, nullptr, nullptr, nullptr, nullptr);
	button = CreateWindowEx(0, WC_BUTTON, L"&Click me", WS_CHILD | WS_VISIBLE, 10, 10, 75, 25, window, nullptr, nullptr, nullptr);

	defWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

	ShowWindow(window, SW_SHOW);

	MSG message = { 0 };
	while (GetMessage(&message, nullptr, 0, 0)) {
		DispatchMessage(&message);
	}

	return (int)message.wParam;
}
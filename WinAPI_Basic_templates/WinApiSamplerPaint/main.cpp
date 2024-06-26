#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <CommCtrl.h>

HWND window = nullptr;
WNDPROC defWndProc = nullptr;

LRESULT OnWindowClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PostQuitMessage(0);

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

LRESULT OnWindowResize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT result = CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
	RECT rect;
	GetClientRect(hWnd, &rect);
	InvalidateRect(hWnd, &rect, false);

	return result;
}

LRESULT OnWindowPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT paintStruct;
	HDC hdc = BeginPaint(hWnd, &paintStruct);
	HBRUSH brush = CreateSolidBrush(RGB(0x00, 0x20, 0x10));
	HFONT font = CreateFont(int(-34 / 72.0f * GetDeviceCaps(hdc, LOGPIXELSY)), 0, 0, 0, FW_BOLD, true, false, false, 0, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

	FillRect(hdc, &paintStruct.rcPaint, brush);
	SetTextColor(hdc, RGB(0x2D, 0xFF, 0x7F));
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, font);
	DrawText(hdc, L"Hello, World!", -1, &paintStruct.rcPaint, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	EndPaint(hWnd, &paintStruct);

	DeleteObject(brush);
	DeleteObject(font);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_CLOSE && hWnd == window) {
		return OnWindowClose(hWnd, message, wParam, lParam);
	}
	if (message == WM_SIZE && hWnd == window) {
		return OnWindowResize(hWnd, message, wParam, lParam);
	}
	if (message == WM_PAINT && hWnd == window) {
		return OnWindowPaint(hWnd, message, wParam, lParam);
	}

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

int main() {
	window = CreateWindowEx(0, WC_DIALOG, L"Hello world (Paint)", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 316, 340, nullptr, nullptr, nullptr, nullptr);
	defWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

	ShowWindow(window, SW_SHOW);

	MSG message = { 0 };
	while (GetMessage(&message, nullptr, 0, 0)) {
		DispatchMessage(&message);
	}

	return (int)message.wParam;
}
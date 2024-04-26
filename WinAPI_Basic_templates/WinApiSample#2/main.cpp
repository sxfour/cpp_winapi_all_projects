#include <Windows.h>

// �������� ������� ��������� ��������� � ���������������� ���������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR mainMessage[] = L"�����-�� �����!";

int WINAPI WinMain(	HINSTANCE hInst,		// ���������� ��������� ����������
					HINSTANCE hPrevInst,    // 
					LPSTR lpCmdLine,		//
					int nCmdShow			// ����� ����������� ����
) {
	TCHAR szClassName[] = L"��� �����";		// ������ � ������ ������
	HWND hMainWnd{};						// ������ ���������� �������� ����
	MSG msg{};								// ������ ��������� ���������, ��� ��������� ���������
	WNDCLASSEX wc{};						// ���������, ��� ��������� � ������ ������ WNDCLASSEX

	wc.cbSize			= sizeof(wc);							// ������ ��������� � ������
	wc.style			= CS_HREDRAW | CS_VREDRAW;				// ����� ������ ����
	wc.lpfnWndProc		= WndProc;								// ��������� �� ���������������� �������
	wc.lpszMenuName		= NULL;									// ��������� �� ��� ���� (��� ��� ���)
	wc.lpszClassName	= szClassName;							// ��������� �� ��� ������
	wc.cbWndExtra		= NULL;									// ����� ������������� ������ � ����� ���������
	wc.cbClsExtra		= NULL;									// ����� ������������� ������ ��� �������� ���������� ����������
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// ���������� �����������
	wc.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);			// ���������� ��������� ����������� � ����
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// ���������� �������
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	// ���������� ����� ��� �������� ����
	wc.hInstance		= hInst;								// ��������� �� ������, ���������� ��� ����, ������������ ��� ������

	if (!RegisterClassEx(&wc)) {
		// � ������ ���������� ����������� ������
		MessageBox(NULL, L"�� ���������� ���������������� �����", L"������", MB_OK);

		return NULL; // ����� �� WinMain
	}

	hMainWnd = CreateWindow( szClassName,						// ��� ������
							 L"����������� ������� ���������",	// ��� ���� 
							 WS_OVERLAPPEDWINDOW | WS_VSCROLL,	// ����� ����������� ����
							 CW_USEDEFAULT,						// ������� ���� �� ��� �
							 NULL,								// ������� ���� �� ��� � (���� ������ � � �� ������ �� ����)
							 CW_USEDEFAULT,						// ������ ����
							 NULL,								// ������ ����
							 (HWND)NULL,						// ���������� ��� ����
							 NULL,								// ���������� ����
							 HINSTANCE(hInst),					// ���������� ���������� ����
							 NULL);								//

	if (!hMainWnd) {
		// � ������ ������������� �������� ����
		MessageBox(NULL, L"�� ���������� ������� ����", L"������!", MB_OK);

		return NULL;
	}
	ShowWindow(hMainWnd, nCmdShow);		// ����������� ����
	UpdateWindow(hMainWnd);				// ���������� ����

	// ��������� ��������� �� �������, ��������� �-����� ��
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);			// ������������� ���������
		DispatchMessage(&msg);			// �������� ��������� ������� ��
	}

	return msg.wParam;					// ����� �� ����������
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC;								// ���������� ���������� ������ 
	PAINTSTRUCT ps;							// ��������� �������� ���������� � ���������� ��� (����, ������ � ��)
	RECT rect;								// ����������� ���������
	COLORREF colorText = RGB(255, 0, 0);    // ���� ������ 

	switch (uMsg) {
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);													// �������������� �������� ����������
		GetClientRect(hWnd, &rect);														// �������� ������ � ������ ������� ��� ���������
		SetTextColor(hDC, colorText);													// ������������� ���� ������������ ����������
		DrawText(hDC, mainMessage, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);	// ������ �����
		EndPaint(hWnd, &ps);															// ����������� ��������

		break;
	case WM_DESTROY:			// ���� ���� �������
		PostQuitMessage(NULL);	// ���������� WinMain() ��������� WM_QUIT

		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);	// ���� ������� ����
	}

	return NULL;
}
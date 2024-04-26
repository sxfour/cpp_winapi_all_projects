#include <windows.h>

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow) {
	int result = MessageBox(NULL, L"WINAPI", L"Some option", MB_ICONQUESTION | MB_YESNO);
	switch (result) {
	case IDYES: MessageBox(NULL, L"Nice!", L"+", MB_OK | MB_ICONASTERISK);
		break;
	case IDNO: MessageBox(NULL, L"Dont worry", L"+", MB_OK | MB_ICONSTOP);
		break;
	}

	return NULL;
}
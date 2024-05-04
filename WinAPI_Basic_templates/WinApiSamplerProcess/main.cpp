#include <iostream>
#include <string>
#include <Windows.h>

int main() {
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));

	std::wstring name = L"openssh\\ssh-keygen.exe";
	if (CreateProcess(nullptr, (wchar_t*)name.c_str(), nullptr, nullptr, false, 0, nullptr, nullptr, &startupInfo, &processInfo) == 0) {
		std::cout << "CreateProcess failed " << GetLastError() << std::endl;
		return 1;
	}

	WaitForSingleObject(processInfo.hProcess, INFINITE);
}
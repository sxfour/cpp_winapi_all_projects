#include <windows.h>
#include <Shlobj.h>
#include <tchar.h>
#include <Commctrl.h>

#include <iostream>
#include <string>

#pragma comment(lib, "comctl32.lib")

bool cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage = CP_ACP);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    OPENFILENAME ofn;
    wchar_t szFile[260];
    HWND hwnd;
    HANDLE hf;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Архив (*.zip)\0*.zip\0*.zip\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        std::string s("\?");

        cvtLPW2stdstring(s, ofn.lpstrFile, CP_ACP);

        std::cout << s;
    }
    else if (GetOpenFileName(&ofn) == 0) {

     }

    return 0;
}

bool cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage) {
    bool res = false;
    char* p = 0;
    int bsz;

    bsz = WideCharToMultiByte(codepage,
        0,
        pw, -1,
        0, 0,
        0, 0);
    if (bsz > 0) {
        p = new char[bsz];
        int rc = WideCharToMultiByte(codepage,
            0,
            pw, -1,
            p, bsz,
            0, 0);
        if (rc != 0) {
            p[bsz - 1] = 0;
            s = p;
            res = true;
        }
    }
    delete[] p;

    return res;
}

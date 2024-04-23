#include "FileOps.h"

#include <string>
#include <windows.h>
#include <commdlg.h> // Common dialog boxes functions
using namespace std;

std::string OpenFileDialogBox()
{

    wchar_t filename[MAX_PATH] = { 0 }; // to store the file name

    OPENFILENAME ofn; // common dialog box structure
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL; // If you have a window handle, pass it here
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) == TRUE) {
        wstring wfilename(filename);
        // your new String
        return string(wfilename.begin(), wfilename.end());
    }
    return ""; // Return empty if no file was selected
}

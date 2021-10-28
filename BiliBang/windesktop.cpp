#include "windesktop.h"

bool enumUserWindowsCB(HWND hwnd, LPARAM lParam)
{
    long wflags = GetWindowLong(hwnd, GWL_STYLE);
    if (!(wflags & WS_VISIBLE)) {
        return true;
    };

    HWND sndWnd;
    if (!(sndWnd=FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL))) {
        return true;
    }

    HWND targetWnd;
    if (!(targetWnd=FindWindowEx(sndWnd, NULL, L"SysListView32", L"FolderView"))) {
        return true;
    }

//    HWND wpeWnd;
//    if (!(wpeWnd=FindWindowEx(hwnd, NULL, L"WPEAppIntermediateWorker", NULL))) {
//        return true;
//    }

    HWND* resultHwnd = (HWND*)lParam;
    *resultHwnd = targetWnd;
    return false;
}

void setParentDesktop(QWidget *pWidget)
{
    HWND resultHwnd = NULL;
    EnumWindows((WNDENUMPROC)enumUserWindowsCB, (LPARAM)&resultHwnd);

    if (resultHwnd) {
        SetParent((HWND)pWidget->winId(), resultHwnd);
    }
}

QString GetProductVer(const QString pFilePath)
{
    std::wstring wstrfilepath = pFilePath.toStdWString().c_str();
    DWORD dwHanle = 0;
    DWORD dwSize = GetFileVersionInfoSize(wstrfilepath.c_str(), &dwHanle);
    if (0 == dwSize) {
        return "";
    }

    char *buf = new char[dwSize + 1];
    memset(buf, 0, dwSize + 1);

    DWORD dwRet = GetFileVersionInfo(wstrfilepath.c_str(), dwHanle, dwSize, buf);
    if (0 == dwRet) {
        delete[] buf;
        return "";
    }

    LPVOID lpBuf = NULL;
    UINT uLen = 0;
    if (0 == VerQueryValue(buf, TEXT("\\"), &lpBuf, &uLen)) {
        delete[] buf;
        return "";
    }

    VS_FIXEDFILEINFO *fileinfo = (VS_FIXEDFILEINFO *)lpBuf;
    WORD d1 = HIWORD(fileinfo->dwFileVersionMS);
    WORD d2 = LOWORD(fileinfo->dwFileVersionMS);
    WORD d3 = HIWORD(fileinfo->dwFileVersionLS);
    WORD d4 = LOWORD(fileinfo->dwFileVersionLS);
    delete[] buf;
    return QString("%1.%2.%3.%4").arg(d1).arg(d2).arg(d3).arg(d4);
}

bool powerOff()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    //获取进程标志
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
         return false;

    //获取关机特权的LUID
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,    &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    //获取这个进程的关机特权
    AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    if (GetLastError() != ERROR_SUCCESS) return false;

    // 强制关闭计算机
    if ( !ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
          return false;
    return true;
}

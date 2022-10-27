/*
===============================================================================
1. VTune的安裝目录下有include、lib32、lib64几个目录，将它们将到搜索路径中去；
2. 链接库的输入里加上：libittnotify.lib；
2. 包含头文件："ittnotify.h"；
3. To be able to see user tasks in your results, enable the Analyze user tasks checkbox in analysis settings；
4. API的用法可参考帮助文档：[User's Guide]、[API Support]；
===============================================================================
*/

#include <windows.h>
#include <tchar.h>

//#define INTEL_NO_ITTNOTIFY_API
#include "ittnotify.h"


static TCHAR const      szClassName[] = _T("TestVTuneWindowClass");


// Create a domain that is visible globally: we will use it in our example.
__itt_domain* domain = __itt_domain_create(_T("Example.Domain.Global"));

// Create string handles which associates with the "main" task.
__itt_string_handle* handle_main = __itt_string_handle_create(_T("WinMain"));
__itt_string_handle* handle_update = __itt_string_handle_create(_T("OnUpdate"));


static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


static void RegisterWindowClass()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_WINLOGO);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szClassName;
    wcex.hIconSm = LoadIcon(hInstance, (LPCTSTR)IDI_WINLOGO);;
    RegisterClassEx(&wcex);
}


static void CenterWindow(HWND hwnd)
{
    RECT desktop_rect, wnd_rect;
    GetWindowRect(GetDesktopWindow(), &desktop_rect);
    GetWindowRect(hwnd, &wnd_rect);
    LONG wnd_width = wnd_rect.right - wnd_rect.left;
    LONG wnd_height = wnd_rect.bottom - wnd_rect.top;
    SetWindowPos(hwnd, NULL, (desktop_rect.right - wnd_width) / 2, (desktop_rect.bottom - wnd_height) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}


static void OnUpdate()
{
    __itt_task_begin(domain, __itt_null, __itt_null, handle_update);
    __itt_task_end(domain);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // Create a task associated with the "main" routine.
    __itt_task_begin(domain, __itt_null, __itt_null, handle_main);

    // 注册窗口类
    RegisterWindowClass();

    // 根据窗口的客户区大小计算窗口大小
    int const clientWidth = 800;
    int const clientHeight = 600;
    RECT rect = { 0, 0, clientWidth, clientHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    int const screenWidth = rect.right - rect.left;
    int const screenHeight = rect.bottom - rect.top;

    // 创建窗口
    DWORD const dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    HWND hwnd = CreateWindow(szClassName, _T("TestVTune"), dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);
    if (NULL == hwnd) { abort(); }

    ShowWindow(hwnd, SW_SHOW);

    CenterWindow(hwnd);

    LARGE_INTEGER lastFrameTime;
    QueryPerformanceCounter(&lastFrameTime);

    // 消息循环
    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (WM_QUIT == msg.message) { break; }

        OnUpdate();
    }

    // Mark the end of the main task
    __itt_task_end(domain);

    return int(msg.wParam);
}

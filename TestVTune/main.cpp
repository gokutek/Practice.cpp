#include <windows.h>
#include <tchar.h>

static TCHAR const      szClassName[] = _T("TestVTuneWindowClass");


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
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
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

    return int(msg.wParam);
}

#include <Windows.h>
#include <iostream>
#include <sstream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
    )
{
    // windows窗口类
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = "D3D11Application";
    wc.hIconSm = nullptr;
    // 注册窗口
    RegisterClassEx(&wc);

    // 创建窗口类
    HWND hWnd = CreateWindowEx(
        0,
        "D3D11Application",
        "Init Window",
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        200, 200, 1024, 768,
        nullptr,
        nullptr,
        hInstance,
        nullptr
        );

    ShowWindow(hWnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static std::string title;
    switch (msg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            if (wParam == 'F') {
                SetWindowText(hWnd, "Run!!!!");
            }
            break;
        case WM_KEYUP:
            if (wParam == 'F') {
                SetWindowText(hWnd, "I am Back!!!");
            }
            break;
        case WM_CHAR:
            title.push_back((char)wParam);
            SetWindowText(hWnd, title.c_str());
            break;
        case WM_LBUTTONDOWN:
            POINTS pt = MAKEPOINTS(lParam);
            std::ostringstream oss;
            // 显示点击位置
            oss << "(" << pt.x << ", " << pt.y << ")";
            SetWindowText(hWnd, oss.str().c_str());
            break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
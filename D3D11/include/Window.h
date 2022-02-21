#pragma once

// 自定义的头文件
#include "D3D11Win.h"

class Window
{
private:
    // 单例模式，确保一个窗口只有一个窗口类注册
    class WindowClass
    {
    public:
        static const char* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;

    private:
        // 创建窗口类
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        static constexpr const char* wndClassName = "Driect3D11 Engine Window";
        static WindowClass wndClass;
        HINSTANCE  hInst;
    };

public:
    Window(int width, int height, const char* name) noexcept;
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

private:
    // 通过静态函数调用类成员函数，直接在成员函数中传递WINAPI函数（CALLBACK）是不能正常工作的
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
    int width;
    int height;
    HWND hWnd;
};

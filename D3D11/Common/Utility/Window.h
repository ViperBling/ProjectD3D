#pragma once

#include <optional>
#include <memory>

// 自定义的头文件
#include "D3D11Win.h"
#include "D3D11Exception.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "Graphics/D3D11Graphics.h"

class Window
{
private:
    class Exception : public D3D11Exception
    {
    public:
        Exception( int line, const char* file, HRESULT hr ) noexcept;
        const char* what() const noexcept override;
        virtual const char* GetType() const noexcept;
        static std::string TranslateErrorCode( HRESULT hr ) noexcept;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;

    private:
        HRESULT hr;
    };
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
    Window(int width, int height, const char* name);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    // 设置窗口标题
    void SetTitle(const std::string& title);

    static std::optional<int> ProcessMessages();

    D3D11Graphics& Gfx();

private:
    // 通过静态函数调用类成员函数，直接在成员函数中传递WINAPI函数（CALLBACK）是不能正常工作的
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
    Keyboard kbd;
    Mouse mouse;

private:
    int width;
    int height;
    HWND hWnd;
    std::unique_ptr<D3D11Graphics> pGfx;
};

// 定义宏方便调用
#define D3D11WND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__,hr)
#define D3D11WND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__,GetLastError())
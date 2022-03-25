#include <sstream>
#include "Window.h"
#include "resource.h"
#include "Utility/Marcos/WindowsThrowMacros.h"

Window::WindowClass Window::WindowClass::wndClass;

const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

Window::WindowClass::WindowClass() noexcept  :
    hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(
        GetInstance(),
        MAKEINTRESOURCE(IDI_APPICON),
        IMAGE_ICON, 32, 32, 0
        ));
    wc.hIconSm = static_cast<HICON>(LoadImage(
        GetInstance(),
        MAKEINTRESOURCE(IDI_APPICON),
        IMAGE_ICON, 16, 16, 0
    ));;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();

    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

// Window
Window::Window(int width, int height, const char *name) :
    width(width),
    height(height)
{
    // 窗口大小
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
    {
        throw D3D11WND_LAST_EXCEPT();
    }
    // 创建窗口
    hWnd = CreateWindow(
        WindowClass::GetName(),
        name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr,
        WindowClass::GetInstance(),
        this
        );

    if (hWnd == nullptr)
    {
        throw D3D11WND_LAST_EXCEPT();
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);

    pGfx = std::make_unique<D3D11Graphics>(hWnd);
}

Window::~Window()
{
    DestroyWindow(hWnd);
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // NCCREATE: Non Client Create，也就不是由客户端创建的，那么就是初始化的时候，这种情况使用自定义的创建方法
    // 通常在窗口第一次创建的时候使用WM_NCCREATE消息
    if (msg == WM_NCCREATE)
    {
        // 从构造参数里面提取指向WindowClass的指针，lpCreateParams就是创建的窗口
        const CREATESTRUCTW * const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // SetWindowLongPtr可以改变窗口属性，也允许用户指向窗口外的数据，这里就是指向的用户的数据，也就是窗口
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // 然后把消息处理函数替换成我们自己的
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    // 如果在初始化前接收到了消息，
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // 接收窗口类的指针
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // 发送消息
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg) 
    {
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }

        // clear keystate when window loses focus to prevent input getting "stuck"
        case WM_KILLFOCUS:
        {
            kbd.ClearState();
            break;
        }

        /*********** KEYBOARD MESSAGES ***********/
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
            {
                kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            break;
        }

        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
            break;
        }
        case WM_CHAR:
        {
            kbd.OnChar(static_cast<char>(wParam));
            break;
        }
        /*********** END KEYBOARD MESSAGES ***********/

        /************* MOUSE MESSAGES ****************/
        case WM_MOUSEMOVE:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            // 在窗口内部时，记录鼠标移动，同时改变鼠标的状态
            if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
            {
                mouse.OnMouseMove(pt.x, pt.y);
                if (!mouse.IsInWindow())
                {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter();
                }
            } else
            {
                // 如果不在窗口内，但是鼠标按键按下，那么也记录鼠标位置
                if (wParam & (MK_LBUTTON | MK_RBUTTON))
                {
                    mouse.OnMouseMove(pt.x, pt.y);
                } else
                {
                    ReleaseCapture();
                    mouse.OnMouseLeave();
                }
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnLeftPressed(pt.x, pt.y);
            // bring window to foreground on lclick client region
            SetForegroundWindow( hWnd );
            break;
        }
        case WM_RBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnRightPressed(pt.x, pt.y);
            break;
        }
        case WM_LBUTTONUP:
        {
            const POINTS pt = MAKEPOINTS(lParam );
            mouse.OnLeftReleased(pt.x, pt.y);
            // release mouse if outside of window
            if(pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
            {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
            break;
        }
        case WM_RBUTTONUP:
        {
            const POINTS pt = MAKEPOINTS(lParam );
            mouse.OnRightReleased(pt.x, pt.y);
            // release mouse if outside of window
            if(pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
            {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
            break;
        }
        case WM_MOUSEHWHEEL:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            mouse.OnWheelDelta(pt.x, pt.y, delta);
            break;
        }
        /************** END MOUSE MESSAGES **************/
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::SetTitle(const std::string &title)
{
    if (SetWindowText(hWnd, title.c_str()) == 0)
    {
        throw D3D11WND_LAST_EXCEPT();
    }
}

std::optional<int> Window::ProcessMessages()
{
    MSG msg;
    // 队列中有消息时，分发消息
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return (int)msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}

D3D11Graphics& Window::Gfx()
{
    if( !pGfx )
    {
        throw D3D11WND_NOGFX_EXCEPT();
    }
    return *pGfx;
}

// Window Exception
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    // windows will allocate memory for err string and make our pointer point to it
    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
    );
    // 0 string length returned indicates a failure
    if (nMsgLen == 0)
    {
        return "Unidentified error code";
    }
    // copy error string from windows-allocated buffer to std::string
    std::string errorString = pMsgBuf;
    // free windows buffer
    LocalFree( pMsgBuf );
    return errorString;
}

Window::HRException::HRException(int line, const char *file, HRESULT hr) noexcept :
    Exception(line, file),
    hr(hr)
{}

const char* Window::HRException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec
        << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Description] " << GetErrorDescription() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::HRException::GetType() const noexcept
{
    return "D3D11 Window Exception";
}

HRESULT Window::HRException::GetErrorCode() const noexcept
{
    return hr;
}

std::string Window::HRException::GetErrorDescription() const noexcept
{
    return Exception::TranslateErrorCode(hr);
}

const char* Window::NoGfxException::GetType() const noexcept
{
    return "D3D11 Window Exception [No Graphics]";
}

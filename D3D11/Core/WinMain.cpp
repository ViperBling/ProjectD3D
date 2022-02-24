#include <sstream>
#include "Utility/Window.h"

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
    )
{
    try
    {
        Window wnd(1280, 720, "ProjectD3D11");

        MSG msg;
        BOOL gResult;
        while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (wnd.kbd.KeyIsPressed(VK_SPACE))
            {
                MessageBox(nullptr, "I'm Space Bar!", "Space Key Was Pressed!", MB_OK | MB_ICONEXCLAMATION);
            }
            while (!wnd.mouse.IsEmpty())
            {
                const auto event = wnd.mouse.Read();
                if (event.GetType() == Mouse::Event::Type::Move)
                {
                    std::ostringstream oss;
                    oss << "Mouse Position: (" << event.GetPosX() << ", " << event.GetPosY() << ")";
                    wnd.SetTitle(oss.str());
                }
            }
        }

        if (gResult == -1)
        {
            return -1;
        }

        return msg.wParam;
    }
    catch (const D3D11Exception& e)
    {
        // MB_OK指Ok按钮，MB_ICONEXCLAMATION是弹出的感叹号
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox( nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION );
    }
    return -1;
}
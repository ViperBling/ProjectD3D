#include "WindowsApplication.h"

WindowsApplication::WindowsApplication() :
    wnd(800, 600, "ProjectD3D")
{}

int WindowsApplication::Run()
{
    while (true)
    {
        if (const auto eCode = Window::ProcessMessages())
        {
            return *eCode;
        }
        Tick();
    }
}

void WindowsApplication::Tick()
{
    const float c = sin(timer.Peek()) / 2.0f + 0.5f;
    wnd.Gfx().ClearBuffer( c, c, 1.0f);
    wnd.Gfx().EndFrame();
}
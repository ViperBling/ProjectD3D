#include "WindowsApplication.h"

#include <cmath>

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
    const float c = std::sin(timer.Peek()) / 2.0f + 0.5f;
    wnd.Gfx().ClearBuffer( c, c, 1.0f);
    wnd.Gfx().DrawTestTriangle(timer.Peek());
    wnd.Gfx().EndFrame();
}
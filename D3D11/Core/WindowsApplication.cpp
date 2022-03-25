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
    wnd.Gfx().DrawTestTriangle(
        -timer.Peek(),
        0.0f,
        0.0f
    );
    wnd.Gfx().DrawTestTriangle(
        timer.Peek(),
         (float)wnd.mouse.GetPosX() / 400.0f - 1.0f,
        -(float)wnd.mouse.GetPosY() / 300.0f + 1.0f);
    wnd.Gfx().EndFrame();
}
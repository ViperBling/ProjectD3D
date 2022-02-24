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

}
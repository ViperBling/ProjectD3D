#pragma once

#include "Utility/Window.h"

class WindowsApplication
{
public:
    WindowsApplication();
    // master frame / message loop
    int Run();

private:
    void Tick();

private:
    Window wnd;
};
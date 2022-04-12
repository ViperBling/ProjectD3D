#pragma once

#include "Utility/Window.h"
#include "Utility/Timer.h"
#include "Editor/ImguiManager.h"

class WindowsApplication
{
public:
    WindowsApplication();
    // master frame / message loop
    int Run();
    ~WindowsApplication();

private:
    void Tick();

private:
    ImguiManager imgui;
    Window wnd;
    Timer timer;
    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr size_t nDrawables = 180;
};
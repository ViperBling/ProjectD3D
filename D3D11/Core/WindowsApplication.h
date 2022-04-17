#pragma once

#include "Utility/Window.h"
#include "Utility/Timer.h"
#include "Utility/Camera.h"
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
    float speedFactor = 1.0;
    Camera camera;
    static constexpr size_t nDrawables = 180;
};
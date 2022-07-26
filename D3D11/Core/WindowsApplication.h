#pragma once

#include <set>

#include "Utility/Window.h"
#include "Utility/Timer.h"
#include "Utility/Camera.h"
#include "Editor/ImguiManager.h"
#include "Graphics/Lighting/PointLight.h"

class WindowsApplication
{
public:
    WindowsApplication();
    // master frame / message loop
    int Run();
    ~WindowsApplication();

private:
    void Tick();
    void SpawnSimulationWindow() noexcept;
    void SpawnBoxManagerWindow() noexcept;
    void SpawnBoxWindows() noexcept;

private:
    ImguiManager imgui;
    Window wnd;
    Timer timer;
    std::vector<std::unique_ptr<class Drawable>> drawables;
    std::vector<class Box*> boxes;
    float speedFactor = 1.0;
    Camera camera;
    PointLight light;
    static constexpr size_t nDrawables = 180;

    std::optional<int> comboBoxIndex;
    std::set<int> boxControlIds;
};
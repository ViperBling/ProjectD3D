#include "WindowsApplication.h"
#include "Graphics/Drawable/Shapes/Melon.h"
#include "Graphics/Drawable/Shapes/Pyramid.h"
#include "Graphics/Drawable/Shapes/Box.h"
#include "Graphics/Drawable/Shapes/Sheet.h"
#include "Graphics/Drawable/Shapes/SkinnedBox.h"
#include "Math/D3D11Math.h"
#include "Graphics/Surface.h"
#include "Graphics/GDIPlusManager.h"
#include "Imgui/imgui.h"
//#include "Imgui/backends/imgui_impl_win32.h"
//#include "Imgui/backends/imgui_impl_dx11.h"

#include <memory>
#include <algorithm>
#include <iterator>

namespace dx = DirectX;

GDIPlusManager gdiPlusManger;

WindowsApplication::WindowsApplication() :
    wnd(800, 600, "ProjectD3D")
{
    class Factory
    {
    public:
        Factory(D3D11Graphics& gfx )
            :
            gfx( gfx )
        {}
        std::unique_ptr<Drawable> operator()()
        {
            switch( typedist( rng ) )
            {
                case 0:
                    return std::make_unique<Pyramid>(
                        gfx,rng,adist,ddist,
                        odist,rdist
                    );
                case 1:
                    return std::make_unique<Box>(
                        gfx,rng,adist,ddist,
                        odist,rdist,bdist
                    );
                case 2:
                    return std::make_unique<Melon>(
                        gfx, rng, adist, ddist,
                        odist, rdist, longdist, latdist
                    );
                case 3:
                    return std::make_unique<Sheet>(
                        gfx, rng, adist, ddist,
                        odist, rdist
                        );
                case 4:
                    return std::make_unique<SkinnedBox>(
                        gfx,rng,adist,ddist,
                        odist,rdist
                        );
                default:
                    assert( false && "bad drawable type in factory" );
                    return {};
            }
        }
    private:
        D3D11Graphics& gfx;
        std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> adist{0.0f, PI * 2.0f};
        std::uniform_real_distribution<float> ddist{0.0f, PI * 0.5f};
        std::uniform_real_distribution<float> odist{0.0f, PI * 0.08f};
        std::uniform_real_distribution<float> rdist{6.0f, 20.0f};
        std::uniform_real_distribution<float> bdist{0.4f, 3.0f};
        std::uniform_int_distribution<int> latdist{5, 20};
        std::uniform_int_distribution<int> longdist{10, 40};
        std::uniform_int_distribution<int> typedist{0, 4};
    };

    drawables.reserve( nDrawables );
    std::generate_n( std::back_inserter( drawables ),nDrawables,Factory{wnd.Gfx()} );

    wnd.Gfx().SetProjection( dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

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

WindowsApplication::~WindowsApplication()
{}

void WindowsApplication::Tick()
{
    const auto DeltaTime = timer.Mark() * speedFactor;
    wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
    wnd.Gfx().SetCamera(camera.GetMatrix());

    for (auto & d : drawables)
    {
        d->Update(wnd.kbd.KeyIsPressed( VK_SPACE ) ? 0.0f : DeltaTime);
        d->Draw(wnd.Gfx());
    }

    if (ImGui::Begin("Simulation Speed")) {
        ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 4.0f);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNINGRUNNING (hold spacebar to pause)");
    }
    ImGui::End();

    camera.SpawnControlWindow();

    wnd.Gfx().EndFrame();
}
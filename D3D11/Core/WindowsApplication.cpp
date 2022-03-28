#include "WindowsApplication.h"
#include "Graphics/Drawable/Box.h"
#include <cmath>

WindowsApplication::WindowsApplication() :
    wnd(800, 600, "ProjectD3D")
{
    std::mt19937 rng( std::random_device{}() );
    std::uniform_real_distribution<float> adist( 0.0f,3.1415f * 2.0f );
    std::uniform_real_distribution<float> ddist( 0.0f,3.1415f * 2.0f );
    std::uniform_real_distribution<float> odist( 0.0f,3.1415f * 0.3f );
    std::uniform_real_distribution<float> rdist( 6.0f,20.0f );
    for( auto i = 0; i < 80; i++ )
    {
        boxes.push_back(std::make_unique<Box>(
            wnd.Gfx(),
            rng,
            adist,
            ddist,
            odist,
            rdist
        ));
    }
    wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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
    auto DeltaTime = timer.Mark();
    wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
    for (auto & b : boxes)
    {
        b->Update(DeltaTime);
        b->Draw(wnd.Gfx());
    }
    wnd.Gfx().EndFrame();
}
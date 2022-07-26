#include "WindowsApplication.h"
#include "Graphics/Drawable/Shapes/Box.h"
#include "Graphics/Drawable/Shapes/Cylinder.h"
#include "Graphics/Drawable/Shapes/Pyramid.h"
#include "Graphics/Drawable/Shapes/SkinnedBox.h"
#include "Math/D3D11Math.h"
#include "Graphics/Surface.h"
#include "Graphics/GDIPlusManager.h"
#include "Imgui/imgui.h"


#include <memory>
#include <algorithm>
#include <iterator>

namespace dx = DirectX;

GDIPlusManager gdiPlusManger;

WindowsApplication::WindowsApplication() :
    wnd(1280, 720, "ProjectD3D"),
    light(wnd.Gfx())
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
            const DirectX::XMFLOAT3 mat = {cdist(rng), cdist(rng), cdist(rng)};

            switch (sdist(rng)) {
            case 0:
                return std::make_unique<Box>(
                    gfx, rng, adist, ddist,
                    odist, rdist, bdist, mat
                    );
            case 1:
                return std::make_unique<Cylinder>(
                    gfx, rng, adist, ddist, odist,
                    rdist, bdist, tdist
                    );
            case 2:
                return std::make_unique<Pyramid>(
                    gfx, rng, adist, ddist, odist,
                    rdist, tdist
                    );
            case 3:
                return std::make_unique<SkinnedBox>(
                    gfx,rng,adist,ddist,
                    odist,rdist
                );
            default:
                assert(false && "impossible drawable option in factory");
                return {};
            }
        }
    private:
        D3D11Graphics& gfx;
        std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<int> sdist{0, 2};
        std::uniform_real_distribution<float> adist{0.0f, PI * 2.0f};
        std::uniform_real_distribution<float> ddist{0.0f, PI * 0.5f};
        std::uniform_real_distribution<float> odist{0.0f, PI * 0.08f};
        std::uniform_real_distribution<float> rdist{6.0f, 20.0f};
        std::uniform_real_distribution<float> bdist{0.4f, 3.0f};
        std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
        std::uniform_int_distribution<int> tdist{3, 30};
    };

    drawables.reserve( nDrawables );
    std::generate_n( std::back_inserter( drawables ),nDrawables,Factory{wnd.Gfx()} );

    for (auto& pd : drawables) {
        if (auto pb = dynamic_cast<Box*>(pd.get())) boxes.push_back(pb);
    }

    wnd.Gfx().SetProjection( dx::XMMatrixPerspectiveLH(1.0f, 720.0f / 1280.0f, 0.5f, 40.0f));
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
    light.Bind(wnd.Gfx(), camera.GetMatrix());

    for (auto & d : drawables)
    {
        d->Update(wnd.kbd.KeyIsPressed( VK_SPACE ) ? 0.0f : DeltaTime);
        d->Draw(wnd.Gfx());
    }
    light.Draw(wnd.Gfx());

    SpawnSimulationWindow();
    camera.SpawnControlWindow();
    light.SpawnControlWindow();
    SpawnBoxManagerWindow();
    SpawnBoxWindows();

    wnd.Gfx().EndFrame();
}

void WindowsApplication::SpawnSimulationWindow() noexcept {

    if( ImGui::Begin( "Simulation Speed" ) )
    {
        ImGui::SliderFloat( "Speed Factor", &speedFactor, 0.0f, 6.0f,"%.4f");
        ImGui::Text( "%.3f ms/frame (%.1f FPS)",1000.0f / ImGui::GetIO().Framerate,ImGui::GetIO().Framerate );
        ImGui::Text( "Status: %s",wnd.kbd.KeyIsPressed( VK_SPACE ) ? "PAUSED" : "RUNNING (hold spacebar to pause)" );
    }
    ImGui::End();
}

void WindowsApplication::SpawnBoxManagerWindow() noexcept {

    if( ImGui::Begin( "Boxes" ) )
    {
        using namespace std::string_literals;
        const auto preview = comboBoxIndex ? std::to_string( *comboBoxIndex ) : "Choose a box..."s;
        if( ImGui::BeginCombo( "Box Number",preview.c_str() ) )
        {
            for( int i = 0; i < boxes.size(); i++ )
            {
                const bool selected = *comboBoxIndex == i;
                if( ImGui::Selectable( std::to_string( i ).c_str(),selected ) )
                {
                    comboBoxIndex = i;
                }
                if( selected )
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if( ImGui::Button( "Spawn Control Window" ) && comboBoxIndex )
        {
            boxControlIds.insert( *comboBoxIndex );
            comboBoxIndex.reset();
        }
    }
    ImGui::End();
}

void WindowsApplication::SpawnBoxWindows() noexcept {

    for( auto id : boxControlIds )
    {
        boxes[id]->SpawnControlWindow( id,wnd.Gfx() );
    }
}

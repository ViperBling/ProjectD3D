#include "Sheet.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"
#include "Graphics/Drawable/Shapes/Plane.h"
#include "Graphics/Surface.h"
#include "Graphics/Bindable/Texture.h"
#include "Graphics/Bindable/Sampler.h"

Sheet::Sheet(
    D3D11Graphics &gfx,
    std::mt19937 &rng,
    std::uniform_real_distribution<float> &adist,
    std::uniform_real_distribution<float> &ddist,
    std::uniform_real_distribution<float> &odist,
    std::uniform_real_distribution<float> &rdist) :
    r( rdist( rng ) ),
    droll( ddist( rng ) ),
    dpitch( ddist( rng ) ),
    dyaw( ddist( rng ) ),
    dphi( odist( rng ) ),
    dtheta( odist( rng ) ),
    dchi( odist( rng ) ),
    chi( adist( rng ) ),
    theta( adist( rng ) ),
    phi( adist( rng ) )
{
    namespace dx = DirectX;

    if (!IsStaticInitialized()) {
        struct Vertex {
            dx::XMFLOAT3 pos;
            struct {
                float u;
                float v;
            } tex ;
        };

        auto model = Plane::Make<Vertex>();
        model.vertices[0].tex = {0.0f, 0.0f};
        model.vertices[1].tex = {1.0f, 0.0f};
        model.vertices[2].tex = {0.0f, 1.0f};
        model.vertices[3].tex = {1.0f, 1.0f};

        AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile("Images\\kappa50.png")));
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
        AddStaticBind(std::make_unique<Sampler>(gfx));
    }
}

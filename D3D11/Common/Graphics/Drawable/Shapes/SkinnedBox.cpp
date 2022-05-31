#include "SkinnedBox.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"
#include "Graphics/Drawable/Shapes/Cube.h"
#include "Graphics/Surface.h"
#include "Graphics/Bindable/Texture.h"
#include "Graphics/Bindable/Sampler.h"

SkinnedBox::SkinnedBox(
    D3D11Graphics &gfx,
    std::mt19937 &rng,
    std::uniform_real_distribution<float> &adist,
    std::uniform_real_distribution<float> &ddist,
    std::uniform_real_distribution<float> &odist,
    std::uniform_real_distribution<float> &rdist) :
    TestObject(gfx, rng, adist, ddist, odist, rdist)
{
    namespace dx = DirectX;

    if(!IsStaticInitialized()) {
        struct Vertex {
            dx::XMFLOAT3 pos;
            dx::XMFLOAT3 n;
            dx::XMFLOAT2 uv;
        };

        const auto model = Cube::MakeIndependentTextured<Vertex>();

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
        AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile("./Assets/Textures/cube.png")));

        AddStaticBind(std::make_unique<Sampler>(gfx));

        auto pvs = std::make_unique<VertexShader>(gfx, L"./Assets/Shaders/TextureVS.cso");
        auto pvsbc = pvs->GetBytecode();

        AddStaticBind(std::move(pvs));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"Assets/Shaders/TexturePS.cso"));
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

        struct PSMaterialConstant {
            float specularIntensity = 0.6f;
            float specularPower = 30.0f;
            float padding[2];
        } colorConst;
        AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));

    } else {
        SetIndexFromStatic();
    }
    AddBind(std::make_unique<Sampler>(gfx));
    AddBind(std::make_unique<TransformCBuffer>(gfx, *this));
}

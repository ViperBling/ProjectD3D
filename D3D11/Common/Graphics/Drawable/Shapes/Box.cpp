#include "Box.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"
#include "Cube.h"

Box::Box(
    D3D11Graphics &gfx, std::mt19937 &rng,
    std::uniform_real_distribution<float>& adist,
    std::uniform_real_distribution<float>& ddist,
    std::uniform_real_distribution<float>& odist,
    std::uniform_real_distribution<float>& rdist,
    std::uniform_real_distribution<float>& bdist)
    :
    r(rdist(rng)),
    droll(ddist(rng)),
    dpitch(ddist(rng)),
    dyaw(ddist(rng)),
    dphi(odist(rng)),
    dtheta(odist(rng)),
    dchi(odist(rng)),
    chi(adist(rng)),
    theta(adist(rng)),
    phi(adist(rng))
{
    namespace dx = DirectX;
    // 如果不是第一次实例化，就可以使用静态绑定，但是要注意对其他的静态绑定要进行一次IndexBuffer的指针赋值
    if (!IsStaticInitialized())
    {
        struct Vertex
        {
            dx::XMFLOAT3 pos;
        };
        auto model = Cube::Make<Vertex>();
        model.Transform( dx::XMMatrixScaling( 1.0f,1.0f,1.2f ) );
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

        auto pvs = std::make_unique<VertexShader>(gfx, L"../../Shaders/ColorIndexVS.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"../../Shaders/ColorIndexPS.cso"));

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>(gfx, model.indices));

        struct PixelShaderConstants
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            } face_colors[8];
        };
        const PixelShaderConstants cb2 =
            {
                {
                    {1.0f, 1.0f, 1.0f},
                    {1.0f, 0.0f, 0.0f},
                    {0.0f, 1.0f, 0.0f},
                    {1.0f, 1.0f, 0.0f},
                    {0.0f, 0.0f, 1.0f},
                    {1.0f, 0.0f, 1.0f},
                    {0.0f, 1.0f, 1.0f},
                    {0.0f, 0.0f, 0.0f},
                }
            };

        AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, cb2));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied ={
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformCBuffer>(gfx, *this));

    // model deformation transform (per instance, not stored as bind)
    dx::XMStoreFloat3x3(
        &mt,
        dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
    );
}

void Box::Update( float DeltaTime ) noexcept
{
    roll += droll * DeltaTime;
    pitch += dpitch * DeltaTime;
    yaw += dyaw * DeltaTime;
    theta += dtheta * DeltaTime;
    phi += dphi * DeltaTime;
    chi += dchi * DeltaTime;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
    namespace dx = DirectX;
    return dx::XMLoadFloat3x3(&mt) *
           dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
           dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
           dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
           dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}

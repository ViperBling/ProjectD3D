﻿#include "Box.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"
#include "Cube.h"

Box::Box(
    D3D11Graphics &gfx, std::mt19937 &rng,
    std::uniform_real_distribution<float>& adist,
    std::uniform_real_distribution<float>& ddist,
    std::uniform_real_distribution<float>& odist,
    std::uniform_real_distribution<float>& rdist,
    std::uniform_real_distribution<float>& bdist,
    DirectX::XMFLOAT3 material)
    :
    TestObject(gfx, rng, adist, ddist, odist, rdist)
{
    namespace dx = DirectX;
    // 如果不是第一次实例化，就可以使用静态绑定，但是要注意对其他的静态绑定要进行一次IndexBuffer的指针赋值
    if (!IsStaticInitialized())
    {
        struct Vertex
        {
            dx::XMFLOAT3 pos;
            dx::XMFLOAT3 n;
        };
        auto model = Cube::MakeIndependent<Vertex>();
        model.SetNormalsIndependentFlat();

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

        auto pvs = std::make_unique<VertexShader>(gfx, L"../../Shaders/PhongVS.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"../../Shaders/PhongPS.cso"));

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>(gfx, model.indices));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied ={
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformCBuffer>(gfx, *this));

    struct PSMaterialConstant
    {
        dx::XMFLOAT3 color;
        float specularIntensity = 0.6f;
        float specularPower = 30.0f;
        float padding[3];
    } colorConstant;
    colorConstant.color = material;
    AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConstant, 1u));

    // model deformation transform (per instance, not stored as bind)
    dx::XMStoreFloat3x3(
        &mt,
        dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
    );
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
    namespace dx = DirectX;
    return dx::XMLoadFloat3x3(&mt) * TestObject::GetTransformXM();

}

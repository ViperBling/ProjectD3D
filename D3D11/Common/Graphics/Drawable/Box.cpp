﻿#include "Box.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

Box::Box(
    D3D11Graphics &gfx, std::mt19937 &rng,
    std::uniform_real_distribution<float> &adist,
    std::uniform_real_distribution<float> &ddist,
    std::uniform_real_distribution<float> &odist,
    std::uniform_real_distribution<float> &rdist)
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
    // 如果不是第一次实例化，就可以使用静态绑定，但是要注意对其他的静态绑定要进行一次IndexBuffer的指针赋值
    if (!IsStaticInitialized())
    {
        struct Vertex
        {
            struct {
                float x;
                float y;
                float z;
            } pos;
        };
        const std::vector<Vertex> vertices =
            {
                {-1.0f, -1.0f, -1.0f},
                {1.0f, -1.0f, -1.0f},
                {-1.0f, 1.0f, -1.0f},
                {1.0f, 1.0f, -1.0f},
                {-1.0f, -1.0f, 1.0f},
                {1.0f, -1.0f, 1.0f},
                {-1.0f, 1.0f, 1.0f},
                {1.0f, 1.0f, 1.0f},
            };
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

        auto pvs = std::make_unique<VertexShader>(gfx, L"../../Shaders/VertexShader.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        AddStaticBind(std::make_unique<PixelShder>(gfx, L"../../Shaders/PixelShader.cso"));

        const std::vector<unsigned short> indices ={
            0,2,1, 2,3,1,
            1,3,5, 3,7,5,
            2,6,3, 3,6,7,
            4,5,7, 4,7,6,
            0,4,2, 2,4,6,
            0,1,4, 1,5,4
        };
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        struct ConstantBuffer2
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            } face_colors[6];
        };
        const ConstantBuffer2 cb2 = {
            {
                {1.0f, 0.0f, 1.0f},
                {1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
                {1.0f, 1.0f, 0.0f},
                {0.0f, 1.0f, 1.0f},
            }
        };

        AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied ={
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
        };

        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformCBuffer>(gfx, *this));
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
    return DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
           DirectX::XMMatrixTranslation( r,0.0f,0.0f ) *
           DirectX::XMMatrixRotationRollPitchYaw( theta,phi,chi ) *
           DirectX::XMMatrixTranslation( 0.0f,0.0f,20.0f );
}

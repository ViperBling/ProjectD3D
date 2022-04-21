#pragma once
#include "CMakeConfig.h"
#include "Graphics/D3D11Graphics.h"
#include "Graphics/Drawable/Shapes/SolidSphere.h"
#include "Graphics/Bindable/ConstantBuffers.h"

class PointLight
{
public:
    PointLight(D3D11Graphics& gfx, float radius = 0.5f);
    void SpawnControlWindow() noexcept;
    void Reset() noexcept;
    void Draw(D3D11Graphics& gfx) const noexcept(!IS_DEBUG);
    void Bind(D3D11Graphics& gfx) const noexcept;

private:
    struct PointLightCBuffer {
        DirectX::XMFLOAT3 pos;
        float padding;
    };

private:
    DirectX::XMFLOAT3 pos = {0.0f, 0.0f, 0.0f};
    mutable SolidSphere mesh;
    mutable PixelConstantBuffer<PointLightCBuffer> cBuffer;
};
#pragma once

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
    void Bind(D3D11Graphics& gfx, DirectX::XMMATRIX view) const noexcept;

private:
    struct PointLightCBuffer {
        alignas(16) DirectX::XMFLOAT3 pos;
        alignas(16) DirectX::XMFLOAT3 ambient;
        alignas(16) DirectX::XMFLOAT3 diffuseColor;
        float diffuseIntensity;
        float attenConst;
        float attenLin;
        float attenQuad;
    };

private:
    PointLightCBuffer pointLightCBData;
    mutable SolidSphere mesh;
    mutable PixelConstantBuffer<PointLightCBuffer> cBuffer;
};
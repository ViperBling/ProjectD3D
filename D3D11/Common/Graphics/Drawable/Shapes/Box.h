#pragma once

#include "TestObjects.h"
#include "Graphics/Bindable/ConstantBuffers.h"

// 使用中间层来创建Box
class Box : public TestObject<Box>
{
public:
    Box(D3D11Graphics& gfx, std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_real_distribution<float>& bdist,
        DirectX::XMFLOAT3 material
    );
    DirectX::XMMATRIX GetTransformXM() const noexcept override;

    void SpawnControlWindow(int id, D3D11Graphics& gfx) noexcept;

private:
    void SyncMaterial(D3D11Graphics& gfx) noexcept(!IS_DEBUG);

private:
    struct PSMaterialConstant {
        DirectX::XMFLOAT3 color;
        float specularIntensity = 0.6f;
        float specularPower = 30.0f;
        float padding[3];
    } materialConstant;
    using MaterialCbuffer = PixelConstantBuffer<PSMaterialConstant>;

    // model transform
    DirectX::XMFLOAT3X3 mt;
};
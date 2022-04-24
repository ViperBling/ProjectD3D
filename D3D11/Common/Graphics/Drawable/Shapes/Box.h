#pragma once
#include "Graphics/Drawable/DrawableBase.h"

// 使用中间层来创建Box
class Box : public DrawableBase<Box>
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
    void Update(float DeltaTime) noexcept override;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
    // positional
    float r;
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float theta;
    float phi;
    float chi;
    // speed (delta/s)
    float droll;
    float dpitch;
    float dyaw;
    float dtheta;
    float dphi;
    float dchi;
    // model transform
    DirectX::XMFLOAT3X3 mt;
};
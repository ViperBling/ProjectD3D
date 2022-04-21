#pragma once
#include "Graphics/Drawable/DrawableBase.h"

class SolidSphere : public DrawableBase<SolidSphere>
{
public:
    SolidSphere(D3D11Graphics& gfx, float radius);
    void Update(float DeltaTime) noexcept override;
    void SetPos(DirectX::XMFLOAT3 pos) noexcept;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
    DirectX::XMFLOAT3 pos = {1.0f, 1.0f, 1.0f};
};
#pragma once
#include "ConstantBuffers.h"
#include "Graphics/Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformCBuffer : public Bindable
{
private:
    struct Transforms {
        DirectX::XMMATRIX modelViewProj;
        DirectX::XMMATRIX model;
    };

public:
    TransformCBuffer(D3D11Graphics& gfx, const Drawable& parent, UINT slot = 0);
    void Bind(D3D11Graphics& gfx) noexcept override;

private:
    static std::unique_ptr<VertexConstantBuffer<Transforms>> pVCBuffer;
    const Drawable& parent;
};

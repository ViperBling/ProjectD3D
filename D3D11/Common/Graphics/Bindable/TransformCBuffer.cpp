#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(D3D11Graphics &gfx, const Drawable &parent) :
    parent(parent)
{
    if (!pVCBuffer)
    {
        pVCBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
    }
}

void TransformCBuffer::Bind(D3D11Graphics &gfx) noexcept
{
    auto model = parent.GetTransformXM();
    const Transforms transforms = {
        DirectX::XMMatrixTranspose(model),
        DirectX::XMMatrixTranspose(
            model *
            gfx.GetCamera() *
            gfx.GetProjection()
            )
    };

    pVCBuffer->Update(gfx, transforms);
    pVCBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::pVCBuffer;
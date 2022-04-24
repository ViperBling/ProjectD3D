#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(D3D11Graphics &gfx, const Drawable &parent, UINT slot) :
    parent(parent)
{
    if (!pVCBuffer)
    {
        pVCBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
    }
}

void TransformCBuffer::Bind(D3D11Graphics &gfx) noexcept
{
    const auto modelView = parent.GetTransformXM() * gfx.GetCamera();
    const Transforms transforms = {
        DirectX::XMMatrixTranspose(modelView),
        DirectX::XMMatrixTranspose(
            modelView *
            gfx.GetProjection()
            )
    };

    pVCBuffer->Update(gfx, transforms);
    pVCBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::pVCBuffer;
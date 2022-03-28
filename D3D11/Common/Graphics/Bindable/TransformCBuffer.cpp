#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(D3D11Graphics &gfx, const Drawable &parent) :
    vcBuffer(gfx),
    parent(parent)
{}

void TransformCBuffer::Bind(D3D11Graphics &gfx) noexcept
{
    vcBuffer.Update(
        gfx,
        DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection())
        );
    vcBuffer.Bind(gfx);
}
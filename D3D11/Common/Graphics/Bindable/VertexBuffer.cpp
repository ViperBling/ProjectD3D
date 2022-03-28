#include "VertexBuffer.h"

void VertexBuffer::Bind(D3D11Graphics &gfx) noexcept
{
    const UINT offset = 0;
    GetContext(gfx)->IASetVertexBuffers(
        0u,
        1u,
        pVertexBuffer.GetAddressOf(),
        &stride, &offset);
}
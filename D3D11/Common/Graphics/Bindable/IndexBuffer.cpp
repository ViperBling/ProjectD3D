#include "IndexBuffer.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

IndexBuffer::IndexBuffer(D3D11Graphics &gfx, const std::vector<unsigned short> &indices) :
    count(UINT(indices.size()))
{
    INFOMAN(gfx);

    D3D11_BUFFER_DESC iBufferDesc = {};
    iBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    iBufferDesc.CPUAccessFlags = 0u;
    iBufferDesc.MiscFlags = 0u;
    iBufferDesc.ByteWidth = UINT(count * sizeof(unsigned short));
    iBufferDesc.StructureByteStride = sizeof(unsigned short);

    D3D11_SUBRESOURCE_DATA iSRD = {};
    iSRD.pSysMem = indices.data();
    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&iBufferDesc, &iSRD, &pIndexBuffer));
}

void IndexBuffer::Bind(D3D11Graphics &gfx) noexcept
{
    GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
    return count;
}

#pragma once

#include "Bindable.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

class VertexBuffer : public Bindable
{
public:
    template<class T>
    VertexBuffer(D3D11Graphics& gfx, const std::vector<T>& vertices) :
        stride(sizeof(T))
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC vBufferDesc = {};
        vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vBufferDesc.CPUAccessFlags = 0u;
        vBufferDesc.MiscFlags = 0u;
        vBufferDesc.ByteWidth = UINT(sizeof(T) * vertices.size());
        vBufferDesc.StructureByteStride = sizeof(T);
        D3D11_SUBRESOURCE_DATA vSRD = {};
        vSRD.pSysMem = vertices.data();
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vBufferDesc, &vSRD, &pVertexBuffer));
    }
    void Bind(D3D11Graphics& gfx) noexcept override;

protected:
    UINT stride;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

};

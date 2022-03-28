#pragma once

#include "Bindable.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

template<typename T>
class ConstantBuffer : public Bindable
{
public:
    void Update(D3D11Graphics& gfx, const T& consts)
    {
        INFOMAN(gfx);

        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        GFX_THROW_INFO(
            GetContext(gfx)->Map(
                pConstantBuffer.Get(), 0u,
                D3D11_MAP_WRITE_DISCARD, 0u,
                &mappedSubresource
                ));
        memcpy(mappedSubresource.pData, &consts, sizeof(consts));
        GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
    }

    ConstantBuffer(D3D11Graphics& gfx, const T& consts)
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC cBufferDesc;
        cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cBufferDesc.MiscFlags = 0u;
        cBufferDesc.ByteWidth = sizeof(consts);
        cBufferDesc.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA cSRD;
        cSRD.pSysMem = &consts;
        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cBufferDesc, &cSRD, &pConstantBuffer));
    }

    ConstantBuffer(D3D11Graphics& gfx)
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC cBufferDesc;
        cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cBufferDesc.MiscFlags = 0u;
        cBufferDesc.ByteWidth = sizeof(T);
        cBufferDesc.StructureByteStride = 0u;

        GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cBufferDesc, nullptr, &pConstantBuffer));
    }

protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
    using ConstantBuffer<T>::pConstantBuffer;
    using Bindable::GetContext;

public:
    using ConstantBuffer<T>::ConstantBuffer;
    void Bind(D3D11Graphics& gfx) noexcept override
    {
        GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
    }
};

template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
    using ConstantBuffer<T>::pConstantBuffer;
    using Bindable::GetContext;

public:
    using ConstantBuffer<T>::ConstantBuffer;
    void Bind(D3D11Graphics& gfx) noexcept override
    {
        GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
    }
};
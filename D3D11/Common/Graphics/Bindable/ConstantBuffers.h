#pragma once

#include "Bindable.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

template<typename T>
class ConstantBuffer : public Bindable
{
public:
    // 每帧更新，为了避免每帧创建内容，用map和unmap的方式实现内存的更新
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

    // 根据指定内容创建ConstantBuffer
    ConstantBuffer(D3D11Graphics& gfx, const T& consts, UINT slot = 0) :
        slot(slot)
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

    // 默认的创建类型
    ConstantBuffer(D3D11Graphics& gfx, UINT slot = 0) :
        slot(slot)
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
    UINT slot;
};

template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
    // 可以使用this指针的方式调用GetContext，也可以在这里using声明来调用
    using ConstantBuffer<T>::pConstantBuffer;
    using ConstantBuffer<T>::slot;
    using Bindable::GetContext;

public:
    using ConstantBuffer<T>::ConstantBuffer;
    void Bind(D3D11Graphics& gfx) noexcept override
    {
        GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
    }
};

template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
    // 可以使用this指针的方式调用GetContext，也可以在这里using声明来调用
    using ConstantBuffer<T>::pConstantBuffer;
    using ConstantBuffer<T>::slot;
    using Bindable::GetContext;

public:
    using ConstantBuffer<T>::ConstantBuffer;
    void Bind(D3D11Graphics& gfx) noexcept override
    {
        GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
    }
};
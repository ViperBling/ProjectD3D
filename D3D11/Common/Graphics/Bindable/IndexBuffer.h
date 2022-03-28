#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
    IndexBuffer(D3D11Graphics& gfx, const std::vector<unsigned short>& indices);
    void Bind(D3D11Graphics& gfx) noexcept override;
    UINT GetCount() const noexcept;

protected:
    UINT count;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};
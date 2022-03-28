#pragma once

#include "Bindable.h"

class InputLayout : public Bindable
{
public:
    InputLayout(
        D3D11Graphics& gfx,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
        ID3DBlob* pVertexShaderByteCode);
    void Bind(D3D11Graphics& gfx) noexcept override;

protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};
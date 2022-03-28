#include "InputLayout.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

InputLayout::InputLayout(
    D3D11Graphics &gfx,
    const std::vector<D3D11_INPUT_ELEMENT_DESC> &layout,
    ID3DBlob *pVertexShaderByteCode)
{
    INFOMAN(gfx);

    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
        layout.data(), (UINT)layout.size(),
        pVertexShaderByteCode->GetBufferPointer(),
        pVertexShaderByteCode->GetBufferSize(),
        &pInputLayout
        ));
}

void InputLayout::Bind(D3D11Graphics &gfx) noexcept
{
    GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}

#include "PixelShader.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

PixelShader::PixelShader(D3D11Graphics &gfx, const std::wstring &path)
{
    INFOMAN(gfx);

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
    GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        nullptr,
        &pPixelShader
        ));
}

void PixelShader::Bind(D3D11Graphics &gfx) noexcept
{
    GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
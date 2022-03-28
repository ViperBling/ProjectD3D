#include "VertexShader.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

VertexShader::VertexShader(D3D11Graphics &gfx, const std::wstring &path)
{
    INFOMAN(gfx);

    GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));
    GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
        pBytecodeBlob->GetBufferPointer(),
        pBytecodeBlob->GetBufferSize(),
        nullptr,
        &pVertexShader
        ));
}

void VertexShader::Bind(D3D11Graphics &gfx) noexcept
{
    GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
    return pBytecodeBlob.Get();
}

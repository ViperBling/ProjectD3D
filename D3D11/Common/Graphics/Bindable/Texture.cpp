#include "Texture.h"
#include "Graphics/Surface.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

namespace wrl = Microsoft::WRL;

Texture::Texture(D3D11Graphics &gfx, const class Surface &s) {
    INFOMAN(gfx);

    // 创建纹理
    D3D11_TEXTURE2D_DESC texture2DDesc = {};
    texture2DDesc.Width = s.GetWidth();
    texture2DDesc.Height = s.GetHeight();
    texture2DDesc.MipLevels = 1;
    texture2DDesc.ArraySize = 1;
    texture2DDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texture2DDesc.SampleDesc.Count = 1;
    texture2DDesc.SampleDesc.Quality = 0;
    texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
    texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texture2DDesc.CPUAccessFlags = 0;
    texture2DDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pSysMem = s.GetBufferPtr();
    subResourceData.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);
    wrl::ComPtr<ID3D11Texture2D> pTexture;
    GFX_THROW_INFO(
        GetDevice(gfx)->CreateTexture2D(
            &texture2DDesc, &subResourceData, &pTexture
            )
        );

    // 创建SRV
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texture2DDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    GFX_THROW_INFO(
        GetDevice(gfx)->CreateShaderResourceView(
            pTexture.Get(), &srvDesc, &pTextureView
            )
        );
}

void Texture::Bind(D3D11Graphics &gfx) noexcept {
    GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}
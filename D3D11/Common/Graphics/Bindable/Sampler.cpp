#include "Sampler.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"

Sampler::Sampler(D3D11Graphics &gfx) {
    INFOMAN(gfx);

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    GFX_THROW_INFO(
        GetDevice(gfx)->CreateSamplerState(
            &samplerDesc, &pSampler)
        );
}

void Sampler::Bind(D3D11Graphics &gfx) noexcept {
    GetContext(gfx)->PSSetSamplers(0u, 1u, pSampler.GetAddressOf());
}
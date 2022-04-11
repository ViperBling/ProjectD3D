#pragma once

#include "Bindable.h"

class Sampler : public Bindable
{
public:
    Sampler(D3D11Graphics& gfx);
    void Bind(D3D11Graphics& gfx) noexcept override;

protected:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};
#pragma once

#include "Bindable.h"

class Texture : public Bindable
{
public:
    Texture(D3D11Graphics& gfx, const class Surface& s);
    void Bind(D3D11Graphics& gfx) noexcept override;

protected:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};
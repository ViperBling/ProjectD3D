#pragma once

#include "Bindable.h"

class PixelShder : public Bindable
{
public:
    PixelShder(D3D11Graphics& gfx, const std::wstring& path);
    void Bind(D3D11Graphics& gfx) noexcept override;

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};
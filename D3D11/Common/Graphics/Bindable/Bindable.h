#pragma once

#include "CMakeConfig.h"
#include "Graphics/D3D11Graphics.h"

class Bindable
{
public:
    virtual void Bind(D3D11Graphics& gfx) noexcept = 0;
    virtual ~Bindable() = default;

protected:
    static ID3D11DeviceContext* GetContext(D3D11Graphics& gfx) noexcept;
    static ID3D11Device* GetDevice(D3D11Graphics& gfx) noexcept;
    static DXGIInfoManager& GetInfoManager(D3D11Graphics& gfx) noexcept(!IS_DEBUG);
};
#pragma once

#include "CMakeConfig.h"
#include "Graphics/D3D11Graphics.h"

// Bindable基类，包括所有可绑定到管线上的对象
class Bindable
{
public:
    virtual void Bind(D3D11Graphics& gfx) noexcept = 0;
    virtual ~Bindable() = default;

protected:
    // 把基类设为D3D11Graphics的友元，
    // 并把这些函数设置为静态的，那么子类就可以在基类不实例化情况下访问gfx的内容，同时也做到了访问限制
    static ID3D11DeviceContext* GetContext(D3D11Graphics& gfx) noexcept;
    static ID3D11Device* GetDevice(D3D11Graphics& gfx) noexcept;
    static DXGIInfoManager& GetInfoManager(D3D11Graphics& gfx) noexcept(!IS_DEBUG);
};
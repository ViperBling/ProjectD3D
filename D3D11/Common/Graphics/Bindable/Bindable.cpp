#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(D3D11Graphics &gfx) noexcept
{
    return gfx.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(D3D11Graphics &gfx) noexcept
{
    return gfx.pDevice.Get();
}

DXGIInfoManager& Bindable::GetInfoManager(D3D11Graphics &gfx) noexcept(!IS_DEBUG)
{
#ifndef NDEBUG
    return gfx.infoManager;
#else
    throw std::logic_error( "Failed! (tried to access gfx.infoManager in Release config)" );
#endif
}
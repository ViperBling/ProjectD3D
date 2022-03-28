#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
    Topology(D3D11Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
    void Bind(D3D11Graphics& gfx) noexcept override;

protected:
    D3D11_PRIMITIVE_TOPOLOGY type;
};
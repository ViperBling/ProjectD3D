#include "Topology.h"

Topology::Topology(D3D11Graphics &gfx, D3D11_PRIMITIVE_TOPOLOGY type) :
    type(type)
{}

void Topology::Bind(D3D11Graphics &gfx) noexcept
{
    GetContext(gfx)->IASetPrimitiveTopology(type);
}
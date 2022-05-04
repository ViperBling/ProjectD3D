#pragma once
#include "Graphics/Drawable/Shapes/TestObjects.h"

class Pyramid : public TestObject<Pyramid>
{
public:
    Pyramid(
        D3D11Graphics& gfx,
        std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_int_distribution<int>& tdist);
};
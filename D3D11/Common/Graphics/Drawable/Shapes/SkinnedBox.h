#pragma once

#include "Graphics/Drawable/Shapes/TestObjects.h"

class SkinnedBox : public TestObject<SkinnedBox>
{
public:
    SkinnedBox(
        D3D11Graphics& gfx,
        std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist
    );
};
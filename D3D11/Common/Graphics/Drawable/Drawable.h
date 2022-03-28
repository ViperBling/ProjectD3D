#pragma once

#include "CMakeConfig.h"
#include "Graphics/D3D11Graphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;
    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
    void Draw(D3D11Graphics& gfx) const noexcept(!IS_DEBUG);
    virtual void Update(float DeltaTime) noexcept = 0;
    void AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
    void AddIndexBuffer(std::unique_ptr<class IndexBuffer> idBuffer) noexcept;
    virtual ~Drawable() = default;

private:
    const IndexBuffer* pIndexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable>> binds;
};

#pragma once

#include "CMakeConfig.h"
#include "Graphics/D3D11Graphics.h"
#include <DirectXMath.h>

class Bindable;

// 控制所有Bindable对象以及逻辑组织
class Drawable
{
    // Drawable和实体的中间层，这样可以绘制不同物体使用不同的Buffer，多个相同物体使用共同的Buffer
    template<class T>
    friend class DrawableBase;
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;
    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
    void Draw(D3D11Graphics& gfx) const noexcept(!IS_DEBUG);
    virtual void Update(float DeltaTime) noexcept = 0;
    virtual ~Drawable() = default;

protected:
    void AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
    void AddIndexBuffer(std::unique_ptr<class IndexBuffer> idBuffer) noexcept(!IS_DEBUG);

private:
    // 通过纯虚函数访问StaticBinds
    virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

private:
    const class IndexBuffer* pIndexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable>> binds;
};

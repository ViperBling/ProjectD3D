#include "Drawable.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"
#include "Graphics/Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(D3D11Graphics &gfx) const noexcept(!IS_DEBUG)
{
    // 绑定非静态的物体
    for (auto& b : binds)
    {
        b->Bind(gfx);
    }
    // 绑定静态物体，也就是需要多次创建的相同物体，通过GetStaticBinds虚函数实现
    // 具体是在DrawableBase中实现，然后根据模板类型返回对应的staticBinds
    for (auto& b : GetStaticBinds())
    {
        b->Bind(gfx);
    }
    gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
{
    assert("Must use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
    binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> idBuffer) noexcept(!IS_DEBUG)
{
    assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
    pIndexBuffer = idBuffer.get();
    binds.push_back(std::move(idBuffer));
}
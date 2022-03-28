#include "Drawable.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"
#include "Graphics/Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(D3D11Graphics &gfx) const noexcept(false)
{
    for (auto& b : binds)
    {
        b->Bind(gfx);
    }
    gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
{
    assert("*Must use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
    binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> idBuffer) noexcept
{
    assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
    pIndexBuffer = idBuffer.get();
    binds.push_back(std::move(idBuffer));
}
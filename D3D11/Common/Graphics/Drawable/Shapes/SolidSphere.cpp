#include "SolidSphere.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"
#include "Sphere.h"

SolidSphere::SolidSphere(D3D11Graphics &gfx, float radius) {
    namespace dx = DirectX;

    if (!IsStaticInitialized()) {
        struct Vertex {
            dx::XMFLOAT3 pos;
        };
        auto model = Sphere::Make<Vertex>();
        model.Transform(dx::XMMatrixScaling(radius, radius, radius));
        AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
        AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        auto pvs = std::make_unique<VertexShader>(gfx, L"./Assets/Shaders/SolidVS.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"./Assets/Shaders/SolidPS.cso"));

        struct PSColorConstant {
            dx::XMFLOAT3 color = {1.0f, 1.0f, 1.0f};
            float padding;
        } colorConst;
        AddStaticBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    } else {
        SetIndexFromStatic();
    }

    AddBind(std::make_unique<TransformCBuffer>(gfx, *this));
}

void SolidSphere::Update(float DeltaTime) noexcept {

}

void SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept {
    this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept {
    return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

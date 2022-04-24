#include "PointLight.h"
#include "Imgui/imgui.h"

PointLight::PointLight(D3D11Graphics &gfx, float radius) :
    mesh(gfx, radius),
    cBuffer(gfx)
{
    Reset();
}

void PointLight::SpawnControlWindow() noexcept {
    if (ImGui::Begin("Light")) {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &pointLightCBData.pos.x, -60.0f, 60.0f, "%.1f");
        ImGui::SliderFloat("Y", &pointLightCBData.pos.y, -60.0f, 60.0f, "%.1f");
        ImGui::SliderFloat("Z", &pointLightCBData.pos.z, -60.0f, 60.0f, "%.1f");

        ImGui::Text("Intensity/Color");
        ImGui::SliderFloat("Intensity", &pointLightCBData.diffuseIntensity, 0.01f, 2.0f, "%.2f");
        ImGui::ColorEdit3("Diffuse Color", &pointLightCBData.diffuseColor.x);
        ImGui::ColorEdit3("Ambient", &pointLightCBData.ambient.x);

        ImGui::Text( "Falloff" );
        ImGui::SliderFloat("Constant", &pointLightCBData.attenConst, 0.05f, 10.0f, "%.2f");
        ImGui::SliderFloat("Linear", &pointLightCBData.attenLin, 0.0001f, 4.0f, "%.4f");
        ImGui::SliderFloat("Quadratic", &pointLightCBData.attenQuad, 0.0000001f, 10.0f, "%.7f");

        if (ImGui::Button("Reset")) {
            Reset();
        }
    }
    ImGui::End();
}

void PointLight::Reset() noexcept {
    pointLightCBData = {
        {0.0f, 0.0f, 0.0f},
        {0.05f, 0.05f, 0.05f},
        { 1.0f,1.0f,1.0f },
        1.0f,
        1.0f,
        0.045f,
        0.0075f,
    };
}

void PointLight::Draw(D3D11Graphics &gfx) const noexcept(!IS_DEBUG) {
    mesh.SetPos(pointLightCBData.pos);
    mesh.Draw(gfx);
}

void PointLight::Bind(D3D11Graphics &gfx, DirectX::XMMATRIX view) const noexcept {
    auto dataCopy = pointLightCBData;
    const auto pos = DirectX::XMLoadFloat3(&pointLightCBData.pos);
    DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
    cBuffer.Update(gfx, dataCopy);
    cBuffer.Bind(gfx);
}

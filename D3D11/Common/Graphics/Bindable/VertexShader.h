#pragma once
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
    VertexShader(D3D11Graphics& gfx, const std::wstring& path);
    void Bind(D3D11Graphics& gfx) noexcept override;
    ID3DBlob* GetBytecode() const noexcept;

protected:
    Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};
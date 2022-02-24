#pragma once

#include <d3d11.h>
#include "Utility/D3D11Win.h"

class D3D11Graphics
{
public:
    explicit D3D11Graphics(HWND hWnd);
    D3D11Graphics(const D3D11Graphics&) = delete;
    D3D11Graphics& operator=(const D3D11Graphics&) = delete;
    ~D3D11Graphics();
    void EndFrame();
    void ClearBuffer(float r, float g, float b) noexcept;

private:
    ID3D11Device* pDevice = nullptr;
    IDXGISwapChain* pSwap = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    ID3D11RenderTargetView* pRenderTraget = nullptr;
};
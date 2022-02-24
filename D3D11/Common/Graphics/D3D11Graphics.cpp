#include "D3D11Graphics.h"

D3D11Graphics::D3D11Graphics(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    // 创建前后缓冲
    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &pSwap,
        &pDevice,
        nullptr,
        &pContext
        );

    ID3D11Resource* pBackBuffer = nullptr;
    pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
    pDevice->CreateRenderTargetView(
        pBackBuffer,
        nullptr,
        &pRenderTraget
        );
    pBackBuffer->Release();
}

D3D11Graphics::~D3D11Graphics()
{
    if (pRenderTraget != nullptr) pRenderTraget->Release();
    if (pContext != nullptr) pContext->Release();
    if (pSwap != nullptr) pSwap->Release();
    if (pDevice != nullptr) pDevice->Release();
}

void D3D11Graphics::EndFrame()
{
    pSwap->Present(1u, 0u);
}

void D3D11Graphics::ClearBuffer(float r, float g, float b) noexcept
{
    const float color[] = {r, g, b, 1.0f};
    pContext->ClearRenderTargetView(pRenderTraget, color);
}
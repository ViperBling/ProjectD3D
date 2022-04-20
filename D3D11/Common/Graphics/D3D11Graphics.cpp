#include "D3D11Graphics.h"
#include "Utility/dxerr.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"
#include "Imgui/backends/imgui_impl_dx11.h"
#include "Imgui/backends/imgui_impl_win32.h"
#include <sstream>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

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

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // 检查D3D函数结果
    HRESULT hr;

    GFX_THROW_INFO(
    // 创建前后缓冲
        D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            swapCreateFlags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &pSwap,
            &pDevice,
            nullptr,
            &pContext
        ));

    // gain access to texture subresource in swap chain (back buffer)
    wrl::ComPtr<ID3D11Resource> pBackBuffer;
    GFX_THROW_INFO( pSwap->GetBuffer(
        0,
        __uuidof(ID3D11Resource),
        &pBackBuffer) );
    GFX_THROW_INFO( pDevice->CreateRenderTargetView(
        pBackBuffer.Get(),
        nullptr, &pRenderTraget ) );

    // 创建depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    wrl::ComPtr<ID3D11DepthStencilState> pDSState;
    GFX_THROW_INFO(pDevice->CreateDepthStencilState(&depthStencilDesc, &pDSState));

    // 绑定depth state
    pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

    // create depth stencil texture
    wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = 800u;
    descDepth.Height = 600u;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO( pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

    // create view of depth stencil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;
    GFX_THROW_INFO( pDevice->CreateDepthStencilView(
        pDepthStencil.Get(),&descDSV,&pDSV
    ));

    // bind depth stensil view to OM
    pContext->OMSetRenderTargets(1u, pRenderTraget.GetAddressOf(), pDSV.Get());

    // configure viewport
    D3D11_VIEWPORT viewport;
    viewport.Width = 800.0f;
    viewport.Height = 600.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    pContext->RSSetViewports(1u, &viewport);

    // init imgui d3d impl
    ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

void D3D11Graphics::BeginFrame(float red, float green, float blue) noexcept {
    // imgui begin frame
    if (imguiEnabled) {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
    const float color[] = {red, green, blue, 1.0f};
    pContext->ClearRenderTargetView(pRenderTraget.Get(), color);
    pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void D3D11Graphics::EndFrame()
{
    // imgui frame end
    if (imguiEnabled) {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    HRESULT hr;
#ifndef NDEBUG
    infoManager.Set();
#endif
    if( FAILED( hr = pSwap->Present( 1u,0u ) ) )
    {
        if( hr == DXGI_ERROR_DEVICE_REMOVED )
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else
        {
            throw GFX_EXCEPT( hr );
        }
    }
}

void D3D11Graphics::DrawIndexed(UINT count) noexcept(!IS_DEBUG)
{
    GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
}

void D3D11Graphics::SetProjection(DirectX::XMMATRIX proj) noexcept
{
    projection = proj;
}

DirectX::XMMATRIX D3D11Graphics::GetProjection() const noexcept
{
    return projection;
}

void D3D11Graphics::EnableImgui() noexcept {
    imguiEnabled = true;
}

void D3D11Graphics::DisableImgui() noexcept {
    imguiEnabled = false;
}

bool D3D11Graphics::IsImguiEnabled() const noexcept {
    return imguiEnabled;
}

void D3D11Graphics::SetCamera(DirectX::XMMATRIX cam) noexcept {
    camera = cam;
}

DirectX::XMMATRIX D3D11Graphics::GetCamera() const noexcept {
    return camera;
}

D3D11Graphics::~D3D11Graphics() {
    ImGui_ImplDX11_Shutdown();
}

D3D11Graphics::HRException::HRException(
    int line, const char *file,
    HRESULT hr,
    std::vector<std::string> infoMsgs) noexcept :
    Exception(line, file),
    hr(hr)
{
    // join all info messages with newlines into single string
    for( const auto& m : infoMsgs )
    {
        info += m;
        info.push_back( '\n' );
    }
    // remove final newline if exists
    if( !info.empty() )
    {
        info.pop_back();
    }
}

const char *D3D11Graphics::HRException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;
    if(!info.empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *D3D11Graphics::HRException::GetType() const noexcept
{
    return "D3D11 Graphics Exception";
}

HRESULT D3D11Graphics::HRException::GetErrorCode() const noexcept
{
    return hr;
}

std::string D3D11Graphics::HRException::GetErrorString() const noexcept
{
    return DXGetErrorString(hr);
}

std::string D3D11Graphics::HRException::GetErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(hr, buf, sizeof( buf ));
    return buf;
}

std::string D3D11Graphics::HRException::GetErrorInfo() const noexcept
{
    return info;
}

const char *D3D11Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "D3D11 Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

D3D11Graphics::InfoException::InfoException(int line, const char *file, std::vector<std::string> infoMsgs) noexcept :
    Exception(line, file)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char *D3D11Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *D3D11Graphics::InfoException::GetType() const noexcept
{
    return "D3D11 Graphics Info Exception";
}

std::string D3D11Graphics::InfoException::GetErrorInfo() const noexcept
{
    return info;
}



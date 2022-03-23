#include "D3D11Graphics.h"
#include "Utility/dxerr.h"
#include <sstream>
#include <d3dcompiler.h>

namespace wrl = Microsoft::WRL;

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) D3D11Graphics::HRException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw D3D11Graphics::HRException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) D3D11Graphics::HRException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) D3D11Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw D3D11Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) D3D11Graphics::HRException(__LINE__, __FILE__, (hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) D3D11Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

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
}

void D3D11Graphics::EndFrame()
{
    HRESULT hr;
#ifndef NDEBUG
    infoManager.Set();
#endif
    if( FAILED( hr = pSwap->Present( 1u,0u ) ) )
    {
        if( hr == DXGI_ERROR_DEVICE_REMOVED )
        {
            throw GFX_DEVICE_REMOVED_EXCEPT( pDevice->GetDeviceRemovedReason() );
        }
        else
        {
            throw GFX_EXCEPT( hr );
        }
    }
}

void D3D11Graphics::ClearBuffer(float r, float g, float b) noexcept
{
    const float color[] = {r, g, b, 1.0f};
    pContext->ClearRenderTargetView(pRenderTraget.Get(), color);
}

void D3D11Graphics::DrawTestTriangle()
{
    namespace wrl = Microsoft::WRL;
    HRESULT hr;

    struct Vertex
    {
        float x;
        float y;
    };

    // 创建顶点缓冲
    const Vertex vertices[] =
    {
        {0.0f, 0.5f},
        {0.5f, -0.5f},
        {-0.5f, -0.5f},
    };

    // 创建Buffer
    wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.CPUAccessFlags = 0u;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem = vertices;
    GFX_THROW_INFO(pDevice->CreateBuffer(
        &bufferDesc,
        &subresourceData,
        &pVertexBuffer));

    // 绑定顶点缓冲
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);

    // 创建VS
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"../../Shaders/VertexShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreateVertexShader(
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        nullptr,
        &pVertexShader));

    // 绑定VS
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

    GFX_THROW_INFO_ONLY(pContext->Draw((UINT)std::size(vertices), 0u));
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



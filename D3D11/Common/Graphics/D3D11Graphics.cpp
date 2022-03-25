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
    HRESULT hr;

    struct Vertex
    {
        struct
        {
            float x;
            float y;
        } pos;
        struct
        {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        } color;
    };

    // 创建顶点缓冲
    Vertex vertices[] =
    {
        { 0.0f,  0.5f, 255, 0,   0,   0},
        { 0.5f, -0.5f, 0,   255, 0,   0},
        {-0.5f, -0.5f, 0,   0,   255, 0},
        {-0.3f,  0.3f, 0,   255, 0,   0},
        { 0.3f,  0.3f, 0,   0,   255, 0},
        { 0.0f, -0.8f, 255, 0,   0,   0},
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
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

    // 创建Index Buffer
    const unsigned short indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 1,
        2, 1, 5,
    };
    wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.CPUAccessFlags = 0u;
    indexBufferDesc.MiscFlags = 0u;
    indexBufferDesc.ByteWidth = sizeof(indices);
    indexBufferDesc.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA indexSubData = {};
    indexSubData.pSysMem = indices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&indexBufferDesc, &indexSubData, &pIndexBuffer));
    // 绑定IndexBuffer
    pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

    // 创建PS
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"../../Shaders/PixelShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreatePixelShader(
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        nullptr,
        &pPixelShader));
    // 绑定PS
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

    // 创建VS
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;

    GFX_THROW_INFO(D3DReadFileToBlob(L"../../Shaders/VertexShader.cso", &pBlob));
    GFX_THROW_INFO(pDevice->CreateVertexShader(
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        nullptr,
        &pVertexShader));

    // 绑定VS
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

    // input layout
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    GFX_THROW_INFO(pDevice->CreateInputLayout(
        inputElementDesc,
        (UINT)std::size(inputElementDesc),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
        ));

    // bind vertex layout
    pContext->IASetInputLayout(pInputLayout.Get());

    // bind render target
    pContext->OMSetRenderTargets(1u, pRenderTraget.GetAddressOf(), nullptr);

    // Set primitive topology to triangle list (groups of 3 vertices)
    pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // configure viewport
    D3D11_VIEWPORT viewport;
    viewport.Width = 800;
    viewport.Height = 600,
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    pContext->RSSetViewports(1u, &viewport);

    GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
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



#pragma once

#include "Utility/D3D11Win.h"
#include "Utility/D3D11Exception.h"
#include "DXGIInfoManager.h"

#include <wrl.h>
#include <d3d11.h>
#include <vector>

class D3D11Graphics
{
public:
    class Exception : public D3D11Exception
    {
        using D3D11Exception::D3D11Exception;
    };
    class HRException : public Exception
    {
    public:
        HRException(
            int line,
            const char* file,
            HRESULT hr,
            std::vector<std::string> infoMsgs = {}) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
        std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;
    private:
        HRESULT hr;
        std::string info;
    };

    class InfoException : public Exception
    {
    public:
        InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        std::string GetErrorInfo() const noexcept;
    private:
        std::string info;
    };

    class DeviceRemovedException : public HRException
    {
        using HRException::HRException;
    public:
        const char* GetType() const noexcept override;
    private:
        std::string reason;
    };

public:
    explicit D3D11Graphics(HWND hWnd);
    D3D11Graphics(const D3D11Graphics&) = delete;
    D3D11Graphics& operator=(const D3D11Graphics&) = delete;
    ~D3D11Graphics() = default;
    void EndFrame();
    void ClearBuffer(float r, float g, float b) noexcept;

    // Just a Test
    void DrawTestTriangle(float angle, float x, float z);

private:
#ifndef NDEBUG
    DXGIInfoManager infoManager;
#endif
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTraget;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};
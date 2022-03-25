#include "DXGIInfoManager.h"
#include "Utility/Window.h"
#include "Utility/Marcos/WindowsThrowMacros.h"
#include "Utility/Marcos/GraphicsThrowMarcos.h"
#include "D3D11Graphics.h"
#include <dxgidebug.h>
#include <memory>

DXGIInfoManager::DXGIInfoManager()
{
    typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);

    const auto hModDXGIDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if( hModDXGIDebug == nullptr )
    {
        throw D3D11WND_LAST_EXCEPT();
    }

    // 获取DXGIGetDebugInterface的地址
    const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
        reinterpret_cast<void*>(GetProcAddress(
            hModDXGIDebug,"DXGIGetDebugInterface" ))
    );
    if(DxgiGetDebugInterface == nullptr)
    {
        throw D3D11WND_LAST_EXCEPT();
    }

    HRESULT hr;
    GFX_THROW_NOINFO(
        DxgiGetDebugInterface(
            __uuidof(IDXGIInfoQueue),
            &pDXGIInfoQueue ) );
}

void DXGIInfoManager::Set() noexcept
{
    // set the index (next) so that the next all to GetMessages()
    // will only get errors generated after this call
    next = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DXGIInfoManager::GetMessages() const
{
    std::vector<std::string> messages;
    const auto end = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    for( auto i = next; i < end; i++ )
    {
        HRESULT hr;
        SIZE_T messageLength;
        // get the size of message i in bytes
        GFX_THROW_NOINFO(
            pDXGIInfoQueue->GetMessage(
                DXGI_DEBUG_ALL, i,
                nullptr, &messageLength ) );
        // allocate memory for message
        auto bytes = std::make_unique<byte[]>( messageLength );
        auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
        // get the message and push its description into the vector
        GFX_THROW_NOINFO(
            pDXGIInfoQueue->GetMessage(
                DXGI_DEBUG_ALL, i,
                pMessage, &messageLength ) );
        messages.emplace_back( pMessage->pDescription );
    }
    return messages;
}

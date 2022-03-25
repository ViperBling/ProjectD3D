// 定义宏方便调用
#define D3D11WND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__,hr)
#define D3D11WND_LAST_EXCEPT() Window::HRException(__LINE__, __FILE__,GetLastError())
#define D3D11WND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )
add_library(D3D11Lib WinMain.cpp)
add_executable(D3D11 WinMain.cpp)

set_target_properties (D3D11 PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
set_target_properties (D3D11 PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
set_target_properties (D3D11 PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
set_target_properties (D3D11 PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows")

target_link_libraries(D3D11Lib user32 d3d11 d3dcompiler dxgi)
target_link_libraries(D3D11 D3D11Lib)
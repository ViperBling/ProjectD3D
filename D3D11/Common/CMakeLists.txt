set (CommonSource ${PROJECT_SOURCE_DIR}/Common)


add_library(D3D11Lib
        ${CommonSource}/Utility/D3D11Exception.cpp
        ${CommonSource}/Utility/Window.cpp
        ${CommonSource}/Utility/WindowsMessageMap.cpp

        ${CommonSource}/IO/Keyboard.cpp
        ${CommonSource}/IO/Mouse.cpp
        )

set_target_properties (D3D11 PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
set_target_properties (D3D11 PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
set_target_properties (D3D11 PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
set_target_properties (D3D11 PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows")

target_link_libraries(D3D11Lib user32 d3d11 d3dcompiler dxgi)
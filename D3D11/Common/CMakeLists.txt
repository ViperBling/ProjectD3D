set (CommonSource ${PROJECT_SOURCE_DIR}/Common)

aux_source_directory(${CommonSource}/Utility Utility_Source)
aux_source_directory(${CommonSource}/Graphics Graphics_Source)
aux_source_directory(${CommonSource}/Graphics/Bindable Bindable_Source)
aux_source_directory(${CommonSource}/Graphics/Drawable Drawable_Source)
aux_source_directory(${CommonSource}/Graphics/Drawable/Shapes Shapes_Source)
aux_source_directory(${CommonSource}/IO IO_Source)
aux_source_directory(${CommonSource}/Editor Editor_Source)

file (GLOB EXTERNAL_SRC
        ${PROJECT_SOURCE_DIR}/External/Imgui/*.h
        ${PROJECT_SOURCE_DIR}/External/Imgui/*.cpp
        ${PROJECT_SOURCE_DIR}/External/Imgui/backends/imgui_impl_win32.*
        ${PROJECT_SOURCE_DIR}/External/Imgui/backends/imgui_impl_dx11.*
        )


add_library(D3D11Lib
    ${EXTERNAL_SRC}
#    ${CommonSource}/Utility/D3D11Exception.cpp
#    ${CommonSource}/Utility/Timer.cpp
#    ${CommonSource}/Utility/Window.cpp
#    ${CommonSource}/Utility/WindowsMessageMap.cpp
#    ${CommonSource}/Utility/dxerr.cpp
    ${Utility_Source}

#    ${CommonSource}/IO/Keyboard.cpp
#    ${CommonSource}/IO/Mouse.cpp
    ${IO_Source}
    ${Editor_Source}

#    ${CommonSource}/Graphics/Bindable/Bindable.cpp
#    ${CommonSource}/Graphics/D3D11Graphics.cpp
#    ${CommonSource}/Graphics/DXGIInfoManager.cpp
    ${Graphics_Source}
    ${Bindable_Source}
    ${Drawable_Source}
    ${Shapes_Source}
)

set_target_properties (D3D11 PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
set_target_properties (D3D11 PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
set_target_properties (D3D11 PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
set_target_properties (D3D11 PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows")

target_link_libraries(D3D11Lib user32 d3d11 d3dcompiler dxgi dxguid gdiplus)
cmake_minimum_required (VERSION 3.1)
project (D3D11)

set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 17)

 set (CMAKE_CXX_FLAGS -EHsc)

set (APP_WINRC ${PROJECT_SOURCE_DIR}/Assets/Icon/AppIcon.rc)
set_property(SOURCE ${APP_WINRC} APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/Assets/Icon/AppIcon.ico)

add_executable(D3D11
        ${PROJECT_SOURCE_DIR}/Core/WinMain.cpp
        ${PROJECT_SOURCE_DIR}/Core/WindowsApplication.cpp
        ${APP_WINRC}
        )

target_link_libraries(D3D11 D3D11Lib)

#include (CTest)
include_directories(Common)

add_subdirectory(Common)
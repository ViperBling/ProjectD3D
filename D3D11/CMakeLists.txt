cmake_minimum_required (VERSION 3.1)
project (D3D11)

set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS -EHsc)

#if(CMAKE_BUID_TYPE STREQUAL Debug)
   set(IS_DEBUG true)
#endif()

include_directories(${PROJECT_SOURCE_DIR}/External)
include_directories(${PROJECT_SOURCE_DIR}/External/Imgui)

configure_file(CMakeConfig.h.in ${PROJECT_SOURCE_DIR}/Common/CMakeConfig.h)

set (APP_WINRC ${PROJECT_SOURCE_DIR}/Assets/Icon/AppIcon.rc)
set_property(SOURCE ${APP_WINRC} APPEND PROPERTY
        OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/Assets/Icon/AppIcon.ico)

add_executable(D3D11
        ${PROJECT_SOURCE_DIR}/Core/WinMain.cpp
        ${PROJECT_SOURCE_DIR}/Core/WindowsApplication.cpp
        ${APP_WINRC}
        )

target_link_libraries(D3D11 D3D11Lib)

# Build HLSL shaders
add_custom_target(Shaders)

set(HLSL_SHADER_FILES
    Shaders/ColorIndexVS.hlsl Shaders/ColorIndexPS.hlsl
    Shaders/ColorBlendVS.hlsl Shaders/ColorBlendPS.hlsl
    Shaders/TextureVS.hlsl Shaders/TexturePS.hlsl)

set_source_files_properties(Shaders/ColorIndexVS.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(Shaders/ColorBlendVS.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(Shaders/TextureVS.hlsl PROPERTIES ShaderType "vs")

set_source_files_properties(Shaders/ColorIndexPS.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(Shaders/ColorBlendPS.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(Shaders/TexturePS.hlsl PROPERTIES ShaderType "ps")

set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "5_0")

foreach(FILE ${HLSL_SHADER_FILES})
 get_filename_component(FILE_WE ${FILE} NAME_WE)
 get_source_file_property(shadertype ${FILE} ShaderType)
 get_source_file_property(shadermodel ${FILE} ShaderModel)
 add_custom_command(TARGET Shaders
     COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<$<CONFIG:DEBUG>:/Od> /Zi /Fo ${PROJECT_SOURCE_DIR}/Shaders/${FILE_WE}.cso /Fd ${PROJECT_SOURCE_DIR}/Shaders/${FILE_WE}.pdb ${FILE}
     MAIN_DEPENDENCY ${FILE}
     COMMENT "HLSL ${FILE}"
     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
     VERBATIM)
endforeach(FILE)
add_dependencies(${PROJECT_NAME} Shaders)

#include (CTest)
include_directories(Common)

add_subdirectory(Common)
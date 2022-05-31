cmake_minimum_required (VERSION 3.1)
project (D3D11)

set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS -EHsc)

if (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "None")
    set(BuildType Debug)
    add_compile_definitions(PEDEBUG)

elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    set(BuildType Release)
    add_compile_definitions(PERELEASE)

else ()
endif ()

include_directories(${PROJECT_SOURCE_DIR}/External)
include_directories(${PROJECT_SOURCE_DIR}/External/Imgui)


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
        Assets/Shaders/ColorBlendVS.hlsl Assets/Shaders/ColorBlendPS.hlsl
        Assets/Shaders/TextureVS.hlsl Assets/Shaders/TexturePS.hlsl
        Assets/Shaders/SolidVS.hlsl Assets/Shaders/SolidPS.hlsl
        Assets/Shaders/PhongVS.hlsl Assets/Shaders/PhongPS.hlsl
        Assets/Shaders/IndexedPhongPS.hlsl
        Assets/Shaders/BlendedPhongVS.hlsl Assets/Shaders/BlendedPhongPS.hlsl)

set_source_files_properties(Assets/Shaders/ColorBlendVS.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(Assets/Shaders/TextureVS.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(Assets/Shaders/SolidVS.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(Assets/Shaders/PhongVS.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(Assets/Shaders/BlendedPhongVS.hlsl PROPERTIES ShaderType "vs")

set_source_files_properties(Assets/Shaders/ColorBlendPS.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(Assets/Shaders/TexturePS.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(Assets/Shaders/SolidPS.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(Assets/Shaders/PhongPS.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(Assets/Shaders/IndexedPhongPS.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(Assets/Shaders/BlendedPhongPS.hlsl PROPERTIES ShaderType "ps")

set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "5_0")

foreach(FILE ${HLSL_SHADER_FILES})
 get_filename_component(FILE_WE ${FILE} NAME_WE)
 get_source_file_property(shadertype ${FILE} ShaderType)
 get_source_file_property(shadermodel ${FILE} ShaderModel)
 add_custom_command(TARGET Shaders
     COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<$<CONFIG:DEBUG>:/Od> /Zi /Fo Assets/Shaders/${FILE_WE}.cso /Fd Assets/Shaders/${FILE_WE}.pdb ${FILE}
     MAIN_DEPENDENCY ${FILE}
     COMMENT "HLSL ${FILE}"
     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
     VERBATIM)
endforeach(FILE)
add_dependencies(${PROJECT_NAME} Shaders)

#include (CTest)
include_directories(Common)

add_subdirectory(Common)
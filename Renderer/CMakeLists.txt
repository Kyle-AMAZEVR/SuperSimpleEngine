cmake_minimum_required(VERSION 3.0.0)
project(DXEngine)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)


###########################################################
################ Engine					 ################
file(GLOB SOURCES "./*.cpp")
file(GLOB HEADERS "./*.h")

include_directories("../lib/DirectXMath/Include")
include_directories("../lib/DirectXTex/DirectXTex")
include_directories("../lib/freetype/include")
include_directories("../lib/simdjson")
include_directories("../Core")
include_directories("../HeaderOnly")
include_directories("../lib/bin")

add_library(DX11Renderer SHARED ${SOURCES} ${HEADERS})

target_link_libraries(DX11Renderer d3d11.lib d3d12.lib d3dcompiler.lib dxguid.lib dxgi.lib 
$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/lib/bin/directxtex/Debug/directxtex.lib>
$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/lib/bin/directxtex/Release/directxtex.lib>
$<$<CONFIG:RelWithDebInfo>:${CMAKE_SOURCE_DIR}/lib/bin/directxtex/RelWithDebInfo/directxtex.lib>
$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/lib/bin/freetype/Debug/freetyped.lib>
$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/lib/bin/freetype/Release/freetype.lib>
$<$<CONFIG:RelWithDebInfo>:${CMAKE_SOURCE_DIR}/lib/bin/freetype/RelWithDebInfo/freetype.lib>
GameModule simdjson)


target_precompile_headers(DX11Renderer PRIVATE "SSRendererModulePCH.h")

target_compile_definitions(DX11Renderer PRIVATE DX11RENDERER_EXPORTS)
target_compile_definitions(DX11Renderer INTERFACE DX11RENDERER_IMPORTS)

target_compile_options(DX11Renderer PRIVATE /GR-)

###########################################################



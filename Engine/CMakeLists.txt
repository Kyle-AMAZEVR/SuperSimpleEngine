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

#include_directories("lib/DirectXTK/Inc")
include_directories("../lib/DirectXTex/DirectXTex")
include_directories("../lib/freetype/include")
include_directories("../GameModule")
include_directories("../lib/simdjson")

#add_subdirectory("lib/DirectXTK")
add_subdirectory("../lib/DirectXTex" "./DirectXTex")
add_subdirectory("../lib/freetype" "./freetype")
add_subdirectory("../lib/simdjson" "./simdjson")

add_library(DX11Renderer SHARED ${SOURCES} ${HEADERS})

add_dependencies(DX11Renderer GameModule)

target_link_libraries(DX11Renderer d3d11.lib d3d12.lib d3dcompiler.lib dxguid.lib dxgi.lib directxtex freetype GameModule simdjson)

target_precompile_headers(DX11Renderer PRIVATE "Core.h")

target_compile_definitions(DX11Renderer PRIVATE DX11RENDERER_EXPORTS)
target_compile_definitions(DX11Renderer INTERFACE DX11RENDERER_IMPORTS)


###########################################################



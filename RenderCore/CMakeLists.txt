cmake_minimum_required(VERSION 3.0.0)
project(DXEngine)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 20)


###########################################################
################ Engine					 ################
file(GLOB SOURCES "./*.cpp")
file(GLOB HEADERS "./*.h")

include_directories("../lib/DirectXMath/Include")
include_directories("../lib/DirectXTex/DirectXTex")
include_directories("../Core")
include_directories("../lib/bin")
include_directories("../HeaderOnly")

add_library(RenderCore SHARED ${SOURCES} ${HEADERS})

target_link_libraries(RenderCore d3d11.lib d3d12.lib d3dcompiler.lib dxguid.lib dxgi.lib 
$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/lib/bin/directxtex/Debug/directxtex.lib>
$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/lib/bin/directxtex/Release/directxtex.lib>
$<$<CONFIG:RelWithDebInfo>:${CMAKE_SOURCE_DIR}/lib/bin/directxtex/RelWithDebInfo/directxtex.lib>
Core)

target_precompile_headers(RenderCore PRIVATE "SSRenderCore.h")

target_compile_definitions(RenderCore PRIVATE RENDERCORE_EXPORTS)
target_compile_definitions(RenderCore INTERFACE RENDERCORE_IMPORTS)

target_compile_options(RenderCore PRIVATE /GR-)

###########################################################



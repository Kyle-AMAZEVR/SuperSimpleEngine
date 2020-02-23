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

foreach(src_file ${SOURCES})
	set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/YuSSCommon.h")	
endforeach(src_file ${SOURCES})

set_source_files_properties("SSCommon.cpp" PROPERTIES COMPILE_FLAGS "/YcSSCommon.h")

add_library(Common SHARED ${SOURCES} ${HEADERS})

target_link_libraries(Common d3d11.lib d3d12.lib d3dcompiler.lib dxguid.lib dxgi.lib directxtex freetype) 

target_compile_definitions(Common PRIVATE COMMON_EXPORTS)
target_compile_definitions(Common INTERFACE COMMON_IMPORTS)




###########################################################



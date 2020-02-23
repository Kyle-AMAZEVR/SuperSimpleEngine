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
include_directories("../Common")

add_subdirectory("../Common/" "./Common")

foreach(src_file ${SOURCES})
	set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/YuSSDX12.h")	
endforeach(src_file ${SOURCES})

set_source_files_properties("SSDX12.cpp" PROPERTIES COMPILE_FLAGS "/YcSSDX12.h")

add_library(SSDX12 SHARED ${SOURCES} ${HEADERS})

add_dependencies(SSDX12 Common)

target_link_libraries(SSDX12 d3d11.lib d3d12.lib d3dcompiler.lib dxguid.lib dxgi.lib directxtex freetype Common) 

target_compile_definitions(SSDX12 PRIVATE SSDX12_EXPORTS)
target_compile_definitions(SSDX12 INTERFACE SSDX12_IMPORTS)


###########################################################



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

#add_subdirectory("lib/DirectXTK")
add_subdirectory("../lib/DirectXTex" "./DirectXTex")
add_subdirectory("../lib/freetype" "./freetype")

foreach(src_file ${SOURCES})
	set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/YuCore.h")	
endforeach(src_file ${SOURCES})

set_source_files_properties("Core.cpp" PROPERTIES COMPILE_FLAGS "/YcCore.h")

add_library(Core SHARED ${SOURCES} ${HEADERS})

target_link_libraries(Core d3d11.lib d3d12.lib d3dcompiler.lib dxguid.lib directxtex freetype) 

target_compile_definitions(Core PRIVATE ENGINE_EXPORTS)
target_compile_definitions(Core INTERFACE ENGINE_IMPORTS)


###########################################################



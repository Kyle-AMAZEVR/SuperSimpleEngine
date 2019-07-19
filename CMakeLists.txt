cmake_minimum_required(VERSION 3.0.0)
project(DXEngine)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)


###########################################################
################ Engine					 ################
file(GLOB SOURCES "Engine/*.cpp")
file(GLOB HEADERS "Engine/*.h")

include_directories("Engine")
include_directories("lib/DirectXMath/Include")

foreach(src_file ${SOURCES})
	set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "/YuCore.h")	
endforeach(src_file ${SOURCES})

set_source_files_properties("Engine/Core.cpp" PROPERTIES COMPILE_FLAGS "/YcCore.h")

add_library(Core SHARED ${SOURCES} ${HEADERS})

target_link_libraries(Core d3d11.lib d3dcompiler.lib dxguid.lib)

target_compile_definitions(Core PRIVATE ENGINE_EXPORTS)
target_compile_definitions(Core INTERFACE ENGINE_IMPORTS)
###########################################################

add_executable(SuperSimpleEngine resource.h targetver.h DXWindow.h DXWindow.cpp DXWindow.rc)

if(WIN32)
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
endif(WIN32)



target_link_libraries(SuperSimpleEngine 
Core
 )

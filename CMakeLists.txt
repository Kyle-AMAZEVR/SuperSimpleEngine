cmake_minimum_required(VERSION 3.0.0)
project(SuperSimpleEngine)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 20)

set(SIMDJSON_JUST_LIBRARY 1)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


message(STATUS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory("Renderer")
add_subdirectory("Core")
add_subdirectory("lib/simdjson")
add_subdirectory("Engine")
add_subdirectory("RenderCore")
add_subdirectory("GameModule")
#add_subdirectory("Engine12")


include_directories("HeaderOnly")
include_directories("Engine")
include_directories("Renderer")
include_directories("GameModule")
include_directories("Core")
include_directories("RenderCore")



add_executable(SuperSimpleEngine resource.h targetver.h SSAppWindow.h SSAppWindow.cpp SSApplication.cpp DXWindow.rc)

if(WIN32)
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")	
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")	
endif(WIN32)


target_link_libraries(SuperSimpleEngine PRIVATE Engine)

target_compile_options(SuperSimpleEngine PRIVATE /GR-)

target_compile_options(SuperSimpleEngine PRIVATE /EHs-c-)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lib/bin DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



###########################################################

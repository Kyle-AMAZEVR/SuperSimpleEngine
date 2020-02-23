cmake_minimum_required(VERSION 3.0.0)
project(DXEngine)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories("Engine")
include_directories("Engine12")
include_directories("Common")

message(STATUS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory("Engine")
add_subdirectory("Engine12")


add_executable(SuperSimpleEngine resource.h targetver.h DXWindow.h DXWindow.cpp DXWindow.rc)

add_dependencies(SuperSimpleEngine SSDX11)
add_dependencies(SuperSimpleEngine SSDX12)


if(WIN32)
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
endif(WIN32)


target_link_libraries(SuperSimpleEngine Core)
target_link_libraries(SuperSimpleEngine SSDX12)

#INSTALL( TARGETS SuperSimpleEngine Engine DESTINATION ${CMAKE_BUILD_TYPE})


add_custom_command(TARGET SuperSimpleEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Shader $<TARGET_FILE_DIR:SuperSimpleEngine>/Shader)
add_custom_command(TARGET SuperSimpleEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resource $<TARGET_FILE_DIR:SuperSimpleEngine>/Resource)



###########################################################

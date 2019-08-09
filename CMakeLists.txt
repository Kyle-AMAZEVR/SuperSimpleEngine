cmake_minimum_required(VERSION 3.0.0)
project(DXEngine)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)

include_directories("Engine")

add_subdirectory("Engine")

add_executable(SuperSimpleEngine resource.h targetver.h DXWindow.h DXWindow.cpp DXWindow.rc)


if(WIN32)
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
	set_target_properties(SuperSimpleEngine PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
endif(WIN32)


target_link_libraries(SuperSimpleEngine Core)

add_custom_command(TARGET SuperSimpleEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Shader $<TARGET_FILE_DIR:SuperSimpleEngine>/Shader)
add_custom_command(TARGET SuperSimpleEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resource $<TARGET_FILE_DIR:SuperSimpleEngine>/Resource)


###########################################################

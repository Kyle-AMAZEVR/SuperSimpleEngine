cmake_minimum_required(VERSION 3.0.0)
project(CMakeTest VERSION 0.1.0)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)

add_library(message STATIC Message.h Message.cpp )


add_executable(CMakeTest resource.h DXWindow.h DXWindow.cpp DXEngine.cpp)


if(WIN32)
	set_target_properties(CMakeTest PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
	set_target_properties(CMakeTest PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
endif(WIN32)

target_link_libraries(CMakeTest message)


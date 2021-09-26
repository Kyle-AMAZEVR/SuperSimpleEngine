cmake_minimum_required(VERSION 3.0.0)
project(Core_CMAKE)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "./*.cpp")
file(GLOB HEADERS "./*.h")


add_library(Core SHARED ${SOURCES} ${HEADERS})

target_compile_definitions(Core PRIVATE CORE_EXPORTS)
target_compile_definitions(Core INTERFACE CORE_IMPORTS)




###########################################################



cmake_minimum_required(VERSION 3.0.0)

add_compile_options(/wd4251)

set(CMAKE_EXE_LINKER_FLAGS "/machine:x64")
set(CMAKE_MODULE_LINKER_FLAGS "/machine:x64")
set(CMAKE_SHARED_LINKER_FLAGS "/machine:x64")
set(CMAKE_STATIC_LINKER_FLAGS "/machine:x64")
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "./*.cpp")
file(GLOB HEADERS "./*.h")


include_directories("../Renderer")
include_directories("../GameModule")
include_directories("../HeaderOnly")
include_directories("../Core")

add_library(Engine SHARED ${SOURCES} ${HEADERS})

target_compile_definitions(Engine PRIVATE ENGINE_EXPORTS)
target_compile_definitions(Engine INTERFACE ENGINE_IMPORTS)

target_link_libraries(Engine GameModule DX11Renderer)


###########################################################




cmake_minimum_required(VERSION 3.21)
project(absinthe)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(conan)
conan_cmake_autodetect(settings)
conan_cmake_run(
    CONANFILE ${CMAKE_SOURCE_DIR}/conanfile.txt
    INSTALL_FOLDER ${CMAKE_BINARY_DIR}/conan
    BASIC_SETUP
    CMAKE_TARGETS
    BUILD missing
    SETTINGS ${settings}
)
include(${CMAKE_BINARY_DIR}/conan/conan_paths.cmake)

include_directories(include)

add_subdirectory(src)
add_subdirectory(tools)

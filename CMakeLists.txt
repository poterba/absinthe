
cmake_minimum_required(VERSION 3.16)
project(absinthe)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Install Dependencies

include(conan)
conan_cmake_autodetect(settings)
conan_cmake_run(
    CONANFILE ${CMAKE_SOURCE_DIR}/conanfile.txt
    INSTALL_FOLDER ${CMAKE_BINARY_DIR}/conan
    BASIC_SETUP
    CMAKE_TARGETS
    QUIET
    BUILD missing
    SETTINGS ${settings}
)
include(${CMAKE_BINARY_DIR}/conan/conan_paths.cmake)

# Check and Find Dependencies

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(BZip2 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(libimobiledevice REQUIRED)

include_directories(include)

add_subdirectory(src)
add_subdirectory(tools)

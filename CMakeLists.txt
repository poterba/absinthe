
cmake_minimum_required(VERSION 3.16)
project(absinthe C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(absinthe_GENERATE_DIR ${CMAKE_BINARY_DIR}/gen/)

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
    SETTINGS ${settings})
include(${CMAKE_BINARY_DIR}/conan/conan_paths.cmake)

# Check and Find Dependencies

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(BZip2 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Filesystem REQUIRED)

find_package(libimobiledevice REQUIRED)
find_package(libplist REQUIRED)

if(APPLE)
  find_library(FOUNDATION Foundation)
  if (NOT FOUNDATION)
    message(FATAL_ERROR "Foundation not found")
  endif()
endif()

include_directories(include)
configure_file(include/fswrapper.h.in ${absinthe_GENERATE_DIR}/include/fswrapper.h)
include_directories(${absinthe_GENERATE_DIR}/include)

add_subdirectory(src)
# add_subdirectory(tools)

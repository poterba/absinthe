
cmake_minimum_required(VERSION 3.16)

project(absinthe LANGUAGES C CXX VERSION 2.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(absinthe_GENERATE_DIR ${CMAKE_BINARY_DIR}/gen/)

# Install Dependencies

include(FindOpenSSL)
include(FindBZip2)
include(CPM)

# CPMAddPackage(
#   GIT_REPOSITORY https://gitlab.com/bzip2/bzip2.git
#   GIT_TAG bzip2-1.0.8
# )
CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 2.2.0
  OPTIONS
    "CXXOPTS_BUILD_EXAMPLES Off"
    "CXXOPTS_BUILD_TESTS Off"
)

# include(ExternalProject)

# ExternalProject_Add(libplist
#     DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}
#     URL https://github.com/libimobiledevice/libplist
#     UPDATE_COMMAND ${CMAKE_NOOP}
#     SOURCE_DIR ${make_lib_SOURCE_DIR}
#     CONFIGURE_COMMAND "./autogen.sh"
#     BUILD_COMMAND "make -j8"
#     INSTALL_COMMAND "${make_lib_install_commands}"
# )

# ExternalProject_Add(libimobiledevice
#     DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}
#     URL https://github.com/libimobiledevice/libimobiledevice
#     UPDATE_COMMAND ${CMAKE_NOOP}
#     SOURCE_DIR ${make_lib_SOURCE_DIR}
#     CONFIGURE_COMMAND ${CMAKE_NOOP}
#     BUILD_COMMAND "make -j8"
#     INSTALL_COMMAND "${make_lib_install_commands}"
# )
# TODO FIXME
# target_link_libraries(libimobiledevice INTERFACE libplist)

# Check and Find Dependencies

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)

find_package(Filesystem REQUIRED)
# find_package(tclap REQUIRED)

find_package(libplist REQUIRED)
find_package(libimobiledevice REQUIRED)

if(APPLE)
  find_library(FOUNDATION Foundation)
  if (NOT FOUNDATION)
    message(FATAL_ERROR "Foundation not found")
  endif()
endif()

include_directories(include)
configure_file(include/fswrapper.h.in ${absinthe_GENERATE_DIR}/include/fswrapper.h)
configure_file(include/version.h.in ${absinthe_GENERATE_DIR}/include/version.h)
include_directories(${absinthe_GENERATE_DIR}/include)

add_subdirectory(src)
# add_subdirectory(tools)

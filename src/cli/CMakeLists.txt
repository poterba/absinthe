
find_package(PkgConfig)
pkg_check_modules(PC_libimobiledevice QUIET libimobiledevice-1.0)

find_path(IMobileDevice_INCLUDE_DIRS NAMES libimobiledevice/libimobiledevice.h HINTS ${PC_libimobiledevice_INCLUDE_DIRS})
find_library(IMobileDevice_LIBRARIES NAMES imobiledevice HINTS ${PC_libimobiledevice_LIBRARY_DIRS})

file(GLOB SRC_FILES "*.c" "*.h")
add_executable(absinthe_cli ${SRC_FILES})

target_include_directories(absinthe_cli
PRIVATE
    ${IMobileDevice_INCLUDE_DIRS}
)

target_link_directories(absinthe_cli
PRIVATE
    absinthe_crashreport
    ${IMobileDevice_LIBRARIES}
)

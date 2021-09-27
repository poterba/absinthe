find_package(PkgConfig QUIET)
pkg_search_module(PC_libplist QUIET libplist-2.0 libplist)
find_path(PList_INCLUDE_DIRS NAMES plist/plist.h HINTS ${PC_libplist_INCLUDE_DIRS})

pkg_check_modules(PC_libimobiledevice QUIET libimobiledevice-1.0)
find_path(IMobileDevice_INCLUDE_DIRS NAMES libimobiledevice/libimobiledevice.h HINTS ${PC_libimobiledevice_INCLUDE_DIRS})
find_library(IMobileDevice_LIBRARIES NAMES imobiledevice HINTS ${PC_libimobiledevice_LIBRARY_DIRS})

file(GLOB SRC_FILES "*.c" "*.h")
add_library(absinthe_crashreport ${SRC_FILES})

target_include_directories(absinthe_crashreport
PRIVATE
    ${PList_INCLUDE_DIRS}
    ${IMobileDevice_INCLUDE_DIRS}
    absinthe_util
)

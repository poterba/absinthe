
find_package(PkgConfig)
pkg_check_modules(PC_libimobiledevice QUIET libimobiledevice-1.0)

find_path(IMobileDevice_INCLUDE_DIRS NAMES libimobiledevice/libimobiledevice.h HINTS ${PC_libimobiledevice_INCLUDE_DIRS})
find_library(IMobileDevice_LIBRARIES NAMES imobiledevice HINTS ${PC_libimobiledevice_LIBRARY_DIRS})

find_package(OpenSSL REQUIRED)

# library

add_library(absinthe
    architecture.c
    backup_file.c
    backup.c
    bpatch.c
    common.c
    device.c
    dictionary.c
    file.c
    fsgen.c
    idevicebackup2.c
    jailbreak.c
    lockdown.c
    mb1.c
    mbdb_record.c
    mbdb.c
    plist_extras.c
)

target_link_libraries(absinthe
    libimobiledevice
    OpenSSL::SSL
)

target_include_directories(absinthe
PUBLIC
    ${IMobileDevice_INCLUDE_DIRS}
)

add_subdirectory(cli)
add_subdirectory(crashreport)
add_subdirectory(dyld)
add_subdirectory(gui)
add_subdirectory(macho)

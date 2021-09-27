find_package(PkgConfig QUIET)
pkg_check_modules(PC_libimobiledevice QUIET libimobiledevice-1.0)
find_path(IMobileDevice_INCLUDE_DIRS NAMES libimobiledevice/libimobiledevice.h HINTS ${PC_libimobiledevice_INCLUDE_DIRS})
# find_library(IMobileDevice_LIBRARIES NAMES imobiledevice HINTS ${PC_libimobiledevice_LIBRARY_DIRS})

list(APPEND SOURCES_LIST
    main.cpp
    AbsintheWorker.cpp
    AbsintheJailbreaker.cpp
    iTunesKiller.cpp
)

set(DEPENDENCIES_LIST "")
if(WIN32)
    list(APPEND SOURCES_LIST AbsintheMainWnd_win32.cpp)
elseif(APPLE)
    list(APPEND SOURCES_LIST AbsintheMainWnd_osx.cpp)
elseif(UNIX)
    find_library(FOUNDATION Foundation)
    if (NOT FOUNDATION)
        message(FATAL_ERROR "Foundation not found")
    endif()

    list(APPEND DEPENDENCIES_LIST ${FOUNDATION})
    list(APPEND SOURCES_LIST AbsintheMainWnd_linux.cpp)
endif()

add_executable(absinthe_gui ${SOURCES_LIST})

target_link_libraries(absinthe_gui
    absinthe
    ${DEPENDENCIES_LIST}
)

target_include_directories(absinthe_gui
PRIVATE
    ${IMobileDevice_INCLUDE_DIRS}
)

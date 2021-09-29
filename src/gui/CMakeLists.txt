
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
    find_library(FOUNDATION Foundation)
    if (NOT FOUNDATION)
        message(FATAL_ERROR "Foundation not found")
    endif()

    list(APPEND DEPENDENCIES_LIST ${FOUNDATION})

    list(APPEND SOURCES_LIST AbsintheMainWnd_osx.cpp)
elseif(UNIX)
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


list(APPEND SOURCES_LIST
  main.cpp
  AbsintheWorker.cpp
  AbsintheJailbreaker.cpp
  iTunesKiller.cpp
)

set(DEPENDENCIES_LIST "")
set(INCLUDES_LIST "")
if(WIN32)
  list(APPEND SOURCES_LIST AbsintheMainWnd_win32.cpp)

elseif(APPLE)
  list(APPEND DEPENDENCIES_LIST ${FOUNDATION})
  list(APPEND SOURCES_LIST AbsintheMainWnd_osx.cpp)

elseif(UNIX)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

  list(APPEND DEPENDENCIES_LIST ${GTK3_LIBRARIES} Threads::Threads)
  list(APPEND SOURCES_LIST AbsintheMainWnd_linux.cpp)
endif()

add_executable(absinthe_gui ${SOURCES_LIST})

target_link_libraries(absinthe_gui
    ${DEPENDENCIES_LIST})

target_include_directories(absinthe_gui
  PRIVATE
    ${libimobiledevice_INCLUDE_DIR}
)

if(UNIX)
  target_include_directories(absinthe_gui
    PRIVATE
      ${GTK3_INCLUDE_DIRS}
  )
endif()

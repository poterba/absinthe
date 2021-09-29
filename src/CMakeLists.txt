
include_directories(${CMAKE_CURRENT_LIST_DIR}/backup)
include_directories(${CMAKE_CURRENT_LIST_DIR}/crashreport)
include_directories(${CMAKE_CURRENT_LIST_DIR}/dyld)
include_directories(${CMAKE_CURRENT_LIST_DIR}/util)

add_subdirectory(backup)
add_subdirectory(cli)
add_subdirectory(crashreport)
add_subdirectory(dyld)
add_subdirectory(gui)
add_subdirectory(macho)
add_subdirectory(util)

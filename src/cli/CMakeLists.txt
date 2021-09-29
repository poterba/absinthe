
file(GLOB SRC_FILES "*.c" "*.h")
add_executable(absinthe_cli ${SRC_FILES})

target_include_directories(absinthe_cli
  PRIVATE
    ${libimobiledevice_INCLUDE_DIR})

target_link_libraries(absinthe_cli
  PRIVATE
    absinthe_core
    absinthe_dyld
    absinthe_util
    ${libimobiledevice_LIBRARIES})

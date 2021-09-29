file(GLOB SRC_FILES "*.c" "*.h")
add_executable(absinthe_core ${SRC_FILES})

target_include_directories(absinthe_core
PRIVATE
    ${libplist_INCLUDE_DIR}
    ${libimobiledevice_INCLUDE_DIR}
)

target_link_libraries(absinthe_core
    absinthe_backup
    absinthe_crashreport
    absinthe_util
    ${libplist_LIBRARY}
    ${libimobiledevice_LIBRARIES}
)
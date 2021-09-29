file(GLOB SRC_FILES "*.c" "*.h")
add_executable(absinthe_core ${SRC_FILES})

target_include_directories(absinthe_core
PRIVATE
    ${PList_INCLUDE_DIRS}
    ${libimobiledevice_INCLUDE_DIR}
)

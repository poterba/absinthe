
file(GLOB SRC_FILES "*.c" "*.h")
add_library(absinthe_util ${SRC_FILES})

target_link_libraries(absinthe_util
    ${libimobiledevice_LIBRARIES}
)

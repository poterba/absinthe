
file(GLOB SRC_FILES "*.c" "*.h")
add_library(absinthe_util ${SRC_FILES})

target_include_directories(absinthe_util
PRIVATE
    BZip2_INCLUDE_DIRS
    ${libimobiledevice_INCLUDE_DIR}
)

target_link_libraries(absinthe_util
    BZip2::BZip2
    ${libimobiledevice_LIBRARIES}
    libc++fs
)

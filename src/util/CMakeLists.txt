
file(GLOB SRC_FILES "*.c" "*.h")
add_library(absinthe_util ${SRC_FILES})

target_link_libraries(absinthe_util
    ${libimobiledevice_LIBRARIES}
    BZip2::BZip2
)

target_include_directories(absinthe_util
PRIVATE
    BZip2_INCLUDE_DIRS
    ${libimobiledevice_INCLUDE_DIR}
)

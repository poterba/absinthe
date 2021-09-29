
file(GLOB SRC_FILES "*.c" "*.h")
add_library(absinthe_util ${SRC_FILES})

target_include_directories(absinthe_util
  PRIVATE
    ${BZIP2_INCLUDE_DIR}
    ${libimobiledevice_INCLUDE_DIR}
    ${libplist_INCLUDE_DIR})

target_link_libraries(absinthe_util
  PUBLIC
    BZip2::BZip2
    ${libimobiledevice_LIBRARIES})

target_link_libraries(absinthe_util
  PRIVATE
    std::filesystem)

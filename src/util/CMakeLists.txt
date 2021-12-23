
file(GLOB SRC_FILES *.cpp *.hpp)
add_library(absinthe_util ${SRC_FILES})

target_include_directories(absinthe_util
  PRIVATE
    ${libimobiledevice_INCLUDE_DIR}
    ${libplist_INCLUDE_DIR})

target_link_libraries(absinthe_util
  PUBLIC
    BZip2::BZip2
    ${libimobiledevice_LIBRARIES}
    ${libplist_LIBRARIES})

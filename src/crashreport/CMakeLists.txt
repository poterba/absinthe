
file(GLOB SRC_FILES *.cpp *.hpp)
add_library(absinthe_crashreport ${SRC_FILES})

target_include_directories(absinthe_crashreport
  PRIVATE
    ${PList_INCLUDE_DIRS}
    ${libimobiledevice_INCLUDE_DIR}
)

target_link_libraries(absinthe_crashreport
  PRIVATE
    absinthe_util
    ${libplist_LIBRARIES}
)


file(GLOB SRC_FILES *.cpp *.hpp)
add_executable(absinthe_cli ${SRC_FILES})

# target_include_directories(absinthe_cli
#   PRIVATE
#     ${libimobiledevice_INCLUDE_DIR}
#     ${tclap_INCLUDE_DIR})

target_link_libraries(absinthe_cli
  PRIVATE
    absinthe_core
    absinthe_dyld
    absinthe_util
    cxxopts
)

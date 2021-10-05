file(GLOB SRC_FILES *.cpp *.hpp)
add_library(absinthe_backup ${SRC_FILES})

target_link_libraries(absinthe_backup
  PRIVATE
    absinthe_util
    OpenSSL::OpenSSL)

target_include_directories(absinthe_backup
  PRIVATE
    ${OpenSSL_INCLUDE_DIR}
)

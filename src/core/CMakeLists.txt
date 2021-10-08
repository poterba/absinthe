file(GLOB SRC_FILES *.cpp *.hpp)
add_library(absinthe_core ${SRC_FILES})

target_include_directories(absinthe_core
  PUBLIC
    ${libplist_INCLUDE_DIR}
    ${libimobiledevice_INCLUDE_DIR}
)

target_link_libraries(absinthe_core
  PUBLIC
    ${libplist_LIBRARY}
    ${libimobiledevice_LIBRARIES}
  PRIVATE
    absinthe_backup
    absinthe_crashreport
    absinthe_util
    OpenSSL::Crypto)

if(UNIX AND NOT APPLE)
  target_link_libraries(absinthe_core PRIVATE atomic)
endif()

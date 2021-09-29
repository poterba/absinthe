
file(GLOB SRC_FILES "*.c" "*.h")
add_library(absinthe_crashreport ${SRC_FILES})

target_include_directories(absinthe_crashreport
  PRIVATE
    ${PList_INCLUDE_DIRS}
    ${libimobiledevice_INCLUDE_DIR}
)

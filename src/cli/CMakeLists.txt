
file(GLOB SRC_FILES "*.c" "*.h")
add_executable(absinthe_cli ${SRC_FILES})

target_include_directories(absinthe_cli
PRIVATE
    ${IMobileDevice_INCLUDE_DIRS}
)

target_link_directories(absinthe_cli
PRIVATE
    absinthe_crashreport
    ${libimobiledevice_LIBRARIES}
)

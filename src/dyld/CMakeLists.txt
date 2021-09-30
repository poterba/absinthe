
file(GLOB SRC_FILES "*.c" "*.h")
add_library(absinthe_dyld ${SRC_FILES})

if (APPLE)
    target_link_libraries(absinthe_dyld ${FOUNDATION})
endif()
# list(APPEND DEPENDENCIES_LIST ${FOUNDATION})
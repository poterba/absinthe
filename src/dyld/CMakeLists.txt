
file(GLOB SRC_FILES *.cpp *.hpp)
add_library(absinthe_dyld ${SRC_FILES})

set(DEPENDENCIES absinthe_util)
if (APPLE)
    list(APPEND DEPENDENCIES ${FOUNDATION})
endif()

target_link_libraries(absinthe_dyld PRIVATE ${DEPENDENCIES})

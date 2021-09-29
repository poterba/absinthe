
file(GLOB SRC_FILES "*.c" "*.h")
add_library(absinthe_macho ${SRC_FILES})
target_link_libraries(absinthe_macho
  PRIVATE
    absinthe_util
)
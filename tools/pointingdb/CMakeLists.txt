
file(GLOB SRC_FILES "*.c" "*.h")
add_executable(absinthe_tool_pointingdb ${SRC_FILES})

# pointingdb_SOURCES = pointingdb.c \
# 				../src/common.c \
# 				../src/file.c
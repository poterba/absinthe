
file(GLOB SRC_FILES *.cpp *.hpp)
add_executable(absinthe_tool_decache ${SRC_FILES})

# decache_SOURCES = \
# 				decache.c \
# 				../src/file.c \
# 				../src/common.c \
# 				../src/dyldcache.c \
# 				../src/dyldimage.c \
# 				../src/dyldmap.c
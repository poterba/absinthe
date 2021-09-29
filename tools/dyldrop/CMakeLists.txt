
file(GLOB SRC_FILES "*.c" "*.h")
add_executable(absinthe_tool_dyldrop ${SRC_FILES})

# dyldrop_SOURCES = dyldrop.c \
# 				../src/common.c \
# 				../src/file.c \
# 				../src/dyldcache.c \
# 				../src/dyldimage.c \
# 				../src/dyldmap.c \
# 				../src/macho.c \
# 				../src/macho_command.c \
# 				../src/macho_segment.c \
# 				../src/macho_section.c \
# 				../src/macho_symbol.c \
# 				../src/macho_symtab.c
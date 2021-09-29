
file(GLOB SRC_FILES "*.c" "*.h")
add_executable(absinthe_tool_machoman ${SRC_FILES})

# machoman_SOURCES = machoman.c \
# 				../src/common.c \
# 				../src/file.c \
# 				../src/macho.c \
# 				../src/macho_command.c \
# 				../src/macho_segment.c \
# 				../src/macho_section.c \
# 				../src/macho_symbol.c \
# 				../src/macho_symtab.c
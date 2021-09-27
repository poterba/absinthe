file(GLOB SRC_FILES "*.c" "*.h")
add_library(absinthe_macho ${SRC_FILES})

# macho_command.c
# macho_section.c
# macho_segment.c
# macho_symtab.c
# macho.c
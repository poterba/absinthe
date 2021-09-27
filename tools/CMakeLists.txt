add_executable(gizmo gizmo.c)

add_executable(dyld
    dyldrop.c
)
target_link_libraries(dyld
    absinthe_dyld
    absinthe_macho
)
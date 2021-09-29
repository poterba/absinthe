# add_executable(gizmo gizmo.c)

# target_link_libraries(gizmo
#     absinthe_dyld
#     absinthe_macho
# )

add_subdirectory(binpatch)
add_subdirectory(decache)
add_subdirectory(dyldrop)
add_subdirectory(fsgen)
add_subdirectory(gizmo)
add_subdirectory(handoff)
add_subdirectory(injectconf)
add_subdirectory(machoman)
add_subdirectory(mb2insert)
add_subdirectory(mbdbtool)
add_subdirectory(pointingdb)
add_subdirectory(switchapp)

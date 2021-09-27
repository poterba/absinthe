
add_executable(absinthe_gui
    main.cpp
    # $(AM_GUI_SRC_FILES)
    AbsintheWorker.cpp
    AbsintheJailbreaker.cpp
    iTunesKiller.cpp
)

target_link_libraries(absinthe_gui
    absinthe
)


file(GLOB SRC_FILES *.cpp *.hpp)
add_executable(absinthe_tool_handoff ${SRC_FILES})

# handoff_SOURCES = \
# 				handoff.c \
# 				../src/device.c \
# 				../src/file.c \
# 				../src/common.c 

file(GLOB SRC_FILES "*.c" "*.h")
add_executable(absinthe_tool_mb2insert ${SRC_FILES})

target_include_directories(absinthe_tool_mb2insert
PRIVATE
    ${libimobiledevice_INCLUDE_DIR}
)

# mb2insert_SOURCES = mb2insert.c \
# 				../src/idevicebackup2.c \
# 				rop.c \
# 				../src/file.c \
# 				../src/common.c \
# 				../src/backup.c \
# 				../src/backup_file.c \
# 				../src/mbdb.c \
# 				../src/mbdb_record.c 
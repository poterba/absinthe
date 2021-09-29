
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(PC_libplist QUIET libplist-2.0)
find_path(libplist_INCLUDE_DIR NAMES plist/plist.h HINTS ${PC_libplist_INCLUDE_DIRS})
FIND_LIBRARY(libplist_LIBRARY
    NAMES
        plist
        libplist
        plist-2.0
        libplist-2.0
    HINTS
        ${PC_libplist_LIBDIR}
        ${PC_libplist_LIBRARY_DIRS})

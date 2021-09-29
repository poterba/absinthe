
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

MARK_AS_ADVANCED(libplist_INCLUDE_DIR libplist_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set libimobiledevice_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libplist DEFAULT_MSG libplist_LIBRARY libplist_INCLUDE_DIR)

SET(libplist_LIBRARIES    ${libplist_LIBRARY})
SET(libplist_INCLUDE_DIRS ${libplist_INCLUDE_DIR})

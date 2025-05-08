# OGG Build
set(INSTALL_DOCS OFF)
set(INSTALL_PKG_CONFIG_MODULE OFF)
set(INSTALL_CMAKE_PACKAGE_MODULE OFF)
add_subdirectory(ogg)

# Vorbis Build
add_subdirectory(vorbis)

# ZLib build
set(ZLIB_BUILD_TESTING OFF)
add_subdirectory(zlib)

# ZZip Build
add_subdirectory(zziplib)

# wxWidgets Build
add_subdirectory(wxWidgets)

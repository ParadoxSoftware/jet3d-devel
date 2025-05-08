add_subdirectory(ogg)
add_subdirectory(vorbis)
add_subdirectory(zlib)

# options ###########################################################
set(ZZIPMMAPPED OFF CACHE BOOL "" FORCE)
set(ZZIPFSEEKO  OFF CACHE BOOL "" FORCE)

if(UNIX OR MINGW)
set(ZZIP_COMPAT OFF CACHE BOOL "" FORCE)
set(ZZIP_LIBTOOL OFF CACHE BOOL "" FORCE)
set(ZZIP_PKGCONFIG OFF CACHE BOOL "" FORCE)
else()
set(ZZIP_COMPAT OFF CACHE BOOL "" FORCE)
set(ZZIP_LIBTOOL OFF CACHE BOOL "" FORCE)
set(ZZIP_PKGCONFIG OFF CACHE BOOL "" FORCE)
endif()
set(LARGEFILEDETECT OFF CACHE BOOL "" FORCE)
add_subdirectory(zziplib)

add_subdirectory(wxWidgets)
add_subdirectory(ode)
add_subdirectory(FreeImage EXCLUDE_FROM_ALL)

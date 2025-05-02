# JetEngine
file(GLOB_RECURSE JETENGINE_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(JETDLL Jet3d.dll)

add_library(${JETDLL} SHARED
    ${JETENGINE_SRC_FILES}
    ${DEPENDENCIES_PATH}/nvwa/nvwa/bool_array.cpp
    ${DEPENDENCIES_PATH}/nvwa/nvwa/debug_new.cpp
    ${DEPENDENCIES_PATH}/nvwa/nvwa/mem_pool_base.cpp
    ${DEPENDENCIES_PATH}/nvwa/nvwa/static_mem_pool.cpp
)

target_include_directories(${JETDLL} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../../include
    ${CMAKE_CURRENT_LIST_DIR}/../../Tools/Editor
    ${DEPENDENCIES_PATH}/nvwa/nvwa
    ${DEPENDENCIES_PATH}/vorbis/include
    ${DEPENDENCIES_PATH}/zziplibb
    ${DEPENDENCIES_PATH}/FreeImage/Source
    ${DEPENDENCIES_PATH}/DXSDK_81/include
    World
    Engine
    Particle
    Engine/Drivers
    Actor
    BSP
    Math
    Support
    Physics
    VFile
    Bitmap
    Bitmap/Compression
    guWorld
    Mp3Mgr
)

target_compile_definitions(${JETDLL} PRIVATE
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    JETENGINE_EXPORTS
    JET_VERSION_2
)

target_link_directories(${JETDLL} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../lib
    ${DEPENDENCIES_PATH}/VorbisSDK/lib
    ${DEPENDENCIES_PATH}/EAXSDK/lib
    ${DEPENDENCIES_PATH}/DXSDK_81/lib
    ${DEPENDENCIES_PATH}/FreeImage/lib
    ${DEPENDENCIES_PATH}/zzip/lib
)

target_link_libraries(${JETDLL} PRIVATE
    amstrmid.lib
    oldnames.lib
    urlmon.lib
    winmm.lib
    quartz.lib
    dxguid.lib
    ddraw.lib
    dsound.lib
    vorbis_static.lib
    vorbisfile_static.lib
    ogg_static.lib
    FreeImage.lib
    zziplib-Release.lib
)

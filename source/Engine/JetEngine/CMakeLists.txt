# JetEngine
file(GLOB_RECURSE JETENGINE_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(JETDLL Jet3d)

add_library(${JETDLL} SHARED
    ${JETENGINE_SRC_FILES}
    ${DEPENDENCIES_PATH}/nvwa/nvwa/bool_array.cpp
    ${DEPENDENCIES_PATH}/nvwa/nvwa/debug_new.cpp
    ${DEPENDENCIES_PATH}/nvwa/nvwa/mem_pool_base.cpp
    ${DEPENDENCIES_PATH}/nvwa/nvwa/static_mem_pool.cpp
)

target_include_directories(${JETDLL} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/source/Tools/Editor
    ${DEPENDENCIES_PATH}/nvwa/nvwa
    ${DEPENDENCIES_PATH}/vorbis/include
    ${DEPENDENCIES_PATH}/ogg/include
    ${DEPENDENCIES_PATH}/zziplib
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

target_compile_definitions(${JETDLL} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    JETENGINE_EXPORTS
    JET_VERSION_2
)

target_link_directories(${JETDLL}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
    #PRIVATE ${DEPENDENCIES_PATH}/EAXSDK/lib
    PUBLIC ${DEPENDENCIES_PATH}/DXSDK_81/lib
)

target_link_libraries(${JETDLL} PUBLIC
    amstrmid
    oldnames
    urlmon
    winmm
    quartz
    dxguid
    ddraw
    dsound
    vorbis
    vorbisfile
    ogg
    #FreeImage
    libzzip
)

install (TARGETS ${JETDLL} 
    RUNTIME DESTINATION "${JET_ROOT_DIR}/bin"
    LIBRARY DESTINATION "${JET_ROOT_DIR}/bin"
    ARCHIVE DESTINATION "${JET_ROOT_DIR}/lib"
)

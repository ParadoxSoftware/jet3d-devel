# TerrainObj
message(STATUS "TerrainObj Build")

file(GLOB_RECURSE TERRAINOBJ_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${TERRAINOBJ_SRC_FILES})

set(TERRAINOBJ TerrainObj)

add_library(${TERRAINOBJ} SHARED
    ${TERRAINOBJ_SRC_FILES}
)

target_include_directories(${TERRAINOBJ} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${TERRAINOBJ} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    TERRAIN_EXPORTS
)

target_link_directories(${TERRAINOBJ}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${TERRAINOBJ} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${TERRAINOBJ} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

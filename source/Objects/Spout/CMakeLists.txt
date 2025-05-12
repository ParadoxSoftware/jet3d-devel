# SpoutObj
message(STATUS "SpoutObj Build")

file(GLOB_RECURSE SPOUTOBJ_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${SPOUTOBJ_SRC_FILES})

set(SPOUTOBJ SpoutObj)

add_library(${SPOUTOBJ} SHARED
    ${SPOUTOBJ_SRC_FILES}
)

target_include_directories(${SPOUTOBJ} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${SPOUTOBJ} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    SPOUT_EXPORTS
)

target_link_directories(${SPOUTOBJ}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${SPOUTOBJ} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${SPOUTOBJ} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

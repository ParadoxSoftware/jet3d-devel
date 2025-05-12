# PortalObj
message(STATUS "PortalObj Build")

file(GLOB_RECURSE PORTALOBJ_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${PORTALOBJ_SRC_FILES})

set(PORTALOBJ PortalObj)

add_library(${PORTALOBJ} SHARED
    ${PORTALOBJ_SRC_FILES}
)

target_include_directories(${PORTALOBJ} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${PORTALOBJ} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    PORTALS_EXPORTS
)

target_link_directories(${PORTALOBJ}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${PORTALOBJ} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${PORTALOBJ} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

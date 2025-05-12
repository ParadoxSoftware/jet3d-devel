# ModelObj
message(STATUS "ModelObj Build")

file(GLOB_RECURSE MODELOBJ_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${MODELOBJ_SRC_FILES})

set(MODELOBJ ModelObj)

add_library(${MODELOBJ} SHARED
    ${MODELOBJ_SRC_FILES}
)

target_include_directories(${MODELOBJ} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${MODELOBJ} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    MODELOBJECT_EXPORTS
)

target_link_directories(${MODELOBJ}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${MODELOBJ} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${MODELOBJ} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

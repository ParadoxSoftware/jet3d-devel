# ActorObj
message(STATUS "ActorObj Build")

file(GLOB_RECURSE ACTOROBJ_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${ACTOROBJ_SRC_FILES})

set(ACTOROBJ ActorObj)

add_library(${ACTOROBJ} SHARED
    ${ACTOROBJ_SRC_FILES}
)

target_include_directories(${ACTOROBJ} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${ACTOROBJ} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    ACTOROBJ_EXPORTS
)

target_link_directories(${ACTOROBJ}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${ACTOROBJ} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${ACTOROBJ} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

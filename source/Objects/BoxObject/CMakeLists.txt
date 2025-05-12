# ActorObj
message(STATUS "BoxObject Build")

file(GLOB_RECURSE BOXOBJ_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${BOXOBJ_SRC_FILES})

set(BOXOBJ BoxObj)

add_library(${BOXOBJ} SHARED
    ${BOXOBJ_SRC_FILES}
)

target_include_directories(${BOXOBJ} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${BOXOBJ} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    BOXOBJECT_EXPORTS
)

target_link_directories(${BOXOBJ}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${BOXOBJ} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${BOXOBJ} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

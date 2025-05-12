# ActorObj
message(STATUS "AmbObject Build")

file(GLOB_RECURSE AMBOBJ_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${AMBOBJ_SRC_FILES})

set(AMBOBJ AmbientObj)

add_library(${AMBOBJ} SHARED
    ${AMBOBJ_SRC_FILES}
)

target_include_directories(${AMBOBJ} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${AMBOBJ} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    AMBIENTOBJ_EXPORTS
)

target_link_directories(${AMBOBJ}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${AMBOBJ} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${AMBOBJ} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

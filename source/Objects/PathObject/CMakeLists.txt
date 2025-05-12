# PathObject
message(STATUS "PathObject Build")

file(GLOB_RECURSE PATHOBJECT_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${PATHOBJECT_SRC_FILES})

set(PATHOBJECT PathObj)

add_library(${PATHOBJECT} SHARED
    ${PATHOBJECT_SRC_FILES}
    ${CMAKE_CURRENT_LIST_DIR}/PathObj.rc
)

target_include_directories(${PATHOBJECT} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${PATHOBJECT} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    PATHOBJECT_EXPORTS
)

target_link_directories(${PATHOBJECT}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${PATHOBJECT} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${PATHOBJECT} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

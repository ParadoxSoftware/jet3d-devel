# CoronaObj
message(STATUS "CoronaObj Build")

file(GLOB_RECURSE CORONA_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${CORONA_SRC_FILES})

set(CORONA CoronaObj)

add_library(${CORONA} SHARED
    ${CORONA_SRC_FILES}
)

target_include_directories(${CORONA} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${CORONA} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    CORONA_EXPORTS
)

target_link_directories(${CORONA}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${CORONA} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${CORONA} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

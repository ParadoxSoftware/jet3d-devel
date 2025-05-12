# PulsingLightObj
message(STATUS "PulsingLightObj Build")

file(GLOB_RECURSE PULSINGLIGHTOBJ_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${PULSINGLIGHTOBJ_SRC_FILES})

set(PULSINGLIGHTOBJ PulsingLightObj)

add_library(${PULSINGLIGHTOBJ} SHARED
    ${PULSINGLIGHTOBJ_SRC_FILES}
)

target_include_directories(${PULSINGLIGHTOBJ} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${PULSINGLIGHTOBJ} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    PULSINGLIGHT_EXPORTS
)

target_link_directories(${PULSINGLIGHTOBJ}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${PULSINGLIGHTOBJ} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${PULSINGLIGHTOBJ} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

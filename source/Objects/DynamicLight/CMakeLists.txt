# DynLightObj
message(STATUS "DynLightObj Build")

file(GLOB_RECURSE DYNLIGHT_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${DYNLIGHT_SRC_FILES})

set(DYNLIGHT DynLightObj)

add_library(${DYNLIGHT} SHARED
    ${DYNLIGHT_SRC_FILES}
)

target_include_directories(${DYNLIGHT} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${DYNLIGHT} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    DYNAMICLIGHT_EXPORTS
)

target_link_directories(${DYNLIGHT}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${DYNLIGHT} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${DYNLIGHT} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

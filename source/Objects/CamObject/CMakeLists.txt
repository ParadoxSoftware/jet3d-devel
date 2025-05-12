# CamObj
message(STATUS "CamObject Build")

file(GLOB_RECURSE CAMOBJ_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
message(${CAMOBJ_SRC_FILES})

set(CAMOBJ CameraObj)

add_library(${CAMOBJ} SHARED
    ${CAMOBJ_SRC_FILES}
)

target_include_directories(${CAMOBJ} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${CAMOBJ} PUBLIC
    _CRT_SECURE_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    CAMOBJECT_EXPORTS
)

target_link_directories(${CAMOBJ}
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${CAMOBJ} PUBLIC
    winmm
    Jet3d
)

install (TARGETS ${CAMOBJ} 
    RUNTIME DESTINATION "bin/objects"
    LIBRARY DESTINATION "bin/objects"
)

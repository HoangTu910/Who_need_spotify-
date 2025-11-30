file(GLOB FILTER_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/filters/*.cpp
)

file(GLOB MODULE_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/modules/*.cpp
)

file(GLOB UTIL_SOURCES 
    ${CMAKE_SOURCE_DIR}/utils/*.cpp
)

file(GLOB COMMON_SOURCES 
    ${CMAKE_SOURCE_DIR}/common/*.cpp
)

set(ALL_SOURCES
    ${FILTER_SOURCES}
    ${MODULE_SOURCES}
    ${UTIL_SOURCES}
    ${COMMON_SOURCES}
)
add_library(wns_audio_dsp STATIC
    ${ALL_SOURCES}
)

if(EXISTS ${CMAKE_SOURCE_DIR}/src/main.cpp)
    add_executable(wns_main
        ${CMAKE_SOURCE_DIR}/src/main.cpp
    )
    target_link_libraries(wns_main
        wns_audio_dsp
    )
endif()

message(STATUS "Filter sources: ${FILTER_SOURCES}")
message(STATUS "Module sources: ${MODULE_SOURCES}")
message(STATUS "Util sources: ${UTIL_SOURCES}")
message(STATUS "Common sources: ${COMMON_SOURCES}")
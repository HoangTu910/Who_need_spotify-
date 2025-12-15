file(GLOB FILTER_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/filters/*.cpp
)

file(GLOB MODULE_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/modules/*.cpp
)

file(GLOB KERNEL_SOURCES
    ${CMAKE_SOURCE_DIR}/src/kernels/*.cpp
)

file(GLOB BUFFER_SOURCES
    ${CMAKE_SOURCE_DIR}/src/buffer/*.cpp
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
    ${KERNEL_SOURCES}
    ${BUFFER_SOURCES}
)
add_library(wns_audio_dsp STATIC
    ${ALL_SOURCES}
)

# Try to find libsndfile via pkg-config; if found, link and define HAVE_SNDFILE
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(SNDFILE QUIET sndfile)
    if(SNDFILE_FOUND)
        message(STATUS "Found sndfile via pkg-config: ${SNDFILE_VERSION}")
        target_include_directories(wns_audio_dsp PRIVATE ${SNDFILE_INCLUDE_DIRS})
        target_link_libraries(wns_audio_dsp PRIVATE ${SNDFILE_LIBRARIES})
        target_compile_definitions(wns_audio_dsp PRIVATE HAVE_SNDFILE=1)
    else()
        message(WARNING "libsndfile not found via pkg-config; preprocessing will use fallback WAV reader/writer.")
    endif()
else()
    message(WARNING "pkg-config not found; skipping libsndfile detection.")
endif()

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
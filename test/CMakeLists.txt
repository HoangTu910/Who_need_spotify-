# CMake for test/ folder
cmake_minimum_required(VERSION 3.10)
project(wns_tests)

# Expect the library wns_audio_dsp to be built by ../src/CMakeLists.txt
if(TARGET wns_audio_dsp)
    add_subdirectory(utest)
else()
    message(WARNING "wns_audio_dsp target not found. Make sure src/ is configured before tests.")
endif()

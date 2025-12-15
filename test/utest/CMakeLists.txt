# Build unit tests in this folder
add_executable(wns_fft_utest wns_fft_utest.cpp)
target_link_libraries(wns_fft_utest PRIVATE wns_audio_dsp)

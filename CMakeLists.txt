cmake_minimum_required(VERSION 3.10)

project(Who_Need_Spotify)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/utils
)

add_subdirectory(src)    # Process src/CMakeLists.txt
# add_subdirectory(test)   # Process test/CMakeLists.txt
# Enable building tests in the `test/` folder (uncomment to enable)
add_subdirectory(test)
# --- IGNORE ---

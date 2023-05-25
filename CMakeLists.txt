cmake_minimum_required (VERSION 3.8)

project ("DoveCompiler")

# TODO: Add tests and install targets if needed.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
enable_testing()

add_subdirectory(src)
# add_subdirectory(tests)

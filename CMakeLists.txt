cmake_minimum_required (VERSION 3.8)

project ("DoveCompiler")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_compile_options(
	-g -Wall -Werror -Wextra -pthread
	-Wno-unused-parameter
	-Wno-unused-variable
	-Wno-unknown-pragmas
	-std=gnu++17
)

add_link_options(-lpthread -lm -v -pthread)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)



enable_testing()

include(${PROJECT_SOURCE_DIR}/includes/runtime/conanbuildinfo.cmake)
conan_basic_setup()
# link_libraries(${CONAN_LIBS})

file(GLOB generated 
	includes/generated/*.cpp
)


add_subdirectory(src)

file(GLOB Parser 
	includes/Parser/*.cc
	includes/Parser/*.h
)

file(GLOB IR 
	includes/IR/*.cc
	includes/IR/*.h)

file(GLOB Utils
	includes/Utils/*.cc
	includes/Util/s*.h
)

add_executable(DoveCompiler
	${generated}
	"src/main.cc"
)
target_link_libraries(DoveCompiler PUBLIC ${CONAN_LIBS})
target_include_directories(DoveCompiler PUBLIC
	includes/deps
	includes/generated
	src
)





# add_subdirectory(tests)

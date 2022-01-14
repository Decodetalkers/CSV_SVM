project(libcsv)
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_subdirectory(plugins/libsvm)
add_subdirectory(test)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)
add_library(${PROJECT_NAME}
	STATIC
	csv.h
	csv.cpp	
)
target_link_libraries(
	${PROJECT_NAME}
	libsvm
)
target_include_directories(${PROJECT_NAME} PUBLIC .) #${INCLUDE_DIRECTORIES})

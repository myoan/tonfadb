cmake_minimum_required(VERSION 2.6)

project(tonfa)
set(CMAKE_C_FLAGS   "-Wall -O0 -g3")

set(SRC_DIR
	src/isam.cpp
)

set(INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(${INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

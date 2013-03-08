cmake_minimum_required(VERSION 2.6)

project(tonfa)
set(CMAKE_CXX_FLAGS   "-Wall")

SET(CMAKE_C_FLAGS_RELEASE      "${CMAKE_C_FLAGS} -O3 -g -D_DEBUG=1")
SET(CMAKE_C_FLAGS_DEBUG        "${CMAKE_CXX_FLAGS} -O0 -g3")
SET(CMAKE_C_FLAGS_COMPRESSED   "${CMAKE_C_FLAGS} -Os -g0")

set(SRC_DIR
	src/bptree.c
	src/util.c
)

set(INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(${INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

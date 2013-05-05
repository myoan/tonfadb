cmake_minimum_required(VERSION 2.6)

project(tonfa)
set(CMAKE_CXX_FLAGS   "-Wall")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_definitions(-DTNF_DEBUG)
endif()

SET(CFLAGS "-Wsign-compare -Wformat")
SET(CFLAGS "${CFLAGS} -Wall -Wcomment -Wunused-macros -Werror=return-type")
SET(CMAKE_C_FLAGS   "${CFLAGS} -Wno-long-long -Werror-implicit-function-declaration")
SET(CMAKE_CXX_FLAGS "${CFLAGS}")
SET(CMAKE_C_FLAGS_RELEASE      "${CMAKE_CXX_FLAGS} -O3 -g")
SET(CMAKE_C_FLAGS_DEBUG        "${CMAKE_CXX_FLAGS} -O0 -g3")
SET(CMAKE_C_FLAGS_COMPRESSED   "${CMAKE_CXX_FLAGS} -Os -g0")

set(SRC_DIR
	#src/bptree.c
	src/btree.c
	src/file.c
	src/parse.c
	src/readline.c
	src/tonfa.c
	src/util.c
)

set(INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(${INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

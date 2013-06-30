cmake_minimum_required(VERSION 2.6)

project(tonfa)
set(CMAKE_CXX_FLAGS   "-Wall")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

SET(CFLAGS "-Wsign-compare -Wformat")
SET(CFLAGS "${CFLAGS} -Wall -Wcomment -Wunused-macros -Werror=return-type")
SET(CMAKE_C_FLAGS   "${CFLAGS} -Wno-long-long -Werror-implicit-function-declaration")
SET(CMAKE_CXX_FLAGS "${CFLAGS}")
SET(CMAKE_C_FLAGS_RELEASE      "${CMAKE_CXX_FLAGS} -O3 -g")
SET(CMAKE_C_FLAGS_DEBUG        "${CMAKE_CXX_FLAGS} -O0 -g3")
SET(CMAKE_C_FLAGS_COMPRESSED   "${CMAKE_CXX_FLAGS} -Os -g0")

set(SRC_DIR
	src/btree.cpp
	test/isam/AlternateArrayTest.cpp
)

set(INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(${INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

set(ExtraLibs)
find_library(HAVE_CPPUTEST NAMES CppUTest)

if(HAVE_CPPUTEST)
	set(ExtraLibs ${ExtraLibs} ${HAVE_CPPUTEST})
	target_link_libraries(${PROJECT_NAME} ${ExtraLibs})
endif(HAVE_CPPUTEST)

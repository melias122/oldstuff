cmake_minimum_required(VERSION 3.1.0)

project(asos CXX)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions("-std=c++11 -fPIC -Wall -fnon-call-exceptions -DQ_COMPILER_INITIALIZER_LISTS")

find_package(Qt5 COMPONENTS Core REQUIRED)
if(NOT Qt5_FOUND)
	message(FATAL_ERROR "Package Qt5::Core not found")
endif(NOT Qt5_FOUND)

include_directories(
	${Qt5Core_INCLUDE_DIRS}
    src
)

add_subdirectory(src)

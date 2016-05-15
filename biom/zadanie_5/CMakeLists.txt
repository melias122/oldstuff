cmake_minimum_required(VERSION 3.3)
project(zadanie_5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

INCLUDE_DIRECTORIES(include)

set(INCLUDE
include/hamming.h
include/pca.h
include/euclid.h
include/hybrid.h
)

set(SRC
src/main.cpp
src/hamming.cpp
src/pca.cpp
src/euclid.cpp
src/hybrid.cpp
)

set(SOURCE_FILES ${SRC} ${INCLUDE})
add_executable(zadanie_5 ${SOURCE_FILES})

# opencv
find_package(OpenCV 3 REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

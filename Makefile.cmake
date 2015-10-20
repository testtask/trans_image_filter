
cmake_minimum_required(VERSION 2.8)

project( trans_img_filter )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/../install")

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

link_directories(${OpenCV_LIB_DIR})

add_definitions(-std=c++11)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/trans_image_filter)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_library(img_filter ${SRC}/img_filter.cpp )
add_executable(sample ${SRC}/sample.cpp )

target_link_libraries(sample  boost_program_options ${OpenCV_LIBS} img_filter )

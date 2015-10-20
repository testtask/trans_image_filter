cmake_minimum_required (VERSION 2.8)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

project (trans_img_filter)

add_definitions(-std=c++11)
add_definitions(-DNDEBUG)
add_subdirectory(tests)

install(DIRECTORY vpp DESTINATION include
  FILES_MATCHING PATTERN "*.hh")
install(DIRECTORY vpp DESTINATION include
  FILES_MATCHING PATTERN "*.hpp")
  

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_LIST_DIR}/cmake
                      ${CMAKE_SOURCE_DIR}/cmake)


include(${QT_USE_FILE})

SET(THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/install)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..
                    ${THIRDPARTY}/include
                    $ENV{HOME}/local/include
                    ${GLEW_INCLUDE_DIR}
         	    ${DIGE_INCLUDE_DIR})

set(LIBS
         dige
         ${QT_LIBRARIES})

link_directories(${THIRDPARTY}/lib)

add_definitions(-std=c++1y -g -fopenmp)
add_definitions(-Ofast -march=native)
add_definitions(-DNDEBUG)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)


add_executable(sample sample.cpp)
target_link_libraries(box_filter ${OpenCV_LIBS} gomp)

add_executable(fast_detector fast_detector.cc)
target_link_libraries(fast_detector ${OpenCV_LIBS} gomp)

add_executable(tutorial tutorial.cc)
target_link_libraries(tutorial ${OpenCV_LIBS} ${LIBS} gomp)

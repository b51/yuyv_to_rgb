cmake_minimum_required(VERSION 3.2)
project(yuyv_to_rgb)

add_definitions(-std=c++14)

#########################################################################
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "No build type selected, default to ${CMAKE_BUILD_TYPE}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fPIC")
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fPIC")
endif(CMAKE_BUILD_TYPE STREQUAL "Release")
#########################################################################

find_package(OpenCV REQUIRED)

include_directories(
  ${OpenCV_INCLUDE_DIRS}
)

###########
## Build ##
###########

add_executable(${PROJECT_NAME}
  src/main.cc
)

target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBRARIES}
  -lgflags
  -lglog
  -Wl,--no-whole-archive
)


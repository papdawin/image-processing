cmake_minimum_required(VERSION 3.9)
project(ProjectName)

set(CMAKE_CXX_STANDARD 11)

#find_library(OpenCV)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 11)

add_executable(ProjectName main.cpp transformations.h misc.h convolutions.h wallisfilter.h nonlinear.h morphology.h bogarkoveto.h laws.h)

target_link_libraries(ProjectName ${OpenCV_LIBS})
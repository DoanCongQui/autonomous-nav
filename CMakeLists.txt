cmake_minimum_required(VERSION 3.10)

project(project_car)

find_package(OpenCV REQUIRED)
include_directories(
    /usr/local/include/NumCpp
    ${OpenCV_INCLUDE_DIRS})

add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS})

cmake_minimum_required(VERSION 3.10)
project(Random)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

find_package(CURL REQUIRED)

add_executable(Random main.cpp)

target_link_libraries(Random ${wxWidgets_LIBRARIES} ${CURL_LIBRARIES})
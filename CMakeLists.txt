cmake_minimum_required(VERSION 3.10)
project(WhetherApp)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

find_package(CURL REQUIRED)
find_package(nlohmann_json)

add_executable(WhetherApp src/main.cpp)
target_link_libraries(WhetherApp ${wxWidgets_LIBRARIES} ${CURL_LIBRARIES})
cmake_minimum_required(VERSION 3.31)
project(tchat)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ASIO REQUIRED asio)
include_directories(${ASIO_INCLUDE_DIRS})

include_directories(include)

add_executable(client src/client.cpp)
add_executable(server src/server.cpp)
target_link_libraries(client PRIVATE ${ASIO_LIBRARIES})
target_link_libraries(client PRIVATE ${ASIO_LIBRARIES})

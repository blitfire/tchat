cmake_minimum_required(VERSION 3.31)
project(tchat)

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pkg_check_modules(ASIO REQUIRED)
include_directories(${ASIO_INCLUDE_DIRS})

include_directories(include)
file(GLOB SHARED_SOURCES "src/shared/*.cpp")
file(GLOB CLIENT_SOURCES "src/client/*.cpp")
file(GLOB SERVER_SOURCES "src/server/*.cpp")

add_executable(client ${SHARED_SOURCES} ${CLIENT_SOURCES})
add_executable(server ${SHARED_SOURCES} ${SERVER_SOURCES})
target_link_libraries(client PRIVATE ${ASIO_LIBRARIES})
target_link_libraries(server PRIVATE ${ASIO_LIBRARIES})

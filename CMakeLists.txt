cmake_minimum_required(VERSION 3.21)
project(marketwatch LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE) # for clangd

file(GLOB_RECURSE SOURCES src/*.hpp src/*.cpp)
add_executable(main ${SOURCES})

find_package(websocketpp CONFIG REQUIRED)
target_link_libraries(main PRIVATE websocketpp::websocketpp)

find_package(OpenSSL REQUIRED)
target_link_libraries(main PRIVATE OpenSSL::SSL)
target_link_libraries(main PRIVATE OpenSSL::Crypto)

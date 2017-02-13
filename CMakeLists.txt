cmake_minimum_required(VERSION 2.6)
project(libkafka_asio )

enable_testing()

add_subdirectory("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory("${PROJECT_SOURCE_DIR}/examples")
add_subdirectory("${PROJECT_SOURCE_DIR}/test")

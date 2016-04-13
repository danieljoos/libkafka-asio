cmake_minimum_required(VERSION 2.6)
project(libkafka_asio_examples)

add_subdirectory("${PROJECT_SOURCE_DIR}/cpp03")

option(NO_CXX11, "Disable build of C++11 examples")
if (NOT NO_CXX11)
    add_subdirectory("${PROJECT_SOURCE_DIR}/cpp11")
endif ()

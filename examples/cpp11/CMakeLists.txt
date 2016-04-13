cmake_minimum_required(VERSION 3.1)
project(libkafka_asio_examples_cxx11)

add_definitions("-DLIBKAFKAASIO_NO_COMPRESSION")

find_package(Boost REQUIRED COMPONENTS thread system)

include_directories(
        ${PROJECT_SOURCE_DIR}/../../lib
        ${Boost_INCLUDE_DIRS})

set(examples_LINK_LIBRARIES ${Boost_LIBRARIES})

if (UNIX)
    list(APPEND examples_LINK_LIBRARIES pthread)
endif ()

add_executable(fetch_cxx11 ${PROJECT_SOURCE_DIR}/fetch.cpp)
target_link_libraries(fetch_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET fetch_cxx11 PROPERTY CXX_STANDARD 11)

add_executable(produce_cxx11 ${PROJECT_SOURCE_DIR}/produce.cpp)
target_link_libraries(produce_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET produce_cxx11 PROPERTY CXX_STANDARD 11)

add_executable(metadata_cxx11 ${PROJECT_SOURCE_DIR}/metadata.cpp)
target_link_libraries(metadata_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET metadata_cxx11 PROPERTY CXX_STANDARD 11)

add_executable(offset_cxx11 ${PROJECT_SOURCE_DIR}/offset.cpp)
target_link_libraries(offset_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET offset_cxx11 PROPERTY CXX_STANDARD 11)

add_executable(offset_fetch_cxx11 ${PROJECT_SOURCE_DIR}/offset_fetch.cpp)
target_link_libraries(offset_fetch_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET offset_fetch_cxx11 PROPERTY CXX_STANDARD 11)

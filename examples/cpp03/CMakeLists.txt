cmake_minimum_required(VERSION 2.6)
project(libkafka_asio_examples_cxx03)

add_definitions("-DLIBKAFKAASIO_NO_COMPRESSION")

find_package(Boost REQUIRED COMPONENTS thread system)

include_directories(
        ${PROJECT_SOURCE_DIR}/../../lib
        ${Boost_INCLUDE_DIRS})

set(examples_LINK_LIBRARIES ${Boost_LIBRARIES})

if (UNIX)
    list(APPEND examples_LINK_LIBRARIES pthread)
endif ()

add_executable(produce_cxx03 ${PROJECT_SOURCE_DIR}/produce.cpp)
target_link_libraries(produce_cxx03 ${examples_LINK_LIBRARIES})

add_executable(fetch_cxx03 ${PROJECT_SOURCE_DIR}/fetch.cpp)
target_link_libraries(fetch_cxx03 ${examples_LINK_LIBRARIES})

add_executable(metadata_cxx03 ${PROJECT_SOURCE_DIR}/metadata.cpp)
target_link_libraries(metadata_cxx03 ${examples_LINK_LIBRARIES})

add_executable(offset_cxx03 ${PROJECT_SOURCE_DIR}/offset.cpp)
target_link_libraries(offset_cxx03 ${examples_LINK_LIBRARIES})

add_executable(offset_fetch_cxx03 ${PROJECT_SOURCE_DIR}/offset_fetch.cpp)
target_link_libraries(offset_fetch_cxx03 ${examples_LINK_LIBRARIES})

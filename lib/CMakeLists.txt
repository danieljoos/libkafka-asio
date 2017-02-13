cmake_minimum_required(VERSION 2.6)

project(libkafka_asio_lib CXX)

file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${PROJECT_SOURCE_DIR}/*.h)
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

include("${PROJECT_SOURCE_DIR}/../cmake/SourceGroupByFolder.cmake")
source_group_by_folder(${PROJECT_NAME})

cmake_minimum_required(VERSION 2.6)
project(libkafka_asio_test)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread system)

include_directories(
    ${PROJECT_SOURCE_DIR}/../lib
    ${GTEST_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})
    
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} ${Boost_LIBRARIES})
if(UNIX)
target_link_libraries(${PROJECT_NAME} pthread)
endif()

if(COMMAND GTEST_ADD_TESTS)
    enable_testing()
    GTEST_ADD_TESTS(${PROJECT_NAME} "" ${${PROJECT_NAME}_SOURCES})
endif()
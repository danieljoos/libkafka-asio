cmake_minimum_required(VERSION 2.6)
project(libkafka_asio_examples)
option(NO_CXX11, "Disable build of C++11 examples")

find_package(Boost REQUIRED COMPONENTS thread system)

include_directories(
    ${PROJECT_SOURCE_DIR}/../lib
    ${Boost_INCLUDE_DIRS})
set(examples_LINK_LIBRARIES ${Boost_LIBRARIES})
if(UNIX)
list(APPEND examples_LINK_LIBRARIES pthread)
endif()

add_executable(produce_cxx03 ${PROJECT_SOURCE_DIR}/produce_cxx03.cpp)
target_link_libraries(produce_cxx03 ${examples_LINK_LIBRARIES})

add_executable(fetch_cxx03 ${PROJECT_SOURCE_DIR}/fetch_cxx03.cpp)
target_link_libraries(fetch_cxx03 ${examples_LINK_LIBRARIES})

add_executable(metadata_cxx03 ${PROJECT_SOURCE_DIR}/metadata_cxx03.cpp)
target_link_libraries(metadata_cxx03 ${examples_LINK_LIBRARIES})

add_executable(offset_cxx03 ${PROJECT_SOURCE_DIR}/offset_cxx03.cpp)
target_link_libraries(offset_cxx03 ${examples_LINK_LIBRARIES})

if(NOT NO_CXX11)

add_executable(fetch_cxx11 ${PROJECT_SOURCE_DIR}/fetch_cxx11.cpp)
target_link_libraries(fetch_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET fetch_cxx11 PROPERTY CXX_STANDARD 11)

add_executable(produce_cxx11 ${PROJECT_SOURCE_DIR}/produce_cxx11.cpp)
target_link_libraries(produce_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET produce_cxx11 PROPERTY CXX_STANDARD 11)

add_executable(metadata_cxx11 ${PROJECT_SOURCE_DIR}/metadata_cxx11.cpp)
target_link_libraries(metadata_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET metadata_cxx11 PROPERTY CXX_STANDARD 11)

add_executable(offset_cxx11 ${PROJECT_SOURCE_DIR}/offset_cxx11.cpp)
target_link_libraries(offset_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET offset_cxx11 PROPERTY CXX_STANDARD 11)

add_executable(offset_fetch_cxx11 ${PROJECT_SOURCE_DIR}/offset_fetch_cxx11.cpp)
target_link_libraries(offset_fetch_cxx11 ${examples_LINK_LIBRARIES})
set_property(TARGET offset_fetch_cxx11 PROPERTY CXX_STANDARD 11)

endif()

cmake_minimum_required(VERSION 3.12)
project(c_map)
set(CMAKE_CXX_STANDARD 17)


project(GTestProject)

include(ExternalProject)

ExternalProject_Add(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v2.4.2
    GIT_SHALLOW true
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

include_directories(
	${CMAKE_BINARY_DIR}/catch2-prefix/src/catch2/single_include/catch2
	src
)

add_executable(
	test_c_map
	src/c_map.c
	src/c_map.h
	test/test_c_map.cpp
	test/test_main.cpp
)

add_custom_target(test
    COMMAND test_c_map
    DEPENDS test_c_map
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_dependencies(test_c_map catch2)

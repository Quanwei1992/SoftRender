cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
include(cmake/ucm.cmake)

project(SoftRender)

ucm_add_dirs(core platform renderer TO sources)
add_executable(renderer ${sources})
target_include_directories(renderer PRIVATE ${CMAKE_HOME_DIRECTORY}/third_part ${CMAKE_HOME_DIRECTORY} ${CMAKE_HOME_DIRECTORY}/core)

install(TARGETS renderer  DESTINATION ${CMAKE_HOME_DIRECTORY}/output)
install(DIRECTORY data DESTINATION ${CMAKE_HOME_DIRECTORY}/output)
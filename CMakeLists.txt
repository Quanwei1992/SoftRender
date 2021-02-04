cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
include(cmake/ucm.cmake)

project(SoftRender)

ucm_add_dirs(math platform renderer TO sources)
add_library(Core STATIC ${sources})
target_include_directories(Core PUBLIC ${CMAKE_HOME_DIRECTORY}
                                        ${CMAKE_HOME_DIRECTORY}/third_part 
                                        ${CMAKE_HOME_DIRECTORY}/math
                                        ${CMAKE_HOME_DIRECTORY}/platform
                                        ${CMAKE_HOME_DIRECTORY}/renderer
)



add_executable(EXAMPLE0 examples/example0.cpp)
target_link_libraries(EXAMPLE0 PRIVATE Core)

add_executable(EXAMPLE1 examples/example1.cpp)
target_link_libraries(EXAMPLE1 PRIVATE Core)



install(TARGETS EXAMPLE0 EXAMPLE1 DESTINATION ${CMAKE_HOME_DIRECTORY}/output)

install(DIRECTORY data DESTINATION ${CMAKE_HOME_DIRECTORY}/output)
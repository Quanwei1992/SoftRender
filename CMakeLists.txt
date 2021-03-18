cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
include(cmake/ucm.cmake)

project(SoftRender)


add_subdirectory(third_part/glm)
add_subdirectory(third_part/stb)
add_subdirectory(third_part/glfw)

SET_TARGET_PROPERTIES (glfw PROPERTIES FOLDER ThirdPart)
SET_TARGET_PROPERTIES (stb PROPERTIES FOLDER ThirdPart)


add_subdirectory(renderer)
add_subdirectory(examples/01_Creating_a_window)
add_subdirectory(examples/02_Hello_Triangle)
add_subdirectory(examples/03_Shaders)

install(DIRECTORY data DESTINATION ${CMAKE_HOME_DIRECTORY}/output)
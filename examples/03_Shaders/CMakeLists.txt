
set(this 03_Shaders)

add_executable(${this} main.cpp)
target_link_libraries(${this} PRIVATE glfw renderer)

install(TARGETS ${this} DESTINATION ${CMAKE_HOME_DIRECTORY}/output)

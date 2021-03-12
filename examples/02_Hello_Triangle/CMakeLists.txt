
set(this 02_Hello_Triangle)

add_executable(${this} main.cpp)
target_link_libraries(${this} PRIVATE glfw renderer)

install(TARGETS ${this} DESTINATION ${CMAKE_HOME_DIRECTORY}/output)

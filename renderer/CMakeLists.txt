set(this renderer)


ucm_add_dirs(include src TO sources FILTER_POP  RECURSIVE)

add_library(${this} STATIC ${sources})
target_include_directories(${this} PUBLIC include)
target_link_libraries(${this} PUBLIC glm)
target_compile_definitions(${this} PUBLIC GLM_FORCE_SWIZZLE)
project(automata)

add_library(${PROJECT_NAME} STATIC)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    src
    include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    glfw
    glm::glm
    glad
)

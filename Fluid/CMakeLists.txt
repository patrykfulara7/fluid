project(fluid)

add_executable(${PROJECT_NAME})

add_subdirectory(src)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    src
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    automata
)

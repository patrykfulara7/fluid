project(stb_image)

add_library(${PROJECT_NAME} STATIC
    stb_image.cxx
    stb_image.hxx
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ../
)

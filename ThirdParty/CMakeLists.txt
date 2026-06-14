set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glm)

add_subdirectory(glad)
add_subdirectory(stb_image)
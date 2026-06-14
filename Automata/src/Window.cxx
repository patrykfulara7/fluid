#include "Window.hxx"

namespace Automata {
Window::Window(int width, int height, const std::string &title)
    : width(width), height(height) {
  {
    int result = glfwInit();
    ASSERT(result == GLFW_TRUE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  ASSERT(window != nullptr);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  {
    GLenum result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ASSERT(result == GL_TRUE);
  }

#ifdef DEBUG
  std::cout << glGetString(GL_VERSION) << std::endl;

  glEnable(GL_DEBUG_CALLBACK_FUNCTION);
  glDebugMessageCallback(
      [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
         const GLchar *message, const void *userParam) {
        (void)source, (void)type, (void)id, (void)severity, (void)length,
            (void)userParam;

        std::cerr << message << std::endl;
        abort();
      },
      nullptr);
#endif
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::Update() {
  glfwPollEvents();
  glfwSwapBuffers(window);
}
} // namespace Automata
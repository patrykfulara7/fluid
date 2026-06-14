#include "pch.hxx"

namespace Automata {
class Window {
private:
  GLFWwindow *window = nullptr;
  int width = 0;
  int height = 0;

public:
  Window(int width, int height, const std::string &title);
  ~Window();

  void Update();

  GLFWwindow *GetWindowwindow() const { return window; }
  int GetWidth() const { return width; }
  int GetHeight() const { return height; }
};
} // namespace Automata

#pragma once

#include "pch.hxx"

namespace Automata {
class BufferLayout {
public:
  struct Attribute {
    GLint size = 0;
    GLenum type = 0;

    Attribute(GLint size, GLenum type) : size(size), type(type) {}
  };

private:
  std::vector<Attribute> bufferLayout;
  GLsizei stride = 0;

public:
  BufferLayout(std::initializer_list<Attribute> attributes);

  void Enable() const;

  static std::shared_ptr<BufferLayout>
  Create(std::initializer_list<Attribute> bufferLayout) {
    return std::make_shared<BufferLayout>(bufferLayout);
  }
};
} // namespace Automata

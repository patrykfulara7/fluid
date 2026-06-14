#pragma once

#include "pch.hxx"

namespace Automata {
class VertexBuffer {
private:
  GLuint ID = 0;

public:
  VertexBuffer(GLsizeiptr size, const GLvoid *data, GLenum usage);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;

  static std::shared_ptr<VertexBuffer>
  Create(GLsizeiptr size, const GLvoid *data, GLenum usage) {
    return std::make_shared<VertexBuffer>(size, data, usage);
  }
};
} // namespace Automata

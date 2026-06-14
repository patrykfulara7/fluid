#pragma once

#include "pch.hxx"

namespace Automata {
class VertexArray {
private:
  GLuint ID;

public:
  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;
};
} // namespace Automata

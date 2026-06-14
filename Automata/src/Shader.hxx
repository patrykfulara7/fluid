#pragma once

#include "pch.hxx"

namespace Automata {
class Shader {
private:
  GLuint ID = 0;

public:
  Shader(const std::unordered_map<GLenum, std::string> &shaderPaths);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  void Set1i(const std::string &name, GLint v0);
  void Set4f(const std::string &name, GLint v0, GLint v1, GLint v2, GLint v3);
  void SetMatrix4fv(const std::string &name, const glm::mat4 &value);
};
} // namespace Automata

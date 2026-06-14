#include "GLTypes.hxx"

namespace Automata {
GLsizei GLTypes::GetSize(GLenum type) {
  switch (type) {
  case GL_FLOAT: {
    return sizeof(GLfloat);
  }
  case GL_UNSIGNED_INT: {
    return sizeof(GLuint);
  }
  }

  return 0;
}
} // namespace Automata

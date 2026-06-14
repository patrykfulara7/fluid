#include "ElementBuffer.hxx"
#include "GLTypes.hxx"

namespace Automata {
ElementBuffer::ElementBuffer(GLsizeiptr size, const GLvoid *data, GLenum usage,
                             GLenum type)
    : type(type), count(size / GLTypes::GetSize(type)) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

ElementBuffer::~ElementBuffer() { glDeleteBuffers(1, &ID); }

void ElementBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }

void ElementBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
} // namespace Automata

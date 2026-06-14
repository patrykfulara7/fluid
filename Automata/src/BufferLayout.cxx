#include "BufferLayout.hxx"
#include "GLTypes.hxx"

namespace Automata {
BufferLayout::BufferLayout(std::initializer_list<Attribute> bufferLayout)
    : bufferLayout(bufferLayout) {
  for (const auto &attribute : bufferLayout) {
    stride += GLTypes::GetSize(attribute.type) * attribute.size;
  }
}

void BufferLayout::Enable() const {
  uint32_t pointer = 0;
  GLuint index = 0;
  for (const auto &attribute : bufferLayout) {
    glVertexAttribPointer(index, attribute.size, attribute.type, GL_FALSE,
                          stride, reinterpret_cast<const GLvoid *>(pointer));
    glEnableVertexAttribArray(index);

    pointer += GLTypes::GetSize(attribute.type) * attribute.size;
    index++;
  }
}
} // namespace Automata

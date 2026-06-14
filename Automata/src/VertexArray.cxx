#include "VertexArray.hxx"

namespace Automata {
VertexArray::VertexArray() { glGenVertexArrays(1, &ID); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &ID); }

void VertexArray::Bind() const { glBindVertexArray(ID); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

} // namespace Automata

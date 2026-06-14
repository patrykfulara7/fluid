#pragma once

#include "pch.hxx"

namespace Automata {
class FileIO {
public:
  static std::string ReadFile(const std::string &path);
};
} // namespace Automata

#include "FileIO.hxx"

namespace Automata {
std::string FileIO::ReadFile(const std::string &path) {
  std::ifstream fileStream(path);
  std::stringstream stringStream;
  std::string line;

  ASSERT(fileStream.is_open());

  while (std::getline(fileStream, line)) {
    stringStream << line << '\n';
  }

  fileStream.close();

  return stringStream.str();
}
}; // namespace Automata

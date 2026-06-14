#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <sstream>

#include <unordered_map>
#include <vector>

#include <memory>

#ifdef DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif

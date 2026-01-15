#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "spdlog/spdlog.h"
#include "glad/gl.h"

namespace xe {
      GLuint create_texture(const std::string &name, bool is_sRGB = true);
}
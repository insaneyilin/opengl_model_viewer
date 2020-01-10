#pragma once

#include "glsl_shader.h"

namespace ogl_viewer {

class Drawable {
 public:
  Drawable() = default;
  virtual ~Drawable() = default;

  virtual void Draw(GLSLShader *shader) const = 0;
};

}  // namespace ogl_viewer

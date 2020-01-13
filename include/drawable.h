#pragma once

#include "glsl_shader.h"
#include <string>

namespace ogl_viewer {

class Drawable {
 public:
  Drawable() = default;
  virtual ~Drawable() = default;

  virtual void Draw(GLSLShader *shader) const = 0;
  virtual bool LoadDataFromFile(const std::string &filepath) {
    return false;
  }
};

class PointCloud : public Drawable {
 public:
  PointCloud() = default;
  ~PointCloud() override;
  void Draw(GLSLShader *shader) const override;
  bool LoadDataFromFile(const std::string &filepath) override;

 private:
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  int num_points_ = 0;
  int stride_ = 0;
};

}  // namespace ogl_viewer

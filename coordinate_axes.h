#pragma once
#include "drawable.h"

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <vector>

namespace ogl_viewer {

class CoordinateAxes : public Drawable {
 public:
  CoordinateAxes();
  ~CoordinateAxes();

  void Draw(GLSLShader *shader) const override;

 private:
  std::vector<Eigen::Vector3f, Eigen::aligned_allocator<Eigen::Vector3f> > vertices_;
  std::vector<Eigen::Vector4f, Eigen::aligned_allocator<Eigen::Vector4f> > colors_;

  int num_vertices_ = 0;
  int num_indices_ = 0;

  GLuint vao_;
  GLuint vbo_;
  GLuint cbo_;
  GLuint ebo_;
};

}  // namespace ogl_viewer

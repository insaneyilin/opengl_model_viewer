#include "coordinate_axes.h"

#include <iostream>

namespace ogl_viewer {

CoordinateAxes::CoordinateAxes() {
  vertices_.push_back(Eigen::Vector3f::Zero());
  vertices_.push_back(Eigen::Vector3f::UnitX());
  vertices_.push_back(Eigen::Vector3f::Zero());
  vertices_.push_back(Eigen::Vector3f::UnitY());
  vertices_.push_back(Eigen::Vector3f::Zero());
  vertices_.push_back(Eigen::Vector3f::UnitZ());

  colors_.push_back(Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
  colors_.push_back(Eigen::Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
  colors_.push_back(Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f));
  colors_.push_back(Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f));
  colors_.push_back(Eigen::Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
  colors_.push_back(Eigen::Vector4f(0.0f, 0.0f, 1.0f, 1.0f));

  num_vertices_ = vertices_.size();
  vao_ = 0;
  vbo_ = 0;
  cbo_ = 0;
  ebo_ = 0;

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // use a cuboid(a set of triangles) to represent a 'line segment'
  // bind vertex buffer
  std::vector<Eigen::Vector3f, Eigen::aligned_allocator<Eigen::Vector3f>> vertices_ext(
      vertices_.size() * 4);
  const float line_width = 0.01f;
  for (std::size_t i = 0; i < vertices_.size(); i += 2) {
    Eigen::Vector3f line_direction = vertices_[i + 1] - vertices_[i];
    // construct local axes
    Eigen::Vector3f axis =
        std::abs(line_direction.normalized().dot(Eigen::Vector3f::UnitZ())) < 0.9f ?
            Eigen::Vector3f::UnitZ() : Eigen::Vector3f::UnitX();
    Eigen::Vector3f local_x = axis.cross(line_direction).normalized();
    Eigen::Vector3f local_y = local_x.cross(line_direction).normalized();

    const auto local_x_offset = local_x * line_width;
    const auto local_y_offset = local_y * line_width;
    vertices_ext[i * 4] = vertices_[i] - local_x_offset;
    vertices_ext[i * 4 + 1] = vertices_[i + 1] - local_x_offset;
    vertices_ext[i * 4 + 2] = vertices_[i] + local_x_offset;
    vertices_ext[i * 4 + 3] = vertices_[i + 1] + local_x_offset;
    vertices_ext[i * 4 + 4] = vertices_[i] - local_y_offset;
    vertices_ext[i * 4 + 5] = vertices_[i + 1] - local_y_offset;
    vertices_ext[i * 4 + 6] = vertices_[i] + local_y_offset;
    vertices_ext[i * 4 + 7] = vertices_[i + 1] + local_y_offset;
  }
  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_ext.size() * 3,
      vertices_ext.data(), GL_STATIC_DRAW);

  // bind color buffer
  std::vector<Eigen::Vector4f, Eigen::aligned_allocator<Eigen::Vector4f>> colors_ext(
      colors_.size() * 4);
  for (std::size_t i = 0; i < colors_.size(); i += 2) {
    for (int j = 0; j < 4; ++j) {
      colors_ext[i * 4 + j * 2] = colors_[i];
      colors_ext[i * 4 + j * 2 + 1] = colors_[i + 1];
    }
  }

  glGenBuffers(1, &cbo_);
  glBindBuffer(GL_ARRAY_BUFFER, cbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors_ext.size() * 4,
      colors_ext.data(), GL_STATIC_DRAW);

  // bind element buffer (draw triangles)
  // triangle vertex indices of a 'line segment'
  const std::vector<int> sub_indices = {
      0, 1, 4,
      1, 5, 4,
      4, 5, 2,
      5, 3, 2,
      2, 3, 6,
      3, 6, 7,
      6, 7, 0,
      7, 1, 0
  };
  std::vector<int> indices;
  for (std::size_t i = 0; i < vertices_ext.size(); i += 8) {
    for (std::size_t j = 0; j < sub_indices.size(); ++j) {
      indices.push_back(sub_indices[j] + i);
    }
  }
  num_indices_ = indices.size();

  glGenBuffers(1, &ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(),
      indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

CoordinateAxes::~CoordinateAxes() {
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &cbo_);
  glDeleteBuffers(1, &ebo_);
  glDeleteVertexArrays(1, &vao_);
}

void CoordinateAxes::Draw(GLSLShader *shader) const {
  glBindVertexArray(vao_);

  GLint position_loc = shader->GetAttribLocation("vert_position");
  glEnableVertexAttribArray(position_loc);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  GLint color_loc = shader->GetAttribLocation("vert_color");
  glEnableVertexAttribArray(color_loc);
  glBindBuffer(GL_ARRAY_BUFFER, cbo_);
  glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableVertexAttribArray(position_loc);
  glDisableVertexAttribArray(color_loc);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

}  // namespace ogl_viewer

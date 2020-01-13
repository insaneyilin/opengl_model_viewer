#include "drawable.h"

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

namespace ogl_viewer {

PointCloud::~PointCloud() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
}

void PointCloud::Draw(GLSLShader *shader) const {
  if (num_points_ == 0) {
    return;
  }

  GLint position_loc = shader->GetAttribLocation("vert_position");
  glBindVertexArray(vao_);
  glEnableVertexAttribArray(position_loc);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_, 0);

  glDrawArrays(GL_POINTS, 0, num_points_);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
}

bool PointCloud::LoadDataFromFile(const std::string &filepath) {
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  if (pcl::io::loadPCDFile<pcl::PointXYZ>(filepath, *cloud) == -1) {
    std::cerr << "Cannot read " << filepath << "\n";
    return false;
  }

  num_points_ = cloud->size();
  stride_ = sizeof(pcl::PointXYZ);

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, num_points_ * stride_,
      cloud->points.data(), GL_STATIC_DRAW);

  return true;
}

}  // namespace ogl_viewer

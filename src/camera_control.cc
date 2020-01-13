#include "camera_control.h"

#include <cmath>

namespace ogl_viewer {

Eigen::Matrix4f CameraControl::GetProjectionMatrix() const {
  Eigen::Matrix4f proj_mat = Eigen::Matrix4f::Zero();
  float aspect_ratio = window_width_ / window_height_;
  const float rad = fovy_ * M_PI / 180.0;
  const float tan_half_fovy = std::tan(rad / 2.f);
  // refer to glm::perspective
  proj_mat(0, 0) = 1.f / (aspect_ratio * tan_half_fovy);
  proj_mat(1, 1) = 1.f / tan_half_fovy;
  proj_mat(2, 2) = -(z_far_ + z_near_) / (z_far_ - z_near_);
  proj_mat(2, 3) = -1.f;
  proj_mat(3, 2) = -(2.f * z_far_ * z_near_) / (z_far_ - z_near_);

  // glm's matrix is in column-major, so here we need to
  // return transposed matrix
  return proj_mat.transpose();
}

ArcCameraControl::ArcCameraControl() {
  center_ = Eigen::Vector3d::Zero();
  distance_ = 30.0;
  last_cursor_pos_ = Eigen::Vector2d::Zero();
  theta_ = -45.0 * M_PI / 180.0;
  phi_ = theta_;

  CameraControl::left_button_down_ = false;
  CameraControl::right_button_down_ = false;
  CameraControl::middle_button_down_ = false;
}

ArcCameraControl::~ArcCameraControl() {
}

void ArcCameraControl::OnMouseButton(double x, double y,
    int button, bool press_down) {
  switch (button) {
  case 0:
    CameraControl::left_button_down_ = press_down;
    break;
  case 1:
    CameraControl::right_button_down_ = press_down;
    break;
  case 2:
    CameraControl::middle_button_down_ = press_down;
    break;
  default:
    break;
  }
  last_cursor_pos_[0] = x;
  last_cursor_pos_[1] = y;
}

void ArcCameraControl::OnMouseMove(double x, double y) {
  if (!CameraControl::left_button_down_ && !CameraControl::right_button_down_ &&
      !CameraControl::middle_button_down_) {
    return;
  }
  double dx = x - last_cursor_pos_[0];
  double dy = y - last_cursor_pos_[1];
  if (left_button_down_) {
    theta_ -= dx * 0.01f;
    phi_ -= dy * 0.01f;
    phi_ = std::min(M_PI_2 - 0.01, std::max(-M_PI_2 + 0.01, phi_));
  } else if (right_button_down_) {
    center_ += Eigen::AngleAxisd(theta_ + M_PI_2,
        Eigen::Vector3d::UnitZ()) * Eigen::Vector3d(-dx, dy, 0.0) *
        distance_ * 0.001;
  }
  last_cursor_pos_[0] = x;
  last_cursor_pos_[1] = y;
}

void ArcCameraControl::OnMouseScroll(double xoffset, double yoffset) {
  if (yoffset > 0) {
    distance_ *= 0.9;
  } else {
    distance_ *= 1.1;
  }
  distance_ = std::max(0.1, distance_);
}

Eigen::Matrix4f ArcCameraControl::GetViewMatrix() const {
  Eigen::Quaternionf quat = Eigen::AngleAxisf(theta_,
      Eigen::Vector3f::UnitZ()) *
      Eigen::AngleAxisf(phi_, Eigen::Vector3f::UnitY());
  Eigen::Vector3f offset = quat * Eigen::Vector3f(distance_, 0.0f, 0.0f);
  Eigen::Vector3f center = center_.cast<float>();
  Eigen::Vector3f eye = center + offset;
  Eigen::Vector3f up = Eigen::Vector3f::UnitZ();

  Eigen::Vector3f f = (center - eye).normalized();
  Eigen::Vector3f s = f.cross(up).normalized();
  Eigen::Vector3f u = s.cross(f);

  Eigen::Matrix4f view_mat = Eigen::Matrix4f::Identity();
  // refer to glm::lookAt
  view_mat(0, 0) = s[0];
  view_mat(1, 0) = s[1];
  view_mat(2, 0) = s[2];
  view_mat(0, 1) = u[0];
  view_mat(1, 1) = u[1];
  view_mat(2, 1) = u[2];
  view_mat(0, 2) = -f[0];
  view_mat(1, 2) = -f[1];
  view_mat(2, 2) = -f[2];
  view_mat(3, 0) = -s.dot(eye);
  view_mat(3, 1) = -u.dot(eye);
  view_mat(3, 2) = f.dot(eye);

  return view_mat.transpose();
}

}  // namespace ogl_viewer

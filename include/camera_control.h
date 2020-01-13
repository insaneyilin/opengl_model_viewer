#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace ogl_viewer {

class CameraControl {
 public:
  CameraControl() = default;
  virtual ~CameraControl() = default;

  Eigen::Matrix4f GetProjectionMatrix() const;
  void SetWindowSize(int width, int height) {
    window_width_ = width;
    window_height_ = height;
  }

  virtual void OnMouseButton(double x, double y,
      int button, bool press_down) = 0;
  virtual void OnMouseMove(double x, double y) = 0;
  virtual void OnMouseScroll(double xoffset, double yoffset) = 0;

  virtual Eigen::Matrix4f GetViewMatrix() const = 0;

 protected:
  bool left_button_down_ = false;
  bool middle_button_down_ = false;
  bool right_button_down_ = false;

  float window_width_ = 100.f;
  float window_height_ = 100.f;

  float fovy_ = 30.f;
  float z_near_ = 0.1f;
  float z_far_ = 1000.f;
};

class ArcCameraControl : public CameraControl {
 public:
  ArcCameraControl();
  ~ArcCameraControl() override;

  void OnMouseButton(double x, double y,
      int button, bool press_down) override;
  void OnMouseMove(double x, double y) override;
  void OnMouseScroll(double xoffset, double yoffset) override;

  Eigen::Matrix4f GetViewMatrix() const override;

 private:
  Eigen::Vector3d center_;
  double distance_ = 10.0;
  Eigen::Vector2d last_cursor_pos_;
  double theta_ = 0.0;
  double phi_ = 0.0;
};

}  // namespace ogl_viewer

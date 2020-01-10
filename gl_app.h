#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#undef GLFW_DLL

#include <GLFW/glfw3.h>
#include <Eigen/Core>

#include "drawable.h"
#include "camera_control.h"

namespace ogl_viewer {

class GLApp {
 public:
  GLApp();
  virtual ~GLApp();

  virtual bool Init(const char* window_name, int width, int height,
      const char* glsl_version = "#version 330");

  void Run();

  void Close();

  Eigen::Vector2i FrameBufferSize();

  virtual void Draw();

  static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);
  static void MouseButtonCallback(GLFWwindow* window, int button,
      int action, int mods);
  static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
  static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

 protected:
  GLFWwindow *glfw_window_ = nullptr;
  std::unique_ptr<GLSLShader> shader_;
  std::unique_ptr<Drawable> coord_axes_;
  std::unique_ptr<CameraControl> camera_control_;
};

}  // namespace ogl_viewer

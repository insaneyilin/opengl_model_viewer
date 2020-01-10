#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#undef GLFW_DLL

#include <GLFW/glfw3.h>
#include <Eigen/Core>

#include "drawable.h"

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

 protected:
  virtual void FrameBufferSizeCallback(int width, int height);

 protected:
  GLFWwindow *glfw_window_ = nullptr;
  std::unique_ptr<GLSLShader> shader_;
  std::unique_ptr<Drawable> coord_axes_;
};

}  // namespace ogl_viewer

#include "gl_app.h"
#include <iostream>

#include "coordinate_axes.h"

namespace ogl_viewer {

GLApp::GLApp() {
}

GLApp::~GLApp() {
  if(!glfw_window_) {
    return;
  }

  glfwDestroyWindow(glfw_window_);
  glfwTerminate();
}

bool GLApp::Init(const char* window_name, int width, int height,
    const char* glsl_version) {
  glfwSetErrorCallback([](int err_code, const char* desc) {
    std::cerr << "glfw error " << err_code << ": " << desc << "\n";
  });

  if (!glfwInit()) {
    std::cerr << "failed to initialize GLFW.\n";
    return false;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  glfw_window_ = glfwCreateWindow(width, height, window_name,
      nullptr, nullptr);
  if (glfw_window_ == nullptr) {
    std::cerr << "failed to create GLFW window.\n";
    return false;
  }
  glfwMakeContextCurrent(glfw_window_);
  glfwSwapInterval(1);

  // set callbacks
  glfwSetWindowUserPointer(glfw_window_, this);  // pass user-defined pointer
  glfwSetFramebufferSizeCallback(glfw_window_, GLApp::FrameBufferSizeCallback);

  if (glewInit() != 0) {
    std::cerr << "failed to init GLEW.\n";
    return false;
  }

  // TODO: init shader with config
  shader_.reset(new GLSLShader);
  shader_->Init("./rainbow.vert", "./rainbow.frag");
  // shader_->Init("./simple.vert", "./red.frag");

  // coordinates axes
  coord_axes_.reset(new CoordinateAxes);

  return true;
}

void GLApp::Run() {
  int display_w = 0;
  int display_h = 0;
  while(!glfwWindowShouldClose(glfw_window_)) {
    glfwPollEvents();

    glfwGetFramebufferSize(glfw_window_, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Draw();

    glfwSwapBuffers(glfw_window_);
  }
}

void GLApp::Close() {
  glfwSetWindowShouldClose(glfw_window_, 1);
}

Eigen::Vector2i GLApp::FrameBufferSize() {
  int width = 0;
  int height = 0;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  return Eigen::Vector2i(width, height);
}

void GLApp::Draw() {
  shader_->Use();  // don't forget to "use" our shader

  // TODO: add camera control
  Eigen::Matrix4f view_matrix = Eigen::Matrix4f::Identity();
  Eigen::Matrix4f projection_matrix = Eigen::Matrix4f::Identity();

  shader_->SetUniform("view_matrix", view_matrix);
  shader_->SetUniform("projection_matrix", projection_matrix);

  shader_->SetUniform("color_mode", 2);
  shader_->SetUniform("model_matrix",
      (Eigen::UniformScaling<float>(3.0f) * Eigen::Isometry3f::Identity()).matrix());

  coord_axes_->Draw(shader_.get());
}

void GLApp::FrameBufferSizeCallback(int width, int height) {
  // std::cout << "GLApp::FrameBufferSizeCallback.\n";
}

void GLApp::FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  void *user_data = glfwGetWindowUserPointer(window);
  if (!user_data) {
    return;
  }
  GLApp *gl_app = static_cast<GLApp*>(user_data);
  gl_app->FrameBufferSizeCallback(width, height);
}

}  // namespace ogl_viewer

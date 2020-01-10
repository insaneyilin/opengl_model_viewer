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
  glfwSetMouseButtonCallback(glfw_window_, GLApp::MouseButtonCallback);
  glfwSetCursorPosCallback(glfw_window_, GLApp::CursorPosCallback);
  glfwSetScrollCallback(glfw_window_, GLApp::ScrollCallback);

  if (glewInit() != 0) {
    std::cerr << "failed to init GLEW.\n";
    return false;
  }

  glEnable(GL_DEPTH_TEST);

  // TODO: init shader with config
  shader_.reset(new GLSLShader);
  shader_->Init("./rainbow.vert", "./rainbow.frag");
  // shader_->Init("./simple.vert", "./red.frag");

  // coordinates axes
  coord_axes_.reset(new CoordinateAxes);

  // camera control
  camera_control_.reset(new ArcCameraControl());
  camera_control_->SetWindowSize(width, height);

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

  Eigen::Matrix4f view_matrix = camera_control_->GetViewMatrix();
  Eigen::Matrix4f projection_matrix = camera_control_->GetProjectionMatrix();

  shader_->SetUniform("view_matrix", view_matrix);
  shader_->SetUniform("projection_matrix", projection_matrix);

  shader_->SetUniform("z_clipping", 0);
  shader_->SetUniform("z_range", Eigen::Vector2f(-100.f, 100.f));

  shader_->SetUniform("color_mode", 2);
  shader_->SetUniform("model_matrix",
      (Eigen::UniformScaling<float>(3.0f) * Eigen::Isometry3f::Identity()).matrix());

  coord_axes_->Draw(shader_.get());
}

void GLApp::FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  void *user_data = glfwGetWindowUserPointer(window);
  if (!user_data) {
    return;
  }
  GLApp *gl_app = static_cast<GLApp*>(user_data);
  gl_app->camera_control_->SetWindowSize(width, height);
}

void GLApp::MouseButtonCallback(GLFWwindow* window, int button,
    int action, int mods) {
  void *user_data = glfwGetWindowUserPointer(window);
  if (!user_data) {
    return;
  }
  GLApp *gl_app = static_cast<GLApp*>(user_data);
  bool button_press_down = (action == GLFW_PRESS);
  double x = 0.0;
  double y = 0.0;
  glfwGetCursorPos(window, &x, &y);
  gl_app->camera_control_->OnMouseButton(x, y, button, button_press_down);
}

void GLApp::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  void *user_data = glfwGetWindowUserPointer(window);
  if (!user_data) {
    return;
  }
  GLApp *gl_app = static_cast<GLApp*>(user_data);
  gl_app->camera_control_->OnMouseMove(xpos, ypos);
}

void GLApp::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  void *user_data = glfwGetWindowUserPointer(window);
  if (!user_data) {
    return;
  }
  GLApp *gl_app = static_cast<GLApp*>(user_data);
  gl_app->camera_control_->OnMouseScroll(xoffset, yoffset);
}

}  // namespace ogl_viewer

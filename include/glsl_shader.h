#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#undef GLFW_DLL

#include <memory>
#include <string>
#include <unordered_map>

#include <Eigen/Core>

namespace ogl_viewer {

class GLSLShader {
 public:
  GLSLShader() = default;
  ~GLSLShader() = default;

  bool Init(const std::string& vertex_shader_filepath,
      const std::string& fragment_shader_filepath);

  void Use() const {
    glUseProgram(shader_program_);
  }

  /** @brief get attribute variable location **/
  GLint GetAttribLocation(const std::string& name);

  /** @brief get uniform variable location **/
  GLint GetUniformLocation(const std::string& name);

  Eigen::Vector4f GetUniform4f(const std::string& name) {
    Eigen::Vector4f vec;
    glGetUniformfv(shader_program_, GetUniformLocation(name), vec.data());
    return vec;
  }

  Eigen::Matrix4f GetUniformMatrix4f(const std::string& name) {
    Eigen::Matrix4f mat;
    glGetUniformfv(shader_program_, GetUniformLocation(name), mat.data());
    return mat;
  }

  void SetUniform(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
  }

  void SetUniform(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
  }

  void SetUniform(const std::string& name, const Eigen::Vector2f& vector) {
    glUniform2fv(GetUniformLocation(name), 1, vector.data());
  }

  void SetUniform(const std::string& name, const Eigen::Vector3f& vector) {
    glUniform3fv(GetUniformLocation(name), 1, vector.data());
  }

  void SetUniform(const std::string& name, const Eigen::Vector4f& vector) {
    glUniform4fv(GetUniformLocation(name), 1, vector.data());
  }

  void SetUniform(const std::string& name, const Eigen::Vector4i& vector) {
    glUniform4iv(GetUniformLocation(name), 1, vector.data());
  }

  void SetUniform(const std::string& name, const Eigen::Matrix4f& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix.data());
  }

 private:
  GLuint LoadShaderFromFile(const std::string& filepath, GLuint shader_type);

 private:
  GLuint shader_program_ = 0;
  std::unordered_map<std::string, GLint> attrib_map_;
  std::unordered_map<std::string, GLint> uniform_map_;
};

}  // namespace ogl_viewer

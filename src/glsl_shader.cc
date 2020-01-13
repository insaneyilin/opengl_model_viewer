#include "glsl_shader.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ogl_viewer {

bool GLSLShader::Init(const std::string& vertex_shader_filepath,
    const std::string& fragment_shader_filepath) {
  GLuint vertex_shader = LoadShaderFromFile(vertex_shader_filepath,
      GL_VERTEX_SHADER);
  GLuint fragment_shader = LoadShaderFromFile(fragment_shader_filepath,
      GL_FRAGMENT_SHADER);

  shader_program_ = glCreateProgram();
  glAttachShader(shader_program_, vertex_shader);
  glAttachShader(shader_program_, fragment_shader);
  glLinkProgram(shader_program_);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  GLint result = GL_FALSE;
  int info_log_length = -1;

  glGetProgramiv(shader_program_, GL_LINK_STATUS, &result);
  glGetProgramiv(shader_program_, GL_INFO_LOG_LENGTH, &info_log_length);
  std::vector<char> error_message(info_log_length);
  glGetProgramInfoLog(shader_program_, info_log_length, nullptr, error_message.data());

  if (result != GL_TRUE) {
    std::cerr << "error : failed to link program.\n";
    std::cerr << std::string(error_message.begin(), error_message.end()) << "\n";
    return false;
  }

  return true;
}

GLint GLSLShader::GetAttribLocation(const std::string& name) {
  auto iter = attrib_map_.find(name);
  if (iter != attrib_map_.end()) {
    return iter->second;
  }

  GLint id = glGetAttribLocation(shader_program_, name.c_str());
  if (id == -1) {
    std::cerr << "warning : attrib " << name << " not found.\n";
  }

  attrib_map_[name] = id;
  return id;
}

GLint GLSLShader::GetUniformLocation(const std::string& name) {
  auto iter = uniform_map_.find(name);
  if (iter != uniform_map_.end()) {
    return iter->second;
  }

  GLint id = glGetUniformLocation(shader_program_, name.c_str());
  if (id == -1) {
    std::cerr << "warning : uniform " << name << " not found.\n";
  }

  uniform_map_[name] = id;
  return id;
}

GLuint GLSLShader::LoadShaderFromFile(const std::string& filepath,
    GLuint shader_type) {
  GLuint shader_id = glCreateShader(shader_type);
  std::ifstream ifs(filepath);
  if (!ifs) {
    std::cerr << "error: failed to open " << filepath << "\n";
    return GL_FALSE;
  }

  std::stringstream ss;
  ss << ifs.rdbuf();
  std::string shader_content = ss.str();

  GLint result = GL_FALSE;
  int info_log_length = 0;

  char const* shader_content_ptr = shader_content.c_str();
  glShaderSource(shader_id, 1, &shader_content_ptr, nullptr);
  glCompileShader(shader_id);

  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

  std::vector<char> error_message(info_log_length);
  glGetShaderInfoLog(shader_id, info_log_length, nullptr, error_message.data());

  if (result != GL_TRUE) {
    std::cerr << "error : failed to compile shader " << filepath << "\n";
    std::cerr << std::string(error_message.begin(), error_message.end()) << "\n";
  }

  return shader_id;
}

}  // namespace ogl_viewer

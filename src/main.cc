#include <cstdlib>
#include <iostream>

#include "opengl_model_viewer.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <model_file_path>\n";
    return -1;
  }

  const std::string model_file_path(argv[1]);
  ogl_viewer::OpenGLModelViewer app;
  app.Init("OpenGLModelViewer", 1280, 720, model_file_path);
  app.Run();
  return 0;
}

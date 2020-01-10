#include <cstdlib>
#include <iostream>

#include "gl_app.h"

int main() {
  ogl_viewer::GLApp app;
  app.Init("Hello", 450, 300);
  app.Run();
  return 0;
}

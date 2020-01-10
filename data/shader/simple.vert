#version 330 core
in vec3 vert_position;
in vec4 vert_color;
void main(){
  gl_Position.xyz = vert_position;
  gl_Position.w = 1.0;
}

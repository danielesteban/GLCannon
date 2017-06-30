#include "shader.hpp"

const char* Shader::vertex_src =
" attribute vec4 av4position; "
" attribute vec3 av3color; "
" uniform mat4 mvp; "
" varying vec3 vv3color; "
" void main() { "
"    vv3color = av3color; "
"    gl_Position = mvp * av4position; "
" } ";

const char* Shader::fragment_src =
" precision lowp float; "
" varying vec3 vv3color; "
" void main() { "
"    gl_FragColor = vec4(vv3color, 1.0); "
" } ";

void Shader::process(GLuint *shader, const char *source, GLint shader_type) {
  GLint status = GL_FALSE;
  const char *shaders[1] = { NULL };
  char buffer[1024];
  GLsizei length;

  *shader = glCreateShader(shader_type);
  shaders[0] = source;
  glShaderSource(*shader, 1, shaders, NULL);

  shaders[0] = NULL;

  glCompileShader(*shader);
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    glGetProgramInfoLog(*shader, sizeof(buffer), &length, &buffer[0]);
    buffer[length] = '\0';
    SDL_Log("Shader compilation failed: %s", buffer);fflush(stderr);
  }
}

void Shader::init() {
  process(&vertex, vertex_src, GL_VERTEX_SHADER);
  process(&fragment, fragment_src, GL_FRAGMENT_SHADER);

  program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  position = glGetAttribLocation(program, "av4position");
  color = glGetAttribLocation(program, "av3color");
  mvp = glGetUniformLocation(program, "mvp");

  glUseProgram(program);
  glEnableVertexAttribArray(position);
  glEnableVertexAttribArray(color);
}

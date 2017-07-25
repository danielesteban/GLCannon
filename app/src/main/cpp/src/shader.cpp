#include "shader.hpp"

Shader::Shader(
  const char *vertex,
  const char *fragment
) : vertex_src(vertex), fragment_src(fragment) {}

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

  position = glGetAttribLocation(program, "position");
  normal = glGetAttribLocation(program, "normal");
  uv = glGetAttribLocation(program, "uv");
  projection = glGetUniformLocation(program, "projection");
  view = glGetUniformLocation(program, "view");
  normalView = glGetUniformLocation(program, "normalView");
  texture = glGetUniformLocation(program, "texture");
  albedo = glGetUniformLocation(program, "albedo");
  fogColor = glGetUniformLocation(program, "fogColor");
  sunPosition = glGetUniformLocation(program, "sunPosition");
}

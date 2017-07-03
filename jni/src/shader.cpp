#include "shader.hpp"

const char* Shader::vertex_src =
" attribute vec4 position; "
" attribute vec2 uv; "
" varying vec4 fragmentPosition; "
" varying vec2 fragmentUV; "
" uniform mat4 projection; "
" uniform mat4 view; "
" void main() { "
"    fragmentUV = uv; "
"    fragmentPosition = view * position; "
"    gl_Position = projection * view * position; "
" } ";

const char* Shader::fragment_src =
" precision mediump float; "
" varying vec4 fragmentPosition; "
" varying vec2 fragmentUV; "
" uniform sampler2D texture; "
" const float LOG2 = 1.442695; "
" const float density = 0.03; "
" const vec4 fog = vec4(0.0, 0.0, 0.0, 1.0); "
" void main() { "
"   float distance = length(fragmentPosition); "
"	  float fogFactor = clamp(exp2(-density * density * distance * distance * LOG2), 0.0, 1.0); "
"   gl_FragColor = mix(fog, texture2D(texture, fragmentUV), fogFactor); "
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

  position = glGetAttribLocation(program, "position");
  uv = glGetAttribLocation(program, "uv");
  projection = glGetUniformLocation(program, "projection");
  view = glGetUniformLocation(program, "view");
  texture = glGetUniformLocation(program, "texture");
}

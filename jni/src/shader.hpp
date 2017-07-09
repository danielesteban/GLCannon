#pragma once

#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

class Shader {
  public:
    Shader(const char *vertex, const char *fragment);
    void init();
    GLuint program;
    GLint position;
    GLint uv;
    GLint projection;
    GLint view;
    GLint texture;
  private:
    const char *vertex_src;
    const char *fragment_src;
    GLuint fragment;
    GLuint vertex;
    void process(GLuint *shader, const char *source, GLint shader_type);
};

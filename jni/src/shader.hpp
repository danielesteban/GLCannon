#ifndef SHADER_H
#define SHADER_H

#include <SDL.h>
#include <SDL_opengles2.h>

class Shader {
  public:
    void init();
    GLuint program;
    GLint position;
    GLint color;
    GLint mvp;
  private:
    static const char* vertex_src;
    static const char* fragment_src;
    GLuint fragment;
    GLuint vertex;
    void process(GLuint *shader, const char *source, GLint shader_type);
};

#endif

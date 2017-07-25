#pragma once

#include <SDL.h>
#include <SDL_image.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <vector>

class Texture {
  public:
    Texture(const char *filename, const GLenum target, const GLint format, const GLint wrap);

    char *filename;
    const GLenum target;
    const GLint format;
    const GLint wrap;
    GLuint buffer;

    static std::vector<Texture> cache;
    static const GLuint loadFromCache(const char *filename, const GLenum target, const GLint format, const GLint wrap);
};

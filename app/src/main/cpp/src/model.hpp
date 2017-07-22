#pragma once

#include <SDL.h>
#include <SDL_image.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <btBulletDynamicsCommon.h>
#include "shader.hpp"

class Model {
  public:
    Model(
      const GLfloat *VBO,
      const GLsizei VBOSIZE,
      const GLushort EBO[],
      const GLsizei EBOSIZE,
      const GLsizei EBOCOUNT,
      const char *TEXTURE = NULL,
      const GLenum TEXTURETARGET = GL_TEXTURE_2D,
      const GLint TEXTUREFORMAT = GL_RGB,
      const GLint TEXTUREWRAP = GL_REPEAT
    );
    virtual void init(Shader *shader);
    void initTexture();
    void render(const GLfloat *view, const GLfloat *normalView);
    btCollisionShape *collision;
    Shader *shader;
    const char *textureFilename;
  private:
    GLuint vao;
    GLuint vbo;
    const GLfloat *vboData;
    const GLsizei vboSize;
    GLuint ebo;
    const GLushort *eboData;
    const GLsizei eboSize;
    const GLsizei eboCount;
    GLuint texture;
    const GLenum textureTarget;
    const GLint textureFormat;
    const GLint textureWrap;
};

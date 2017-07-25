#pragma once

#include <SDL.h>
#include <SDL_image.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <btBulletDynamicsCommon.h>
#include "shader.hpp"
#include "texture.hpp"

class Model {
  public:
    Model(
      const GLfloat VBO[],
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
    void updateTexture(const char *filename);
    void render(const GLfloat *view, const GLfloat *normalView);
    btCollisionShape *collision;
    Shader *shader;
  protected:
    void updateVBO();
    const GLfloat *vboData;
    GLsizei vboSize;
    const GLushort *eboData;
    GLsizei eboSize;
    GLsizei eboCount;
    const char *textureFilename;
    const GLenum textureTarget;
    const GLint textureFormat;
    const GLint textureWrap;
  private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint texture;
};

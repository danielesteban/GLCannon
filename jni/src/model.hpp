#ifndef MODEL_H
#define MODEL_H

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
      const char *TEXTURE,
      const GLenum TEXTURETARGET = GL_TEXTURE_2D
    );
    virtual void init(Shader *shader);
    void render(const GLfloat *view);
    btCollisionShape *collision;
  protected:
    Shader *shader;
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
    const char *textureFilename;
    const GLenum textureTarget;
};

#endif

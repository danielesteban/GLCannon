#include "model.hpp"

Model::Model(
  const GLfloat VBO[],
  const GLsizei VBOSIZE,
  const GLushort EBO[],
  const GLsizei EBOSIZE,
  const GLsizei EBOCOUNT,
  const char *TEXTURE,
  const GLenum TEXTURETARGET,
  const GLint TEXTUREFORMAT,
  const GLint TEXTUREWRAP
) : vboData(VBO),
    vboSize(VBOSIZE),
    eboData(EBO),
    eboSize(EBOSIZE),
    eboCount(EBOCOUNT),
    textureFilename(TEXTURE),
    textureTarget(TEXTURETARGET),
    textureFormat(TEXTUREFORMAT),
    textureWrap(TEXTUREWRAP) {

    }

void Model::init(Shader *shader) {
  this->shader = shader;
  collision = NULL;
  texture = 0;
  if (textureFilename != NULL) updateTexture(textureFilename);
  glGenVertexArraysOES(1, &vao);
  glBindVertexArrayOES(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vboSize, vboData, GL_STATIC_DRAW);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboSize, eboData, GL_STATIC_DRAW);
}

void Model::updateTexture(const char *filename) {
  texture = Texture::loadFromCache(filename, textureTarget, textureFormat, textureWrap);
}

void Model::render(const GLfloat *view, const GLfloat *normalView) {
  glUniformMatrix4fv(shader->view, 1, GL_FALSE, view);
  if (shader->normalView != -1) {
    glUniformMatrix3fv(shader->normalView, 1, GL_FALSE, normalView);
  }
  if (texture != 0) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(textureTarget, texture);
    glUniform1i(shader->texture, 0);
  }
  glBindVertexArrayOES(vao);
  glDrawElements(GL_TRIANGLES, eboCount, GL_UNSIGNED_SHORT, 0);
  glBindVertexArrayOES(0);
  if (texture != 0) glBindTexture(textureTarget, 0);
}

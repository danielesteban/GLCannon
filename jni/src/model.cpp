#include "model.hpp"

Model::Model(
  const GLfloat *VBO,
  const GLsizei VBOSIZE,
  const GLushort *EBO,
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
  if (textureFilename != NULL) initTexture();
  else texture = 0;
  glGenVertexArraysOES(1, &vao);
  glBindVertexArrayOES(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vboSize, vboData, GL_STATIC_DRAW);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboSize, eboData, GL_STATIC_DRAW);
}

void Model::initTexture() {
  glGenTextures(1, &texture);
  glBindTexture(textureTarget, texture);
  if (textureTarget == GL_TEXTURE_2D) {
    char *filename = new char[strlen(textureFilename) + 6];
    sprintf(filename, "%s.webp", textureFilename);
    SDL_Surface *surface = IMG_Load(filename);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    // GLfloat anisotropy;
    // glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    SDL_FreeSurface(surface);
    delete [] filename;
    filename = NULL;
  }
  if (textureTarget == GL_TEXTURE_CUBE_MAP) {
    for (unsigned int i = 0; i < 6; i += 1) {
      char *filename = new char[strlen(textureFilename) + 7];
      sprintf(filename, "%s%d.webp", textureFilename, i);
      SDL_Surface *surface = IMG_Load(filename);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureFormat, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
      SDL_FreeSurface(surface);
      delete [] filename;
      filename = NULL;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  glBindTexture(textureTarget, 0);
}

void Model::render(const GLfloat *view) {
  glUniformMatrix4fv(shader->view, 1, GL_FALSE, view);
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

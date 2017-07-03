#include "model.hpp"

Model::Model(
  const GLfloat *VBO,
  const GLsizei VBOSIZE,
  const GLushort *EBO,
  const GLsizei EBOSIZE,
  const GLsizei EBOCOUNT,
  const char *TEXTURE
) : vboData(VBO), vboSize(VBOSIZE), eboData(EBO), eboSize(EBOSIZE), eboCount(EBOCOUNT), textureFilename(TEXTURE) {}

void Model::init(Shader *shader) {
  SDL_Surface *surface = IMG_Load(textureFilename);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  GLfloat anisotropy;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
  glBindTexture(GL_TEXTURE_2D, 0);
  SDL_FreeSurface(surface);

  glGenVertexArraysOES(1, &vao);
  glBindVertexArrayOES(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vboSize, vboData, GL_STATIC_DRAW);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboSize, eboData, GL_STATIC_DRAW);

  glUseProgram(shader->program);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(shader->uv);
  glVertexAttribPointer(shader->uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
  glBindVertexArrayOES(0);

  this->shader = shader;
}

void Model::render(const GLfloat *view) {
  glUniformMatrix4fv(shader->view, 1, GL_FALSE, view);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(shader->texture, 0);
  glBindVertexArrayOES(vao);
  glDrawElements(GL_TRIANGLES, eboCount, GL_UNSIGNED_SHORT, 0);
  glBindVertexArrayOES(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

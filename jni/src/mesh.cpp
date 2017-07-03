#include "mesh.hpp"

Mesh::Mesh(
  const GLfloat vbo[],
  const GLushort ebo[],
  const GLsizei size,
  const GLsizei count,
  const char texture[]
) : modelVBO(vbo), modelEBO(ebo), modelSize(size), modelCount(count), modelTexture(texture) {}

void Mesh::init(Shader *shader, const glm::vec3 *positions, const int positionsSize) {
  SDL_Surface *surface = IMG_Load(modelTexture);
  SDL_PixelFormat *format = surface->format;
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
  glBufferData(GL_ARRAY_BUFFER, modelSize, modelVBO, GL_STATIC_DRAW);
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelCount * sizeof(GLushort), modelEBO, GL_STATIC_DRAW);

  glUseProgram(shader->program);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(shader->uv);
  glVertexAttribPointer(shader->uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
  glBindVertexArrayOES(0);

  transforms.reserve(positionsSize);
  for (int i = 0; i < positionsSize; i += 1) {
    glm::mat4 transform;
    transform = glm::translate(transform, positions[i]);
    transforms.push_back(transform);
  }
  initialTransforms.reserve(positionsSize);
  copy(transforms.begin(), transforms.end(), back_inserter(initialTransforms));
}

void Mesh::render(Camera *camera, Shader *shader) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(shader->texture, 0);
  glBindVertexArrayOES(vao);
  std::vector<glm::mat4>::const_iterator transform;
  for (transform = transforms.begin(); transform != transforms.end(); ++transform) {
    glUniformMatrix4fv(shader->view, 1, GL_FALSE, glm::value_ptr(camera->view * (*transform)));
    glDrawElements(GL_TRIANGLES, modelCount, GL_UNSIGNED_SHORT, 0);
  }
  glBindVertexArrayOES(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

#include "skybox.hpp"

const GLfloat Skybox::VBO[] = {
  /* X     Y      Z */
  /* Front */
  1.0f, -1.0f,  1.0f,
 -1.0f, -1.0f,  1.0f,
 -1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,

  /* Back */
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

  /* Top */
  -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,

  /* Bottom */
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,

  /* left */
  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,

  /* Right */
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
};

const GLsizei Skybox::VBOSIZE = sizeof(Skybox::VBO);

const GLushort Skybox::EBO[] = {
  0, 1, 2,      2, 3, 0,
  4, 5, 6,      6, 7, 4,
  8, 9, 10,     10, 11, 8,
  12, 13, 14,   14, 15, 12,
  16, 17, 18,   18, 19, 16,
  20, 21, 22,   22, 23, 20,
};

const GLsizei Skybox::EBOSIZE = sizeof(Skybox::EBO);
const GLsizei Skybox::EBOCOUNT = sizeof(Skybox::EBO) / sizeof(GLushort);

const char Skybox::TEXTURE[] = "skybox";

void Skybox::init(Shader *shader) {
  Model::init(shader);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glBindVertexArrayOES(0);
}
#include "cube.hpp"

const GLfloat Cube::VBO[] = {
  /* X     Y      Z        U      V */
  /* Front */
  -0.5f, -0.5f,  0.5f,    0.0f,  1.0f,
   0.5f, -0.5f,  0.5f,    1.0f,  1.0f,
   0.5f,  0.5f,  0.5f,    1.0f,  0.0f,
  -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,

  /* Back */
   0.5f, -0.5f, -0.5f,    0.0f,  1.0f,
  -0.5f, -0.5f, -0.5f,    1.0f,  1.0f,
  -0.5f,  0.5f, -0.5f,    1.0f,  0.0f,
   0.5f,  0.5f, -0.5f,    0.0f,  0.0f,

  /* Top */
  -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,
   0.5f,  0.5f,  0.5f,    1.0f,  1.0f,
   0.5f,  0.5f, -0.5f,    1.0f,  0.0f,
  -0.5f,  0.5f, -0.5f,    0.0f,  0.0f,

  /* Bottom */
  -0.5f, -0.5f, -0.5f,    0.0f,  1.0f,
   0.5f, -0.5f, -0.5f,    1.0f,  1.0f,
   0.5f, -0.5f,  0.5f,    1.0f,  0.0f,
  -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,

  /* left */
  -0.5f, -0.5f, -0.5f,    0.0f,  1.0f,
  -0.5f, -0.5f,  0.5f,    1.0f,  1.0f,
  -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,
  -0.5f,  0.5f, -0.5f,    0.0f,  0.0f,

  /* Right */
   0.5f, -0.5f,  0.5f,    0.0f,  1.0f,
   0.5f, -0.5f, -0.5f,    1.0f,  1.0f,
   0.5f,  0.5f, -0.5f,    1.0f,  0.0f,
   0.5f,  0.5f,  0.5f,    0.0f,  0.0f,
};

const GLsizei Cube::VBOSIZE = sizeof(Cube::VBO);

const GLushort Cube::EBO[] = {
  0, 1, 2,      2, 3, 0,
  4, 5, 6,      6, 7, 4,
  8, 9, 10,     10, 11, 8,
  12, 13, 14,   14, 15, 12,
  16, 17, 18,   18, 19, 16,
  20, 21, 22,   22, 23, 20,
};

const GLsizei Cube::EBOSIZE = sizeof(Cube::EBO);
const GLsizei Cube::EBOCOUNT = sizeof(Cube::EBO) / sizeof(GLushort);

const char Cube::TEXTURE[] = "container";

void Cube::init(Shader *shader) {
  Model::init(shader);
  glUseProgram(shader->program);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(shader->uv);
  glVertexAttribPointer(shader->uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
  glBindVertexArrayOES(0);
  collision = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
}

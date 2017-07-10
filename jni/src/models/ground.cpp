#include "ground.hpp"

const GLfloat GroundModel::VBO[] = {
  /* X     Y      Z            NX     NY     NZ       U      V */
  /* Front */
  -128.0f, -1.0f,  128.0f,    0.0f,  0.0f,  1.0f,    0.0f,  256.0f,
   128.0f, -1.0f,  128.0f,    0.0f,  0.0f,  1.0f,    256.0f,  256.0f,
   128.0f,  1.0f,  128.0f,    0.0f,  0.0f,  1.0f,    256.0f,  0.0f,
  -128.0f,  1.0f,  128.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,

  /* Back */
   128.0f, -1.0f, -128.0f,    0.0f,  0.0f, -1.0f,    0.0f,  256.0f,
  -128.0f, -1.0f, -128.0f,    0.0f,  0.0f, -1.0f,    256.0f,  256.0f,
  -128.0f,  1.0f, -128.0f,    0.0f,  0.0f, -1.0f,    256.0f,  0.0f,
   128.0f,  1.0f, -128.0f,    0.0f,  0.0f, -1.0f,    0.0f,  0.0f,

  /* Top */
  -128.0f,  1.0f,  128.0f,    0.0f,  1.0f,  0.0f,    0.0f,  256.0f,
   128.0f,  1.0f,  128.0f,    0.0f,  1.0f,  0.0f,    256.0f,  256.0f,
   128.0f,  1.0f, -128.0f,    0.0f,  1.0f,  0.0f,    256.0f,  0.0f,
  -128.0f,  1.0f, -128.0f,    0.0f,  1.0f,  0.0f,    0.0f,  0.0f,

  /* Bottom */
  -128.0f, -1.0f, -128.0f,    0.0f, -1.0f,  0.0f,    0.0f,  256.0f,
   128.0f, -1.0f, -128.0f,    0.0f, -1.0f,  0.0f,    256.0f,  256.0f,
   128.0f, -1.0f,  128.0f,    0.0f, -1.0f,  0.0f,    256.0f,  0.0f,
  -128.0f, -1.0f,  128.0f,    0.0f, -1.0f,  0.0f,    0.0f,  0.0f,

  /* left */
  -128.0f, -1.0f, -128.0f,   -1.0f,  0.0f,  0.0f,    0.0f,  256.0f,
  -128.0f, -1.0f,  128.0f,   -1.0f,  0.0f,  0.0f,    256.0f,  256.0f,
  -128.0f,  1.0f,  128.0f,   -1.0f,  0.0f,  0.0f,    256.0f,  0.0f,
  -128.0f,  1.0f, -128.0f,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,

  /* Right */
   128.0f, -1.0f,  128.0f,    1.0f,  0.0f,  0.0f,    0.0f,  256.0f,
   128.0f, -1.0f, -128.0f,    1.0f,  0.0f,  0.0f,    256.0f,  256.0f,
   128.0f,  1.0f, -128.0f,    1.0f,  0.0f,  0.0f,    256.0f,  0.0f,
   128.0f,  1.0f,  128.0f,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
};

const GLsizei GroundModel::VBOSIZE = sizeof(GroundModel::VBO);

const GLushort GroundModel::EBO[] = {
  0, 1, 2,      2, 3, 0,
  4, 5, 6,      6, 7, 4,
  8, 9, 10,     10, 11, 8,
  12, 13, 14,   14, 15, 12,
  16, 17, 18,   18, 19, 16,
  20, 21, 22,   22, 23, 20,
};

const GLsizei GroundModel::EBOSIZE = sizeof(GroundModel::EBO);
const GLsizei GroundModel::EBOCOUNT = sizeof(GroundModel::EBO) / sizeof(GLushort);

const char GroundModel::TEXTURE[] = "sand";

void GroundModel::init(Shader *shader) {
  Model::init(shader);
  glUseProgram(shader->program);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(shader->normal);
  glVertexAttribPointer(shader->normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(shader->uv);
  glVertexAttribPointer(shader->uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
  glBindVertexArrayOES(0);
  collision = new btBoxShape(btVector3(128.0f, 1.0f, 128.0f));
}

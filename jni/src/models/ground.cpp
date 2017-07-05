#include "ground.hpp"

const GLfloat Ground::VBO[] = {
  /* X     Y      Z        U      V */
  /* Front */
  -128.0f, -1.0f,  128.0f,    0.0f,  256.0f,
   128.0f, -1.0f,  128.0f,    256.0f,  256.0f,
   128.0f,  1.0f,  128.0f,    256.0f,  0.0f,
  -128.0f,  1.0f,  128.0f,    0.0f,  0.0f,

  /* Back */
   128.0f, -1.0f, -128.0f,    0.0f,  256.0f,
  -128.0f, -1.0f, -128.0f,    256.0f,  256.0f,
  -128.0f,  1.0f, -128.0f,    256.0f,  0.0f,
   128.0f,  1.0f, -128.0f,    0.0f,  0.0f,

  /* Top */
  -128.0f,  1.0f,  128.0f,    0.0f,  256.0f,
   128.0f,  1.0f,  128.0f,    256.0f,  256.0f,
   128.0f,  1.0f, -128.0f,    256.0f,  0.0f,
  -128.0f,  1.0f, -128.0f,    0.0f,  0.0f,

  /* Bottom */
  -128.0f, -1.0f, -128.0f,    0.0f,  256.0f,
   128.0f, -1.0f, -128.0f,    256.0f,  256.0f,
   128.0f, -1.0f,  128.0f,    256.0f,  0.0f,
  -128.0f, -1.0f,  128.0f,    0.0f,  0.0f,

  /* left */
  -128.0f, -1.0f, -128.0f,    0.0f,  256.0f,
  -128.0f, -1.0f,  128.0f,    256.0f,  256.0f,
  -128.0f,  1.0f,  128.0f,    256.0f,  0.0f,
  -128.0f,  1.0f, -128.0f,    0.0f,  0.0f,

  /* Right */
   128.0f, -1.0f,  128.0f,    0.0f,  256.0f,
   128.0f, -1.0f, -128.0f,    256.0f,  256.0f,
   128.0f,  1.0f, -128.0f,    256.0f,  0.0f,
   128.0f,  1.0f,  128.0f,    0.0f,  0.0f,
};

const GLsizei Ground::VBOSIZE = sizeof(Ground::VBO);

const GLushort Ground::EBO[] = {
  0, 1, 2,      2, 3, 0,
  4, 5, 6,      6, 7, 4,
  8, 9, 10,     10, 11, 8,
  12, 13, 14,   14, 15, 12,
  16, 17, 18,   18, 19, 16,
  20, 21, 22,   22, 23, 20,
};

const GLsizei Ground::EBOSIZE = sizeof(Ground::EBO);
const GLsizei Ground::EBOCOUNT = sizeof(Ground::EBO) / sizeof(GLushort);

const char Ground::TEXTURE[] = "sand";

void Ground::init(Shader *shader) {
  Model::init(shader);
  glUseProgram(shader->program);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(shader->uv);
  glVertexAttribPointer(shader->uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
  glBindVertexArrayOES(0);
  collision = new btBoxShape(btVector3(128.0f, 1.0f, 128.0f));
}

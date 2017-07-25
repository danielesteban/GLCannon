#include "ground.hpp"

const GLfloat GroundModel::VBO[] = {
  -128.0000f,  1.000000f, -128.0000f,     0.000000f,  1.000000f,  0.000000f,     0.000000f,  256.0000f,
  -128.0000f,  1.000000f,  128.0000f,     0.000000f,  1.000000f,  0.000000f,     256.0000f,  256.0000f,
   128.0000f,  1.000000f,  128.0000f,     0.000000f,  1.000000f,  0.000000f,     256.0000f,  0.000000f,
   128.0000f,  1.000000f, -128.0000f,     0.000000f,  1.000000f,  0.000000f,     0.000000f,  0.000000f,
};

const GLsizei GroundModel::VBOSIZE = sizeof(GroundModel::VBO);

const GLushort GroundModel::EBO[] = {
     0,    1,    2,
     0,    2,    3,
};

const GLsizei GroundModel::EBOSIZE = sizeof(GroundModel::EBO);
const GLsizei GroundModel::EBOCOUNT = sizeof(GroundModel::EBO) / sizeof(GLushort);

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
  collision = new btBoxShape(btVector3(128.0000f, 1.000000f, 128.0000f));
}

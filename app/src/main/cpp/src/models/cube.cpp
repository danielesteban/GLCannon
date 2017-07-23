#include "cube.hpp"

const GLfloat CubeModel::VBO[] = {
  -0.500000f, -0.500000f, -0.500000f,    -1.000000f,  0.000000f,  0.000000f,     0.000000f,  1.000000f,
  -0.500000f, -0.500000f,  0.500000f,    -1.000000f,  0.000000f,  0.000000f,     1.000000f,  1.000000f,
  -0.500000f,  0.500000f,  0.500000f,    -1.000000f,  0.000000f,  0.000000f,     1.000000f,  0.000000f,
  -0.500000f,  0.500000f, -0.500000f,    -1.000000f,  0.000000f,  0.000000f,     0.000000f,  0.000000f,
  -0.500000f,  0.500000f, -0.500000f,     0.000000f,  1.000000f,  0.000000f,     0.000000f,  1.000000f,
  -0.500000f,  0.500000f,  0.500000f,     0.000000f,  1.000000f,  0.000000f,     1.000000f,  1.000000f,
   0.500000f,  0.500000f,  0.500000f,     0.000000f,  1.000000f,  0.000000f,     1.000000f,  0.000000f,
   0.500000f,  0.500000f, -0.500000f,     0.000000f,  1.000000f,  0.000000f,     0.000000f,  0.000000f,
   0.500000f,  0.500000f, -0.500000f,     1.000000f,  0.000000f,  0.000000f,     0.000000f,  1.000000f,
   0.500000f,  0.500000f,  0.500000f,     1.000000f,  0.000000f,  0.000000f,     1.000000f,  1.000000f,
   0.500000f, -0.500000f,  0.500000f,     1.000000f,  0.000000f,  0.000000f,     1.000000f,  0.000000f,
   0.500000f, -0.500000f, -0.500000f,     1.000000f,  0.000000f,  0.000000f,     0.000000f,  0.000000f,
   0.500000f, -0.500000f, -0.500000f,     0.000000f, -1.000000f,  0.000000f,     0.000000f,  1.000000f,
   0.500000f, -0.500000f,  0.500000f,     0.000000f, -1.000000f,  0.000000f,     1.000000f,  1.000000f,
  -0.500000f, -0.500000f,  0.500000f,     0.000000f, -1.000000f,  0.000000f,     1.000000f,  0.000000f,
  -0.500000f, -0.500000f, -0.500000f,     0.000000f, -1.000000f,  0.000000f,     0.000000f,  0.000000f,
  -0.500000f,  0.500000f, -0.500000f,     0.000000f,  0.000000f, -1.000000f,     0.000000f,  1.000000f,
   0.500000f,  0.500000f, -0.500000f,     0.000000f,  0.000000f, -1.000000f,     1.000000f,  1.000000f,
   0.500000f, -0.500000f, -0.500000f,     0.000000f,  0.000000f, -1.000000f,     1.000000f,  0.000000f,
  -0.500000f, -0.500000f, -0.500000f,     0.000000f,  0.000000f, -1.000000f,     0.000000f,  0.000000f,
   0.500000f,  0.500000f,  0.500000f,     0.000000f,  0.000000f,  1.000000f,     0.000000f,  1.000000f,
  -0.500000f,  0.500000f,  0.500000f,     0.000000f,  0.000000f,  1.000000f,     1.000000f,  1.000000f,
  -0.500000f, -0.500000f,  0.500000f,     0.000000f,  0.000000f,  1.000000f,     1.000000f,  0.000000f,
   0.500000f, -0.500000f,  0.500000f,     0.000000f,  0.000000f,  1.000000f,     0.000000f,  0.000000f,
};

const GLsizei CubeModel::VBOSIZE = sizeof(CubeModel::VBO);

const GLushort CubeModel::EBO[] = {
     0,    1,    2,
     0,    2,    3,
     4,    5,    6,
     4,    6,    7,
     8,    9,   10,
     8,   10,   11,
    12,   13,   14,
    12,   14,   15,
    16,   17,   18,
    16,   18,   19,
    20,   21,   22,
    20,   22,   23,
};

const GLsizei CubeModel::EBOSIZE = sizeof(CubeModel::EBO);
const GLsizei CubeModel::EBOCOUNT = sizeof(CubeModel::EBO) / sizeof(GLushort);

const char CubeModel::TEXTURE[] = "container";

void CubeModel::init(Shader *shader) {
  Model::init(shader);
  glUseProgram(shader->program);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(shader->normal);
  glVertexAttribPointer(shader->normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(shader->uv);
  glVertexAttribPointer(shader->uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
  glBindVertexArrayOES(0);
  collision = new btBoxShape(btVector3(0.500000f, 0.500000f, 0.500000f));
}

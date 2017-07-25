#include "button.hpp"

const GLfloat ButtonModel::VBO[] = {
   64.00000f,  64.00000f,     1.000000f,  0.000000f,
  -64.00000f,  64.00000f,     0.000000f,  0.000000f,
  -64.00000f, -64.00000f,     0.000000f,  1.000000f,
   64.00000f, -64.00000f,     1.000000f,  1.000000f,
};

const GLsizei ButtonModel::VBOSIZE = sizeof(ButtonModel::VBO);

const GLushort ButtonModel::EBO[] = {
  0,    1,    2,
  0,    2,    3,
};

const GLsizei ButtonModel::EBOSIZE = sizeof(ButtonModel::EBO);
const GLsizei ButtonModel::EBOCOUNT = sizeof(ButtonModel::EBO) / sizeof(GLushort);

void ButtonModel::init(Shader *shader, const char *texture) {
  Model::init(shader);
  updateTexture(texture);
  glUseProgram(shader->program);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(shader->uv);
  glVertexAttribPointer(shader->uv, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
  glBindVertexArrayOES(0);
}

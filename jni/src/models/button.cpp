#include "button.hpp"

const GLfloat ButtonModel::VBO[] = {
  /* X       Y         U      V */
  -64.0f, -64.0f,     0.0f,  1.0f,
   64.0f, -64.0f,     1.0f,  1.0f,
   64.0f,  64.0f,     1.0f,  0.0f,
  -64.0f,  64.0f,     0.0f,  0.0f,
};

const GLsizei ButtonModel::VBOSIZE = sizeof(ButtonModel::VBO);

const GLushort ButtonModel::EBO[] = {
  0, 1, 2,      2, 3, 0,
};

const GLsizei ButtonModel::EBOSIZE = sizeof(ButtonModel::EBO);
const GLsizei ButtonModel::EBOCOUNT = sizeof(ButtonModel::EBO) / sizeof(GLushort);

void ButtonModel::init(Shader *shader, const char *texture) {
  textureFilename = texture;
  Model::init(shader);
  glUseProgram(shader->program);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(shader->uv);
  glVertexAttribPointer(shader->uv, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
  glBindVertexArrayOES(0);
}

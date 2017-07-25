#include "menu.hpp"

const GLfloat MenuModel::itemSize = 100.0f;

void MenuModel::init(Shader *shader, const char *texture, const int numItems) {
  this->numItems = numItems;
  activeUniform = glGetUniformLocation(shader->program, "active");
  const GLushort planeIndices[] = {
    0,    1,    2,
    0,    2,    3,
  };
  const GLfloat uvWidth = 1.000000f / (float) numItems;
  const int numVertices = numItems * 5 * 4;
  const int numIndices = numItems * 6;
  GLfloat *VBO = new GLfloat[numVertices];
  GLushort *EBO = new GLushort[numIndices];
  for (int item = 0; item < numItems; item += 1) {
    const GLfloat x = (float) item * itemSize;
    const GLfloat uvX = (float) item * uvWidth;
    const GLfloat plane[] = {
      x + itemSize,   itemSize,      uvX + uvWidth,   0.000000f,     (float) item,
      x,              itemSize,      uvX,             0.000000f,     (float) item,
      x,              0.000000f,     uvX,             1.000000f,     (float) item,
      x + itemSize,   0.000000f,     uvX + uvWidth,   1.000000f,     (float) item,
    };
    const int offset = item * 5 * 4;
    for (int i = 0; i < 5 * 4; i += 1) {
      VBO[offset + i] = plane[i];
    }
    const int planeIndex = item * 6;
    const int planeOffset = item * 4;
    for (int i = 0; i < 6; i += 1) {
      EBO[planeIndex + i] = planeOffset + planeIndices[i];
    }
  };
  vboData = VBO;
  vboSize = numVertices * sizeof(GLfloat);
  eboData = EBO;
  eboSize = numIndices * sizeof(GLushort);
  eboCount = numIndices;
  Model::init(shader);
  delete [] VBO;
  vboData = NULL;
  delete [] EBO;
  eboData = NULL;
  updateTexture(texture);
  glUseProgram(shader->program);
  glEnableVertexAttribArray(shader->position);
  glVertexAttribPointer(shader->position, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(shader->uv);
  glVertexAttribPointer(shader->uv, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
  GLint itemAttribute = glGetAttribLocation(shader->program, "item");
  glEnableVertexAttribArray(itemAttribute);
  glVertexAttribPointer(itemAttribute, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (4 * sizeof(GLfloat)));
  glBindVertexArrayOES(0);
}

const GLint MenuModel::getActiveUniform() {
  return activeUniform;
}

const int MenuModel::getNumItems() {
  return numItems;
}

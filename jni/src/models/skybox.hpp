#pragma once

#include "model.hpp"

class SkyboxModel : public Model {
  public:
    SkyboxModel() : Model(VBO, VBOSIZE, EBO, EBOSIZE, EBOCOUNT, TEXTURE, GL_TEXTURE_CUBE_MAP) {}
    void init(Shader *shader);
  private:
    static const GLfloat VBO[];
    static const GLsizei VBOSIZE;
    static const GLushort EBO[];
    static const GLsizei EBOSIZE;
    static const GLsizei EBOCOUNT;
    static const char TEXTURE[];
};

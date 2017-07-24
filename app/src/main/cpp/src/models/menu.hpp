#pragma once

#include "model.hpp"
#include <glm.hpp>

class MenuModel : public Model {
  public:
    MenuModel() : Model(NULL, 0, NULL, 0, 0, NULL, GL_TEXTURE_2D, GL_RGBA, GL_CLAMP_TO_EDGE) {}
    void init(Shader *shader, const char *texture, const int numItems);
    const GLint getActiveUniform();
    const int getNumItems();
    static const GLfloat itemSize;
  private:
    int numItems;
    GLint activeUniform;
};

#pragma once

#include "mesh.hpp"

class Skybox : public Mesh {
  public:
    void init(Model *model);
    void render(const Camera *camera);
};

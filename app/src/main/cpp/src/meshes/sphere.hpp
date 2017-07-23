#pragma once

#include "mesh.hpp"

class Sphere : public Mesh {
  public:
    void init(btDiscreteDynamicsWorld *world, Model *model, const btVector3 position);
    void render(const Camera *camera);
  private:
    glm::vec4 albedo;
};

#include "skybox.hpp"

void Skybox::init(Model *model) {
  Mesh::init(NULL, model);
}

void Skybox::render(const Camera *camera) {
  setPosition(btVector3(camera->position[0], camera->position[1], camera->position[2]));
  Mesh::render(camera);
}

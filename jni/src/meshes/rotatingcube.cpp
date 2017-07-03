#include "rotatingcube.hpp"

void RotatingCube::init(Model *model, const int radius) {
  Mesh::init(model, glm::vec3(
    ((float) rand() / (float) RAND_MAX) - 0.5f,
    ((float) rand() / (float) RAND_MAX) - 0.5f,
    ((float) rand() / (float) RAND_MAX) - 0.5f
  ) * glm::vec3(
    (float) (rand() % radius) + 8.0f,
    (float) (rand() % radius) + 8.0f,
    (float) (rand() % radius) + 8.0f
  ));
  this->rotationAxis = glm::vec3(
    ((float) rand() / (float) RAND_MAX) - 0.5f,
    ((float) rand() / (float) RAND_MAX) - 0.5f,
    ((float) rand() / (float) RAND_MAX) - 0.5f
  );
  this->rotationSpeed = 1.0f + ((float) (rand() % 256));
}

void RotatingCube::animate() {
  setRotation(glm::angleAxis(sin((float) SDL_GetTicks() / (512.0f - rotationSpeed)), rotationAxis));
}

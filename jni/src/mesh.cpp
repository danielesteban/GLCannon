#include "mesh.hpp"

void Mesh::init(Model *model, const glm::vec3 position) {
  this->model = model;
  this->position = position;
  updateTransform();
}

void Mesh::render(const Camera *camera) {
  model->render(glm::value_ptr(camera->view * transform));
}

void Mesh::setPosition(const glm::vec3 position) {
  this->position = position;
  updateTransform();
}

void Mesh::setRotation(const glm::quat rotation) {
  this->rotation = rotation;
  updateTransform();
}

void Mesh::updateTransform() {
  transform = glm::translate(glm::mat4(), position) * glm::toMat4(rotation);
}

void Mesh::animate() {}

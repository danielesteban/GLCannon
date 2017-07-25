#include "scene.hpp"

void Scene::unload() {
  for (std::vector<Mesh>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
    (*mesh).destroy();
  }
  meshes.clear();
}

void Scene::simulate(const btScalar delta) {
  for (std::vector<Mesh>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
    (*mesh).simulate(delta);
  }
}

void Scene::render(const Camera *camera) {
  GLuint lastProgram = 0;
  for (std::vector<Mesh>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
    GLuint program = mesh->getModel()->shader->program;
    if (lastProgram != program) {
      glUseProgram(program);
      lastProgram = program;
    }
    (*mesh).render(camera);
  }
}

void Scene::reset() {
  for (std::vector<Mesh>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
    (*mesh).reset();
  }
}

const GLfloat* Scene::getFogColor() {
  return glm::value_ptr(fogColor);
}

const GLfloat* Scene::getSunPosition() {
  return glm::value_ptr(sunPosition);
}

const bool Scene::isResetting() {
  return resetting;
}

#include "main.hpp"

void MainScene::load(btDiscreteDynamicsWorld *world, Camera *camera, Model *groundModel, Model *skyboxModel, Model *cubeModel) {
  camera->position = glm::vec3(0.0f, 1.62f, 0.0f);
  camera->tilt = glm::radians(90.0f);
  camera->pitch = glm::radians(8.0f);
  camera->updateView();

  fogColor = glm::vec4(0.8, 0.658, 0.407, 1.0);
  sunPosition = glm::vec3(-0.1, 0.9, -1.0);

  groundModel->updateTexture("sand");
  skyboxModel->updateTexture("skybox");
  cubeModel->updateTexture("container");

  Mesh ground;
  ground.init(world, groundModel, btVector3(0.0f, -1.0f, 0.0f));
  meshes.push_back(ground);
  for (int x = -3; x < 4; x += 1)
  for (int y = 0; y < 5; y += 1) {
    Mesh cube;
    const float offset = (float) x * 1.12f;
    const btVector3 position = btVector3(offset, (float) y + 0.5f, 8.0f - std::exp(std::abs((float) x)) * 0.18f);
    const btQuaternion rotation = btQuaternion(
      btVector3(0.0f, 1.0f, 0.0f),
      glm::radians(offset * 14.0f)
    );
    cube.init(world, cubeModel, position, rotation, btScalar(5.0f));
    meshes.push_back(cube);
    cubes.push_back(cube);
  }
}

void MainScene::simulate(const btScalar delta) {
  if (isResetting()) {
    explosionTimer -= delta;
    if (explosionTimer <= 0.0f) {
      Scene::reset();
      resetting = false;
    }
  }
  Scene::simulate(delta);
}

void MainScene::reset() {
  if (isResetting()) return;
  resetting = true;
  explosionTimer = 3.5f;
  for (std::vector<Mesh>::iterator cube = cubes.begin(); cube != cubes.end(); ++cube) {
    const btVector3 position = (*cube).getPosition();
    const float force = 80.0f + (float) ((rand() % 101) - 50);
    (*cube).applyImpulse(btVector3(position.x() * 6.0f, std::max(std::abs(position.x()), std::abs(position.z())) * 2.0f, position.z() * 2.0f).normalize() * force);
  }
}

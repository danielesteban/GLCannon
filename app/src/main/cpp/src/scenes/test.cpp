#include "test.hpp"

void TestScene::load(btDiscreteDynamicsWorld *world, Camera *camera, Model *groundModel, Model *skyboxModel, Model *cubeModel) {
  camera->position = glm::vec3(0.0f, 11.62f, 0.0f);
  camera->tilt = glm::radians(90.0f);
  camera->pitch = glm::radians(-30.0f);
  camera->updateView();

  fogColor = glm::vec4(0.0, 0.0, 0.0, 1.0);
  sunPosition = glm::vec3(-0.1, 0.9, -1.0);

  groundModel->updateTexture("snow");
  skyboxModel->updateTexture("starfield");
  cubeModel->updateTexture("sand");

  Mesh ground;
  ground.init(world, groundModel, btVector3(0.0f, -1.0f, 0.0f));
  meshes.push_back(ground);
  for (int x = -3; x < 4; x += 3)
  for (int y = 0; y < 8; y += 1) {
    Mesh cube;
    const float offset = 0.0f;
    const btVector3 position = btVector3((float) x, (float) y * 2.0f, 8.0f);
    cube.init(world, cubeModel, position, btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btScalar(10.0f));
    meshes.push_back(cube);
  }
}

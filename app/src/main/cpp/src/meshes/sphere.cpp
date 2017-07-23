#include "sphere.hpp"

void Sphere::init(btDiscreteDynamicsWorld *world, Model *model, const btVector3 position) {
  Mesh::init(world, model, position, btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btScalar(5.0f));
  albedo = glm::vec4(
    (float) rand() / (float) RAND_MAX,
    (float) rand() / (float) RAND_MAX,
    (float) rand() / (float) RAND_MAX,
    1.0f
  );
}

void Sphere::render(const Camera *camera) {
  glUniform4fv(model->shader->albedo, 1, glm::value_ptr(albedo));
  Mesh::render(camera);
}

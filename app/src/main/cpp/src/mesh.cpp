#include "mesh.hpp"

void Mesh::init(
  btDiscreteDynamicsWorld *world,
  Model *model,
  const btVector3 position,
  const btQuaternion rotation,
  const btScalar mass
) {
  this->model = model;
  this->world = world;
  transform.setIdentity();
  transform.setOrigin(position);
  transform.setRotation(rotation);
  initialTransform = btTransform(transform);
  if (model->collision != NULL) {
    bool isDynamic = (mass != 0.f);
    btVector3 localInertia(0, 0, 0);
    if (isDynamic) {
      model->collision->calculateLocalInertia(mass, localInertia);
    }
    btDefaultMotionState *motionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, model->collision, localInertia);
		body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
  } else {
    body = NULL;
  }
  updateView();
}

void Mesh::destroy() {
  if (body != NULL) {
    world->removeRigidBody(body);
    delete body->getMotionState();
    delete body;
  }
}

void Mesh::render(const Camera *camera) {
  model->render(glm::value_ptr(camera ? (camera->view * view) : view), glm::value_ptr(normalView));
}

void Mesh::reset() {
  transform = btTransform(initialTransform);
  if (body != NULL) {
    body->clearForces();
    body->setLinearVelocity(btVector3(0, 0, 0));
    body->setAngularVelocity(btVector3(0, 0, 0));
    body->setWorldTransform(transform);
    body->activate();
  }
  updateView();
}

const btVector3 Mesh::getPosition() {
  return transform.getOrigin();
}
const btQuaternion Mesh::getRotation() {
  return transform.getRotation();
}

void Mesh::setPosition(const btVector3 position) {
  transform.setOrigin(position);
  if (body != NULL) {
    body->setWorldTransform(transform);
    body->activate();
  }
  updateView();
}

void Mesh::setRotation(const btQuaternion rotation) {
  transform.setRotation(rotation);
  if (body != NULL) {
    body->setWorldTransform(transform);
    body->activate();
  }
  updateView();
}

void Mesh::applyImpulse(const btVector3 impulse) {
  if (body == NULL) return;
  body->activate(true);
  body->applyCentralImpulse(impulse);
}

const Model* Mesh::getModel() {
  return model;
}

void Mesh::updateView() {
  transform.getOpenGLMatrix(glm::value_ptr(view));
  normalView = glm::inverseTranspose(glm::mat3(view));
}

void Mesh::simulate(const btScalar delta) {
  if (body != NULL && body->isActive()) {
    body->getMotionState()->getWorldTransform(transform);
    updateView();
  }
}

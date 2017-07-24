#include "button.hpp"

void Button::init(Model *model, const btVector3 position) {
  Mesh::init(NULL, model, position);
  albedo = glm::vec4(0.0f, 0.0f, 0.0f, 0.9f);
}

bool Button::isHover(const float x, const float y) {
  const btVector3 position = getPosition();
  if (x < position.getX() - 64.0f) return false;
  if (x > position.getX() + 64.0f) return false;
  if (y < position.getY() - 64.0f) return false;
  if (y > position.getY() + 64.0f) return false;
  return true;
}

void Button::render(const Camera *camera) {
  glUniform4fv(model->shader->albedo, 1, glm::value_ptr(albedo));
  Mesh::render(NULL);
}

void Button::simulate(const btScalar delta) {
  const btScalar step = 2.0f * delta;
  if (!firing) return;
  if (raising) {
    force += step;
    if (force > 1.25f) {
      force = 1.25f;
      raising = false;
    }
  } else {
    force -= step;
    if (force < 0.0f) {
      force = 0.0f;
      raising = true;
    }
  }
  albedo[0] = fmin(force, 1.0f);
}

bool Button::isFiring() {
  return firing;
}

void Button::setFiring(const bool state) {
  firing = state;
  raising = true;
  force = 0.0f;
  albedo[0] = 0.0f;
}

btScalar Button::getForce() {
  return fmin(force, 1.0f);
}

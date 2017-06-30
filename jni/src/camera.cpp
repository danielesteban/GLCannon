#include "camera.hpp"

const glm::vec3 Camera::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() {
  tilt = glm::radians(270.0f);
  pitch = glm::radians(-30.0f);
  position[1] = 3.0f;
  position[2] = 3.0f;
}

void Camera::resize(int w, int h) {
  projection = glm::perspective(glm::radians(60.0f), (float) w / (float) h, 0.1f, 512.0f);
  updateTransform();
}

void Camera::updateTransform() {
  front[0] = cos(tilt) * cos(pitch);
  front[1] = sin(pitch);
  front[2] = sin(tilt) * cos(pitch);
  front = glm::normalize(front);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
  transform = projection * glm::lookAt(
    position,
    position + front,
    up
  );
}

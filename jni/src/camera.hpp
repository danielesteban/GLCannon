#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include <SDL_opengles2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
  public:
    Camera();
    void resize(int w, int h);
    void updateVectors();
    void updateTransform();
    glm::vec3 position;
    glm::mat4 projection;
    glm::mat4 transform;
    float tilt;
    float pitch;
  private:
    static const glm::vec3 worldUp;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
};

#endif

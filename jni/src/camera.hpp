#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

class Camera {
  public:
    Camera();
    void processInput(float dx, float dy);
    void resize(int w, int h);
    void updateVectors();
    void updateView();
    glm::vec3 position;
    glm::mat4 projection;
    glm::mat4 view;
    float tilt;
    float pitch;
  private:
    static const glm::vec3 worldUp;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
};

#endif

#ifndef MESH_H
#define MESH_H

#include <SDL.h>
#include <SDL_image.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <vector>
#include "camera.hpp"
#include "shader.hpp"

class Mesh {
  public:
    Mesh(const GLfloat vbo[], const GLushort ebo[], const GLsizei size, const GLsizei count, const char texture[]);
    void init(Shader *shader, const glm::vec3 *positions, const int positionsSize);
    void render(Camera *camera, Shader *shader);
  protected:
    std::vector<glm::mat4> initialTransforms;
    std::vector<glm::mat4> transforms;
  private:
    const GLfloat *modelVBO;
    const GLushort *modelEBO;
    const GLsizei modelSize;
    const GLsizei modelCount;
    const char *modelTexture;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint texture;
};

#endif

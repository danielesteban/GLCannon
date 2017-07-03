#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

#include "camera.hpp"
#include "cube.hpp"
#include "shader.hpp"

SDL_Window* gWindow = NULL;
SDL_GLContext gContext;

Camera camera;
Cube cube;
Shader shader;

void resize();
void init() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 6);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  gWindow = SDL_CreateWindow("GLTest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL);
  gContext = SDL_GL_CreateContext(gWindow);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  SDL_GL_SetSwapInterval(1);
  shader.init();
  resize();
}

void resize() {
  int w, h;
  SDL_GL_GetDrawableSize(gWindow, &w, &h);
  glViewport(0, 0, w, h);
  camera.resize(w, h);
  glUseProgram(shader.program);
  glUniformMatrix4fv(shader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
}

void processTouch(int finger, float x, float y, float dx, float dy) {
  if (finger > 0) return;
  // glClearColor(x, 0.0f, y, 1.0f);
  camera.processInput(dx, dy);
  // SDL_Log("%d %d", (int) (x * 100.0f), (int) (y * 100.0f));
}

void setupScene() {
  srand(time(NULL));
  const int positionsSize = 512;
  glm::vec3 positions[positionsSize];
  for (int i = 0; i < positionsSize; i += 1) {
    positions[i] = glm::vec3(
      ((float) rand() / (float) RAND_MAX) - 0.5f,
      ((float) rand() / (float) RAND_MAX) - 0.5f,
      ((float) rand() / (float) RAND_MAX) - 0.5f
    ) * glm::vec3(
      (float) (rand() % 120) + 8.0f,
      (float) (rand() % 120) + 8.0f,
      (float) (rand() % 120) + 8.0f
    );
  }
  cube.init(&shader, positions, positionsSize);
}

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shader.program);
  cube.render(&camera, &shader);
  SDL_GL_SwapWindow(gWindow);
}

void loop() {
  SDL_bool quit = SDL_FALSE;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT:
          quit = SDL_TRUE;
          break;
        case SDL_KEYDOWN:
          {
            if (!e.key.repeat && e.key.keysym.sym == SDLK_AC_BACK) {
              SDL_Quit();
              return;
            }
          }
          break;
        case SDL_WINDOWEVENT_RESIZED:
          resize();
          break;
        case SDL_FINGERDOWN:
        case SDL_FINGERMOTION:
          processTouch(e.tfinger.fingerId, e.tfinger.x, e.tfinger.y, e.tfinger.dx, e.tfinger.dy);
          break;
      }
    }
    renderScene();
  }
}

int main(int argc, char* args[]) {
  init();
  setupScene();
  loop();
  return 0;
}

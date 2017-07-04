#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <imgui.h>
#include <imgui_impl_sdl_gles.h>

#include "camera.hpp"
#include "mesh.hpp"
#include "shaders/skybox.hpp"
#include "shaders/standard.hpp"
#include "models/cube.hpp"
#include "models/skybox.hpp"
#include "meshes/rotatingcube.hpp"

#define FSAA 2
#define CUBE_COUNT 512

SDL_Window *gWindow = NULL;
SDL_GLContext gContext;

Camera camera;
Cube cubeModel;
Skybox skyboxModel;
RotatingCube cubes[CUBE_COUNT];
Mesh skybox;

StandardShader cubeShader;
SkyboxShader skyboxShader;

void resize();
void init() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, FSAA);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  gWindow = SDL_CreateWindow("GLTest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL);
  gContext = SDL_GL_CreateContext(gWindow);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  SDL_GL_SetSwapInterval(1);
  cubeShader.init();
  skyboxShader.init();
  resize();
  ImGui_ImplSdlGLES_Init(gWindow);
}

void resize() {
  int w, h;
  SDL_GL_GetDrawableSize(gWindow, &w, &h);
  glViewport(0, 0, w, h);
  camera.resize(w, h);
  glUseProgram(cubeShader.program);
  glUniformMatrix4fv(cubeShader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
  glUseProgram(skyboxShader.program);
  glUniformMatrix4fv(skyboxShader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
}

void processTouch(int finger, float dx, float dy) {
  if (finger > 0) return;
  camera.processInput(dx, dy);
}

void setupScene() {
  srand(time(NULL));
  cubeModel.init(&cubeShader);
  for (int i = 0; i < CUBE_COUNT; i += 1) {
    cubes[i].init(&cubeModel, 120);
  }
  skyboxModel.init(&skyboxShader);
  skybox.init(&skyboxModel, camera.position);
}

void renderScene() {
  glUseProgram(cubeShader.program);
  for (int i = 0; i < CUBE_COUNT; i += 1) {
    cubes[i].animate();
    cubes[i].render(&camera);
  }
  glUseProgram(skyboxShader.program);
  glDepthFunc(GL_LEQUAL);
  skybox.render(&camera);
  glDepthFunc(GL_LESS);
}

void renderUI() {
  ImGui_ImplSdlGLES_NewFrame(gWindow);
  bool open = true;
  ImGui::Begin("UI", &open, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
  ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
  ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
  ImGui::Text("%dx%d FSAA x%d", (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y, FSAA);
  ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::GetWindowWidth() - 9, 8));
  ImGui::End();
  ImGui::Render();
}

void loop() {
  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      ImGui_ImplSdlGLES_ProcessEvent(&e);
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          return;
        case SDL_WINDOWEVENT_RESIZED:
          resize();
          break;
        case SDL_FINGERDOWN:
        case SDL_FINGERMOTION:
          processTouch(e.tfinger.fingerId, e.tfinger.dx, e.tfinger.dy);
          break;
      }
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene();
    renderUI();
    SDL_GL_SwapWindow(gWindow);
  }
}

int main(int argc, char *args[]) {
  init();
  setupScene();
  loop();
  return 0;
}

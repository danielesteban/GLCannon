#include "shaders/skybox.hpp"
#include "shaders/standard.hpp"
#include "models/cube.hpp"
#include "models/ground.hpp"
#include "models/skybox.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "fontawesome.hpp"
#include <imgui.h>
#include <imgui_impl_sdl_gles.h>
#include <vector>

#define FSAA 2

Camera camera;
Cube cubeModel;
Ground groundModel;
Skybox skyboxModel;
std::vector<Mesh> cubes;
Mesh ground;
Mesh skybox;

StandardShader standardShader;
SkyboxShader skyboxShader;

btDefaultCollisionConfiguration *collisionConfiguration;
btCollisionDispatcher *dispatcher;
btBroadphaseInterface *overlappingPairCache;
btSequentialImpulseConstraintSolver *solver;
btDiscreteDynamicsWorld *world;

SDL_Window *gWindow = NULL;
SDL_GLContext gContext;

int width;
int height;

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
  glClearColor(0.04f, 0.08f, 0.14f, 1.0f);
  SDL_GL_SetSwapInterval(1);
  standardShader.init();
  skyboxShader.init();
  resize();

  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  overlappingPairCache = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver;
  world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
  world->setGravity(btVector3(0.0f, -9.8f, 0.0f));

  ImGui_ImplSdlGLES_Init(gWindow);
  ImGuiIO& io = ImGui::GetIO();

  io.Fonts->TexDesiredWidth = 2048;
  SDL_RWops *file = SDL_RWFromFile("roboto.ttf", "rb");
  int size = SDL_RWsize(file);
  Uint8 *roboto = new Uint8[size];
  SDL_RWread(file, roboto, size, 1);
  SDL_RWclose(file);
  io.Fonts->AddFontFromMemoryTTF(roboto, size, 32.0f);

  ImFontConfig config;
  config.MergeMode = true;
  config.PixelSnapH = true;
  const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
  file = SDL_RWFromFile("fa.ttf", "rb");
  size = SDL_RWsize(file);
  Uint8 *fa = new Uint8[size];
  SDL_RWread(file, fa, size, 1);
  SDL_RWclose(file);
  io.Fonts->AddFontFromMemoryTTF(fa, size, 64.0f, &config, icon_ranges);
  ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.68f);
  ImGui::GetStyle().Colors[ImGuiCol_Button] = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.6f);
  ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
}

void resize() {
  SDL_GL_GetDrawableSize(gWindow, &width, &height);
  glViewport(0, 0, width, height);
  camera.resize(width, height);
  glUseProgram(standardShader.program);
  glUniformMatrix4fv(standardShader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
  glUseProgram(skyboxShader.program);
  glUniformMatrix4fv(skyboxShader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
}

void processTouch(int finger, float dx, float dy) {
  if (finger > 0) return;
  camera.processInput(dx, dy);
}

void setupScene() {
  srand(time(NULL));
  cubeModel.init(&standardShader);
  groundModel.init(&standardShader);
  ground.init(world, &groundModel, btVector3(0.0f, -1.0f, 0.0f));
  skyboxModel.init(&skyboxShader);
  skybox.init(world, &skyboxModel, btVector3(camera.position[0], camera.position[1], camera.position[2]));
}

void animateScene(const btScalar delta) {
  world->stepSimulation(delta, 4);
  for (std::vector<Mesh>::iterator cube = cubes.begin(); cube != cubes.end(); ++cube) {
    (*cube).animate(delta);
  }
}

void renderScene() {
  glUseProgram(standardShader.program);
  ground.render(&camera);
  for (std::vector<Mesh>::iterator cube = cubes.begin(); cube != cubes.end(); ++cube) {
    (*cube).render(&camera);
  }
  glUseProgram(skyboxShader.program);
  glDepthFunc(GL_LEQUAL);
  skybox.render(&camera);
  glDepthFunc(GL_LESS);
}

void renderUI() {
  ImGui_ImplSdlGLES_NewFrame(gWindow);
  bool open = true;
  ImGui::Begin("DEBUG", &open, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
  ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
  ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
  ImGui::Text("%dx%d FSAA x%d", (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y, FSAA);
  ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::GetWindowWidth() - 9, 8));
  ImGui::End();

  ImGui::Begin("UI", &open, ImVec2(0, 0), 0.0, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
  if (ImGui::Button(ICON_FA_BULLSEYE, ImVec2(256.0f, 128.0f))) {
    Mesh cube;
    const glm::vec3 origin = camera.position + camera.front;
    cube.init(world, &cubeModel, btVector3(origin[0], origin[1], origin[2]), btScalar(1.0f));
    cube.setRotation(btQuaternion(btVector3(
      ((float) rand() / (float) RAND_MAX) - 0.5f,
      ((float) rand() / (float) RAND_MAX) - 0.5f,
      ((float) rand() / (float) RAND_MAX) - 0.5f
    ), btScalar(glm::radians((float) (rand() % 360)))));
    cube.applyImpulse(btVector3(camera.front[0], camera.front[1], camera.front[2]) * 5.0f);
    cubes.push_back(cube);
  }
  ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::GetWindowWidth() - 1, ImGui::GetIO().DisplaySize.y - ImGui::GetWindowHeight() - 1));
  ImGui::End();
}

void loop() {
  SDL_Event e;
  bool quit = false;
  unsigned int lastTicks = SDL_GetTicks();
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          return;
        case SDL_WINDOWEVENT_RESIZED:
          resize();
          break;
        case SDL_FINGERDOWN:
          ImGui::GetIO().MouseDown[0] = true;
          ImGui::GetIO().MousePos = ImVec2(floor(((float) width) * e.tfinger.x), floor(((float) height) * e.tfinger.y));
        case SDL_FINGERMOTION:
          processTouch(e.tfinger.fingerId, e.tfinger.dx, e.tfinger.dy);
          break;
        case SDL_FINGERUP:
          ImGui::GetIO().MouseDown[0] = false;
          break;
      }
    }
    renderUI();
    unsigned int currentTicks = SDL_GetTicks();
    const btScalar delta = btScalar(currentTicks - lastTicks) / btScalar(1000.0f);
    lastTicks = currentTicks;
    animateScene(delta);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene();
    ImGui::Render();
    SDL_GL_SwapWindow(gWindow);
  }
}

int main(int argc, char *args[]) {
  init();
  setupScene();
  loop();
  return 0;
}

#pragma once

#include "mesh.hpp"
#include "models/menu.hpp"

class Menu : public Mesh {
  public:
    void init(MenuModel *model, const btScalar y);
    bool isHover(const float x, const float y);
    void simulate(const btScalar delta);
    void render(const Camera *camera = NULL);
    const int click(const float x);
    void toggle();
    const int getActiveItem();
    void setActiveItem(const int item);
  private:
    btVector3 targetPosition;
    float width;
    bool isCollapsed;
    bool isAnimating;
    int activeItem;
};

#include "menu.hpp"

void Menu::init(MenuModel *model, const btScalar y) {
  isCollapsed = true;
  activeItem = 1;
  width = (float) model->getNumItems() * MenuModel::itemSize;
  targetPosition = btVector3((width - MenuModel::itemSize) * -1.0f, y, 0.0f);
  Mesh::init(NULL, (Model *) model, targetPosition);
}

bool Menu::isHover(const float x, const float y) {
  const btVector3 position = getPosition();
  if (x < position.x()) return false;
  if (x > position.x() + width) return false;
  if (y < position.y()) return false;
  if (y > position.y() + MenuModel::itemSize) return false;
  return true;
}

void Menu::simulate(const btScalar delta) {
  if (!isAnimating) return;
  const btScalar step = width * 4.0f * delta;
  const btVector3 position = getPosition();
  btScalar x;
  if (isCollapsed) {
    x = fmax(position.x() - step, targetPosition.x());
  } else {
    x = fmin(position.x() + step, targetPosition.x());
  }
  setPosition(btVector3(
    x,
    position.y(),
    position.z()
  ));
  if (x == targetPosition.x()) {
    isAnimating = false;
  }
}

void Menu::render(const Camera *camera) {
  glUniform1f(((MenuModel *) model)->getActiveUniform(), (float) activeItem);
  Mesh::render(NULL);
}

const int Menu::click(const float x) {
  const btVector3 position = getPosition();
  const int numItems = ((MenuModel *) model)->getNumItems();
  const int item = (int) ((x - position.x()) / MenuModel::itemSize);
  if (item < 0 || item > numItems) return -1;
  switch (item) {
    case 3:
    case 4:
      break;
    case 5:
      toggle();
      break;
    default:
      setActiveItem(item);
  }
  return item;
}

void Menu::toggle() {
  isCollapsed = !isCollapsed;
  isAnimating = true;
  if (isCollapsed) {
    targetPosition.setX((width - MenuModel::itemSize) * -1.0f);
  } else {
    targetPosition.setX(0.0f);
  }
}

const int Menu::getActiveItem() {
  return activeItem;
}

void Menu::setActiveItem(const int item) {
  activeItem = item;
}

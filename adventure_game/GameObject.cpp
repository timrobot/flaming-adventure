#include "GameObject.h"

Chest::Chest() {
  image = Environment::getImage("chest_closed.png");
  opened = false;
}

Chest::~Chest() {
  
}

void Chest::update(Event_t e, Environment *env) {
  switch (e.type) {
    case ET_KEY:
      switch (e.key) {
        case EK_SPACE:
          open();
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

void Chest::open() {
  if (!opened) {
    opened = true;
    image = Environment::getImage("chest_opened.png");
  }
}

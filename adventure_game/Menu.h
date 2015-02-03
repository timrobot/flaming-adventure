#ifndef Menu_h
#define Menu_h

#include "Common.h"

typedef struct {
  string name;
  SDL_Surface *icon;
} option_t;

class Menu {
  public:
    vector<option> options;
    void select();
    void change_option_up();
    void change_option_down();
    void deselect();
};

#endif

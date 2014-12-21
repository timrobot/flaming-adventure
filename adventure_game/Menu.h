#ifndef Menu_h
#define Menu_h

#include "Common.h"

typedef void (*void_func)(void);

struct option {
  string name;
  SDL_Surface *icon;
};

class Menu {
  public:
    vector<option> options;
    void select();
    void change_option_up();
    void change_option_down();
    void revert();
};

#endif

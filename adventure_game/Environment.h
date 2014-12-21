#ifndef Environment_h
#define Environment_h

#include "Common.h"

class Map {
  vector< vector<int32_t> > dataspace;
  SDL_Surface *asSurface;
};

class Environment {
  private:
    Map *map;
    int region;
};

#endif

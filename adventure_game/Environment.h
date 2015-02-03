#ifndef Environment_h
#define Environment_h

#include "Common.h"

namespace Environment {

  typedef struct Portal {
    int x;
    int y;
    Map *connected_map;
    struct Portal *connection;
  } Portal_t;

  class Tile {
    SDL_Surface *image;
    bool walkable;
    void update();
  };

  class Map { // 2d map
    public:
      Map();
      vector< vector<Tile> > tiles;
      vector<Portal_t> portals;
      vector<GameObject> objects;
      bool visited;
  }; // data is saved on a per map basis

  Map& map;
  vector<Map> maps;
  vector<SDL_Surface *> surfaces;
  vector<std::string> surface_names;
}

#endif

#ifndef ResourceManager_h
#define ResourceManager_cpp

#include <SDL/SDL.h>
#include <vector>

using namespace std;

namespace ResourceManager {
  
  vector<SDL_Surface *> images;
  vector<string> image_names;

  void loadResources();

  SDL_Surface *getImage(string image_name);

}

#endif

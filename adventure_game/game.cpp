#include "Common.h"
#include "Environment.h"
#include "Menu.h"
#include "SaveGame.h"

int main() {
  int screen_width = 480;
  int screen_height = 640;
  int screen_bpp = 32;
  SDL_Surface *screen;
  
  // Load Environment
  Environment *universe = new Environment();
  // Load Controller
  Controller *controls = new Controller();
  // Load UI for Menu
  Menu *menu = new Menu();
  menu->display();

  // Create screen
  screen = SDL_SetVideoMode(screen_width, screen_height, screen_bpp, SDL_SWSURFACE);
  SDL_Quit();
  return 0;
}

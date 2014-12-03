#include <SDL/SDL.h>
#include <stdint.h>

int main() {
  /* declare variables */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface *screen;
  bool running;
  uint32_t start;
  const int FPS = 30;
  SDL_Rect rect;

  /* create screen */
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  running = true;
  rect.x = 10;
  rect.y = 10;
  rect.w = 20;
  rect.h = 20;
  uint32_t black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
  uint32_t color = SDL_MapRGB(screen->format, 0x00, 0xFF, 0xFF);

  /* run loop */
  while (running) {
    SDL_Event event;
    start = SDL_GetTicks();

    /* handle events, such as quit */
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = false;
          break;
      }
    }

    /* logic and render */
    SDL_FillRect(screen, &rect, color);
    SDL_Flip(screen);
    if (1000 / FPS > SDL_GetTicks() - start)
      SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
  }
  SDL_Quit();
  return 0;
}

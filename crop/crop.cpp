#include <SDL/SDL.h>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

#define max(a, b) ((a) > (b)) ? (a) : (b)
#define min(a, b) ((a) < (b)) ? (a) : (b)

void shrink(Mat& src, Mat& dest, int scalefactor) {
  for (int x = 0; x < dest.cols; x++)
    for (int y = 0; y < dest.rows; y++)
      dest.at<Vec3b>(y, x) = src.at<Vec3b>(y * scalefactor, x * scalefactor);
}

SDL_Surface *cvtSurface(Mat& matrix) {
  SDL_Surface *r = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
      matrix.cols, matrix.rows, 32,
      0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  for (int x = 0; x < matrix.cols; x++) {
    for (int y = 0; y < matrix.rows; y++) {
      ((uint8_t *)r->pixels)[r->w * y * 4 + x * 4 + 3] = matrix.at<Vec3b>(y, x)[2];
      ((uint8_t *)r->pixels)[r->w * y * 4 + x * 4 + 2] = matrix.at<Vec3b>(y, x)[1];
      ((uint8_t *)r->pixels)[r->w * y * 4 + x * 4 + 1] = matrix.at<Vec3b>(y, x)[0];
      ((uint8_t *)r->pixels)[r->w * y * 4 + x * 4 + 0] = 0x000000FF;
    }
  }
  return r;
}

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);
  int maxres[2] = {1200, 800};
  bool onclick = false;
  
  if (argc != 3) {
    printf("Usage: %s [source] [dest]\n", argv[0]);
    exit(0);
  }

  /* get a pic */
  Mat pic = imread(argv[1]);
  int startpos[2] = {-1, -1};
  int endpos[2] = {-1, -1};
  bool selected = false;
  int res[2] = {pic.cols, pic.rows};
  int scale_factor = 1;
  while (res[0] > maxres[0] || res[1] > maxres[1]) {
    res[0] /= 2;
    res[1] /= 2;
    scale_factor *= 2;
  }
  Mat temp(res[1], res[0], CV_8UC3);
  shrink(pic, temp, scale_factor);
  SDL_Surface *temp_pic = cvtSurface(temp);
  temp.release();

  SDL_Surface *screen = SDL_SetVideoMode(res[0], res[1], 32, SDL_SWSURFACEALPHA);

  while (!selected) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          SDL_Quit();
          exit(0);
          break;
        case SDL_MOUSEBUTTONDOWN:
          startpos[0] = event.button.x;
          startpos[1] = event.button.y;
          onclick = true;
          break;
        case SDL_MOUSEBUTTONUP:
          endpos[0] = event.button.x;
          endpos[1] = event.button.y;
          onclick = false;
          selected = true;
          break;
        case SDL_MOUSEMOTION:
          endpos[0] = event.motion.x;
          endpos[1] = event.motion.y;
          break;
      }

      SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
      SDL_BlitSurface(temp_pic, NULL, screen, NULL);
      if (onclick) {
        int x1 = min(startpos[0], endpos[0]);
        int y1 = min(startpos[1], endpos[1]);
        int x2 = max(startpos[0], endpos[0]);
        int y2 = max(startpos[1], endpos[1]);
        SDL_Rect selection_box = { x: x1, y: y1, w: x2 - x1, h: y2 - y1 };
        SDL_FillRect(screen, &selection_box, SDL_MapRGBA(screen->format, 0x00, 0x00, 0xFF, 0x80));
      }
      SDL_Flip(screen);
      SDL_Delay(5); /* 1000 / 100 */
    }
  }
  SDL_FreeSurface(temp_pic);

  printf("Start saving...\n");

  int x1 = min(startpos[0], endpos[0]);
  int y1 = min(startpos[1], endpos[1]);
  int x2 = max(startpos[0], endpos[0]);
  int y2 = max(startpos[1], endpos[1]);
  int w = x2 - x1;
  int h = y2 - y1;
  printf("w:%d h:%d x:%d y:%d\n", w, h, x1, y1);
  x1 = min(startpos[0], endpos[0]) * scale_factor;
  y1 = min(startpos[1], endpos[1]) * scale_factor;
  x2 = max(startpos[0], endpos[0]) * scale_factor;
  y2 = max(startpos[1], endpos[1]) * scale_factor;
  w = x2 - x1;
  h = y2 - y1;
  printf("w:%d h:%d x:%d y:%d\n", w, h, x1, y1);

  /* convert the pic to a selected pic */
  Mat new_pic(pic, Rect(x1, y1, x2, y2));
  imwrite(argv[2], new_pic);
  
  SDL_Quit();
  return 0;
}

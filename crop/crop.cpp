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

void subsurface(Mat& src, Mat& dest, int x, int y) {
  for (int i = 0; i < dest.cols; i++)
    for (int j = 0; j < dest.rows; j++)
      dest.at<Vec3b>(j, i) = src.at<Vec3b>(j + y, i + x);
}

SDL_Surface *cvtSurface(Mat& matrix) {
  printf("Entered blit function\n");
  SDL_Surface *r = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
      matrix.cols, matrix.rows, 32,
      0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  for (int x = 0; x < matrix.cols; x++) {
    for (int y = 0; y < matrix.rows; y++) {
      ((uint8_t *)r->pixels)[r->w * y * 4 + x * 4 + 0] = matrix.at<Vec3b>(y, x)[2];
      ((uint8_t *)r->pixels)[r->w * y * 4 + x * 4 + 1] = matrix.at<Vec3b>(y, x)[1];
      ((uint8_t *)r->pixels)[r->w * y * 4 + x * 4 + 2] = matrix.at<Vec3b>(y, x)[0];
      ((uint8_t *)r->pixels)[r->w * y * 4 + x * 4 + 3] = 0x000000FF;
    }
  }
  return r;
}

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);
  int maxres[2] = {1200, 800};
  
  if (argc != 3) {
    printf("Usage: %s [source] [dest]\n", argv[0]);
    return 0;
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

  Mat temp_pic(res[0], res[1], CV_8UC3);
  shrink(pic, temp_pic, scale_factor);
  SDL_Surface *screen = SDL_SetVideoMode(res[0], res[1], 32, SDL_SWSURFACE);
  SDL_Surface *temp_surf = cvtSurface(temp_pic);

  printf("start the selection!\n");

  while (!selected) {
    SDL_Event event;
    bool onclick = false;
    uint32_t start = SDL_GetTicks();
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

      printf("draw!\n");
      SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

      printf("disp image!\n");
      SDL_Rect targetrect = { x: 0, y: 0, w: temp_surf->w, h: temp_surf->h };
      SDL_BlitSurface(screen, &targetrect, temp_surf, &targetrect);

      printf("disp sbox!\n");
      if (onclick) {
        int x1 = min(startpos[0], endpos[0]);
        int y1 = min(startpos[1], endpos[1]);
        int x2 = max(startpos[0], endpos[0]);
        int y2 = max(startpos[1], endpos[1]);
        SDL_Rect selection_box = { x: x1, y: y1, w: x2 - x1, h: y2 - y1 };
        SDL_FillRect(screen, &selection_box, SDL_MapRGBA(screen->format, 0x00, 0x00, 0xFF, 0x80));
      }

      printf("show and move on!\n");
      SDL_Flip(screen);
      uint32_t waittime = 1000 / 40. - (SDL_GetTicks() - start);
      if (waittime > 0)
        SDL_Delay(waittime);
    }
  }
  temp_pic.release();

  int x1 = min(startpos[0], endpos[0]) * scale_factor;
  int y1 = min(startpos[1], endpos[1]) * scale_factor;
  int x2 = max(startpos[0], endpos[0]) * scale_factor;
  int y2 = max(startpos[1], endpos[1]) * scale_factor;
  int w = x2 - x1;
  int h = y2 - y1;

  /* convert the pic to a selected pic */
  Mat new_pic(w, h, CV_8UC3);
  subsurface(pic, new_pic, x1, y1);
  imwrite(argv[2], new_pic);
  
  SDL_Quit();
  return 0;
}

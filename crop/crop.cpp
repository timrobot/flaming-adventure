#include <SDL/SDL.h>
#include <vector>
#include <stdint.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);
  int maxres[2] = {1200, 800};
  bool onclick = false;
  
  if (argc != 3) {
    printf("Usage: %s [source] [dest]".format(argv[0]));
    return 0;
  }

  /* get an image */
  cv::Mat image = imread(argv[1]);
  int startpos[2] = {-1, -1};
  int endpos[2] = {-1, -1};
  bool selected = false;
  int res[2] = {image.cols(), image.rows()};
  int scale_factor = 1;
  while (res[0] > maxres[0] || res[1] > maxres[1]) {
    res[0] /= 2;
    res[1] /= 2;
    scale_factor *= 2;
  }

  cv::Mat temp_image(res[0], res[1], CV_8UC3);
  cv::resize(image, temp_image, 0, 0, 0, cv::INTER_CUBIC);

  /* convert the image to a selected image */
  

  return 0;
}

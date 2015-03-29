#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef union note_ {
  struct {
    char bit8_l;
    char bit8_h;
  };
  int16_t bit16;
} note;

int16_t filter(int16_t input, int16_t threshold) {
  if (input < threshold && input > -threshold) {
    return 0;
  } else {
    return input;
  }
}

void writeRawFile(note *notes, long size, char *fname) {
  FILE *fp = fopen(fname, "w");
  int i;
  for (i = 0; i < (int)size; i++) {
    fputc(notes[i].bit8_l, fp);
    fputc(notes[i].bit8_h, fp);
  }
  fclose(fp);
}

// careful of non 1byte chars!
int main(int argc, char *argv[]) {

  if (argc != 4) {
    printf("usage: %s [filename] [start] [end]\n", argv[0]);
    return -1;
  }

  FILE *fp = fopen(argv[1], "r");
  long start = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  long end = ftell(fp);
  printf("size: %ld\n", (end - start) / 2L);
  long size = (end - start) / 2L;
  note *notes = (note *)malloc(sizeof(note) * size);
  fseek(fp, 0L, SEEK_SET);
  int max = 0;

  int i;
  for (i = 0; i < (int)size; i++) {
    notes[i].bit8_l = fgetc(fp);
    notes[i].bit8_h = fgetc(fp);
    int16_t threshold = 512;
    int16_t possible = filter(notes[i].bit16, threshold);
    if (possible == 0 &&
      ((i == 0 || (i > 0 && filter(notes[i - 1].bit16, threshold) != 0)) ||
      (i == size - 1 || (i < size - 1 && filter(notes[i + 1].bit16, threshold) != 0)))) {
      possible = notes[i].bit16;
    }
    notes[i].bit16 = possible;
    if (notes[i].bit16 > max) {
      max = notes[i].bit16;
    }
  }

  int startind = atoi(argv[2]);
  int endind = atoi(argv[3]);

  if (startind == -1) {
    startind = 0;
  }
  if (endind == -1) {
    endind = size;
  }

  printf("yo\n");
  for (i = startind; i < endind; i++) {
    printf("%d\n", notes[i].bit16);
  }
  printf("oy\n");

  char newfname[64];
  sprintf(newfname, "new%s", argv[1]);

  printf("writing\n");
  writeRawFile(&notes[startind], endind - startind, newfname);
  printf("done writing\n");
  printf("MAX: %d\n", max);

  fclose(fp);

  return 0;
}

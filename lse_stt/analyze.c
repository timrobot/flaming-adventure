#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef union {
  struct {
    char low;
    char high;
  };
  int16_t val;
} note;

int16_t *loadFile(char *fname, long *s) {
  FILE *fp = fopen(fname, "r");
  long start = ftell(fp);
  if (fp == NULL) return NULL;
  fseek(fp, 0L, SEEK_END);
  long end = ftell(fp);
  long size = (end - start) / 2L;
  if (size == 0) {
    return NULL;
  }
  int16_t *notes = malloc(sizeof(int16_t) * size);
  fseek(fp, 0L, SEEK_SET);
  int i = 0;
  for (i = 0; i < size; i++) {
    note n;
    n.low = fgetc(fp);
    n.high = fgetc(fp);
    notes[i] = n.val;
  }
  *s = size;
  return notes;
}

unsigned long long square_error(int16_t *haystack, int hlen, int16_t *needle, int nlen, int *start) {
  unsigned long long sqerr = ((unsigned long long)-1);
  int index = -1;
  int i, j;
  for (i = 0; i < hlen - nlen; i++) {
    unsigned long long interim = 0L;
    for (j = 0; j < nlen; j++) {
      unsigned long long err = haystack[i + j] - needle[i];
      err = err * err *err;
      interim += err;
    }
    if (interim < sqerr) {
      sqerr = interim;
      index = i;
    }
  }
  *start = index;
  return sqerr;
}



int main(int argc, char *argv[]) {
  // read in the db
  if (argc != 2) {
    printf("err\n");
    return -1;
  }
  int i;
  #define NTARGETS 2
  char *db[NTARGETS] = {"newreturn.raw", "newfetch.raw"};
  int16_t *dbfiles[NTARGETS];
  long length[NTARGETS];
  for (i = 0; i < NTARGETS; i++) {
    dbfiles[i] = loadFile(db[i], &length[i]);
  }
  char *fname = argv[1];
  long l;
  int16_t *noise = loadFile(fname, &l);
  if (l == -1) {
    printf("cannot get hypothesis\n");
    return -1;
  }
  unsigned long long err[NTARGETS];
  int index[NTARGETS];
  for (i = 0; i < NTARGETS; i++) {
    err[i] = square_error(noise, l, dbfiles[i], length[i], &index[i]);
  }

  unsigned long long lowest_err = ((unsigned long long)-1);
  int li = -1;
  for (i = 0; i < NTARGETS; i++) {
    if (err[i] < lowest_err) {
      li = i;
      lowest_err = err[i];
    }
  }

  for (i = 0; i < NTARGETS; i++) {
    printf("err[%d]: %lld\n", i, err[i]);
  }
  printf("lowest: [%d] => %lld\n", li, lowest_err);
  char *text[NTARGETS] = {"return", "fetch"};
  printf("hyp: %s\n", text[li]);
  for (i = 0; i < NTARGETS; i++) {
    free(dbfiles[i]);
  }

  return 0;
}

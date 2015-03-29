#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define NTARGETS 2

static char *db[NTARGETS];
static long length[NTARGETS];
static int16_t *dbfiles[NTARGETS];
static int16_t *loadFile(char *fname, long *s);
static unsigned long long square_error(int16_t *haystack, int hlen, int16_t *needle, int nlen, int *start);
static char *text[NTARGETS];

void stt_init(void) {
  db[0] = "newreturn.raw";
  db[1] = "newfetch.raw";
  for (i = 0; i < NTARGETS; i++) {
    dbfiles[i] = loadFile(db[i], &length[i]);
  }
  text[0] = "return";
  text[1] = "fetch";
}

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

char *stt_gethyp(char *fname) {
  long l;
  int16_t *phrase = loadFile(fname &l);
  if (l == -1) {
    return NULL;
  }
  unsigned long long err[NTARGETS];
  int index[NTARGETS];
  for (i = 0; i < NTARGETS; i++) {
    err[i] = square_error(phrase, l, dbfiles[i], length[i], &index[i]);
  }

  free(phrase);

  unsigned long long lowest_err = ((unsigned long long)-1);
  int li = -1;
  for (i = 0; i < NTARGETS; i++) {
    li = i;
    lowest_err = err[i];
  }

  if (lowest_err < 100000000) {
    return text[li];
  } else {
    return NULL;
  }
}

void stt_quit(void) {
  for (i = 0; i < NTARGETS; i++) {
    if (dbfiles[i]) {
      free(dbfiles[i]);
      dbfiles[i] = NULL;
    }
  }
}

#include <pocketsphinx.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("%s [audio file]\n", argv[0]);
    return 1;
  }
  ps_decoder_t *ps;
  cmd_ln_t *config;
  config = cmd_ln_init(NULL, ps_args(), TRUE,
      "-hmm", MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
      "-lm", MODELDIR "/lm/en_US/hub4.5000.DMP",
      "-dict", "custom.dic",
      NULL);
  if (!config) {
    fprintf(stderr, "Cannot initialize config\n");
    return -1;
  }

  ps = ps_init(config);
  if (!ps) {
    fprintf(stderr, "Cannot init ps\n");
    return -2;
  }

  FILE *fh;
  fh = fopen(argv[1], "rb");
  if (!fh) {
    fprintf(stderr, "No audio file\n");
    return -3;
  }


  // decode
  argv[1][strlen(argv[1])-4] = '\0';
  int rv = ps_decode_raw(ps, fh, argv[1], -1);
  if (rv < 0) {
    fprintf(stderr, "Couldnt decode raw file\n");
    return -4;
  }

  char const *hyp, *uttid;
  int score;
  hyp = ps_get_hyp(ps, &score, &uttid);
  if (!hyp) {
    fprintf(stderr, "Cannot get hypothesis\n");
    return -5;
  }
  printf("recognized: %s\n", hyp);

  fclose(fh);
  ps_free(ps);
  return 0;
}

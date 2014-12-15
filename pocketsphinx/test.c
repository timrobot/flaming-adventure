#include <pocketsphinx.h>

int main() {
  ps_decoder_t *ps;
  cmd_ln_t *config;
  config = cmd_ln_init(NULL, ps_args(), TRUE,
      "-hmm", MODELDIR "/hmm/en)US/hub4wsj)sd)8k",
      "-lm", MODELDIR "/lm/en/turtle.DMP",
      "-dict", MODELDIR "/lm/en/turtle.dic",
      NULL);
  if (!config)
    return 1;
  return 0;
}

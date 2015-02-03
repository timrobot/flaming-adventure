#include <festival.h>

int main() {
  festival_initialize(1, 210000);
  festival_eval_command("(voice_kal_diphone)");
  festival_say_text("I went to the bathroom. There was no toilet paper. I had to go under the kitchen sink in order to grab a new roll.");
  festival_wait_for_spooler();
  return 0;
}

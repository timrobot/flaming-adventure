#ifndef Event_h
#define Event_h

#define ET_KEY    1
#define ET_MOUSE  2
#define ET_QUIT   3

#define EK_UP     1
#define EK_DOWN   2
#define EK_LEFT   3
#define EK_RIGHT  4
#define EK_SPACE  5
#define EK_ESC    6
#define EK_TAB    7

typedef struct {
  int type;
  int key;
} Event_t;

#endif

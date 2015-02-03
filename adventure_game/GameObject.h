#ifndef GameObject_h
#define GameObject_h

typedef struct {
  int event_type;
} Event_t;

class GameObject { // things on a tile
  public:
    SDL_Surface *image;
    Map& region;
    int x;
    int y;

    void update(Event_t e, Environment *env); // FSA
    void draw(SDL_Surface *screen);
};

class Chest : public GameObject {
  public:
    Chest();
    ~Chest();

  private:
    void open();
    bool opened;
};

class NPC : public GameObject {
  public:
    NPC();
    ~NPC();

  private:
    int fpa; // frames per action
    int hraw;
    int vraw;
};

class Player : public GameObject {
  public:
    Player();
    ~Player();
};

#endif

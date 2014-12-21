#ifndef SaveGame_h
#define SaveGame_h

#include "Common.h"

class SaveGameEntry {
  public:
    SaveGameEntry();
    void store();
    void load();

    /* place other fields here too */
};

class SaveGameList {
  public:
    void setDirectory(string path);
    vector<SaveGameEntry> getSavedGames();
    string dirpath;
};

#endif

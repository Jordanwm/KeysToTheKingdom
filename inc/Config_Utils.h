#ifndef _CONFIG_UTILS_H_
#define _CONFIG_UTILS_H_

#include "Map.h"
#include <fstream>

extern Map* gMap;

enum ErrorMsgs {
    FAIL_FIND_CONFIG=0,
    FAIL_LOADING_MAP=1,
    FAIL_OPENING_CONFIG,
    FAIL_LOADING_SKYBOX
};

bool LoadGameFile(int argc, char **argv);

bool LoadMapPoints(ifstream &file);
bool LoadSkybox(ifstream &file);

void PrintErrorMsg(ErrorMsgs msg);

void getFloatsFromFile(ifstream &file, vector<float> &result);

#endif
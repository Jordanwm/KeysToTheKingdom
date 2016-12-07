#include "Config_Utils.h"
#include "Texture_Utils.h"
#include <cstdio>
#include <string>

#define DEBUG_LOAD 0

using namespace std;

extern GLuint* gSkyboxTextureHandles;
extern vector<GLchar*> gSkyboxTextureNames;

/* 
 * LoadGameFile
 *
 * Returns 1 for sucessfully loading
 * Returns 0 for failure
*/
bool LoadGameFile(int argc, char **argv) 
{
    if (argc < 2) { // Didn't include config file 
        PrintErrorMsg(FAIL_FIND_CONFIG);
        return false;
    }   

    ifstream file(argv[1]); // open config file
    if (!file.is_open())
    {
        PrintErrorMsg(FAIL_OPENING_CONFIG);
        return false;
    }

    printf("Loading Game File\n");
    
    bool result = true;

    while (!file.eof() && result)
    {
        string line;
        getline(file, line);
        
        if (line == ".MAP")
            if (!(result = LoadMapPoints(file))) PrintErrorMsg(FAIL_LOADING_MAP);

        if (line == ".SKYBOX")
            if(!(result = LoadSkybox(file))) PrintErrorMsg(FAIL_LOADING_SKYBOX);
    }

    if (result)
        printf("Finished loading Game File\n");

    file.close();

    return result;
}

bool LoadMapPoints(ifstream &file)
{
    printf("-> Loading Map\n");
    int NumberOfPoints = 0;
    vector<float> floats;
    vector<Point*> points;

    // Get number of points
    getFloatsFromFile(file, floats);
    if (floats.size() == 1)
        NumberOfPoints = floats[0];
    else
        return false;

    floats.clear();

    // Load the 16 points fromt he config file
    for (int i = 0; i < NumberOfPoints; ++i)
    {
        getFloatsFromFile(file, floats);

        if (floats.size() == 2){
            Point* p = new Point(floats[0], 0.0, floats[1]);
            points.push_back(p);
        }
        else {
            return false;
        }

        floats.clear();
    }

    gMap = new Map(points);
    return true;
}

bool LoadSkybox(ifstream &file)
{
    printf("-> Loading Skybox\n");

    // Load the 16 points fromt he config file
    int i = 0;
    for (; i < 6 && !file.eof(); ++i) {
        string line;
        getline(file, line); 
        string value = line.substr(2, string::npos);
        char * S = new char[value.length() + 1];
        std::strcpy(S,value.c_str());
        gSkyboxTextureNames.push_back(S);
    }

    if (!LoadTextures(gSkyboxTextureHandles, gSkyboxTextureNames))
        return false;

    if (i != 6)
        return false;

    return true;
}

/* 
 * PrintErrorMsg
 *
 * Prints the provided error msg
*/
void PrintErrorMsg(ErrorMsgs msg){
    string err;
    err = "\nERROR: ";
    switch (msg) {
        case FAIL_FIND_CONFIG:
            err.append("Please Include Game config files: \"./fp res/Config.txt\"");
            break;
        case FAIL_LOADING_MAP:
            err.append("Failed to load map, please fix config file.");
            break;
        case FAIL_OPENING_CONFIG: 
            err.append("Failed to open config file.");
            break;
        case FAIL_LOADING_SKYBOX:
            err.append("Failed to load skybox textures");
            break;
    }

    err.append("\n\n");

    printf(err.c_str());
}

/*
 * Helper Function
*/
void getFloatsFromFile(ifstream &file, vector<float> &result) {
    string line;
    getline(file, line);

    string value = line.substr(2, string::npos);

    size_t pos = 0;
    string token;
    while ((pos = value.find(',')) != string::npos) {
        token = value.substr(0, pos);        
        result.push_back(atof(token.c_str()));
        value.erase(0, pos + 1);
    }

    result.push_back(atof(value.c_str()));

    if (DEBUG_LOAD){
        for (int i = 0; i < result.size(); ++i){
            printf("%.1f\n", result[i]);
        }
    }
}
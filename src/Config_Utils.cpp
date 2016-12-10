#include <GL/glew.h>

#ifdef __APPLE__            // if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else                   // else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "Config_Utils.h"
#include "Shader_Utils.h"
#include "Texture_Utils.h"
#include <cstdio>
#include <string>
#include <vector>



#define DEBUG_LOAD 0

using namespace std;

extern GLuint* gSkyboxTextureHandles;
extern vector<GLchar*> gSkyboxTextureNames;

extern GLuint* gTrackTextureHandles;
extern vector<GLchar*> gTrackTextureNames;

extern GLuint trackShaderHandle;

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

        if (line == ".TRACK_TEXTURE")
            if(!(result = LoadTrackTexture(file))) PrintErrorMsg(FAIL_LOADING_TRACK_TEXTURE);

        if (line == ".TRACK_SHADER")
            if(!(result = LoadTrackShaders(file))) PrintErrorMsg(FAIL_LOADING_TRACK_SHADER);            
    }

    if (result)
        printf("Finished loading Game File\n");

    file.close();

    return result;
}

extern vector<Point*> points;
bool LoadMapPoints(ifstream &file)
{
    printf("-> Loading Map\n");
    int NumberOfPoints = 0;
    vector<float> floats;

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
        strcpy(S,value.c_str());
        gSkyboxTextureNames.push_back(S);
    }

    if (!LoadTextures(gSkyboxTextureHandles, gSkyboxTextureNames))
        return false;

    if (i != 6)
        return false;

    return true;
}

bool LoadTrackTexture(ifstream &file)
{
    printf("-> Loading Track Texture\n");

    string line;
    getline(file, line); 
    string value = line.substr(2, string::npos);
    char * S = new char[value.length() + 1];
    std::strcpy(S,value.c_str());
    gTrackTextureNames.push_back(S);

    if (!LoadTextures(gTrackTextureHandles, gTrackTextureNames))
        return false;

    return true;
}

bool LoadTrackShaders(ifstream &file)
{
    printf("-> Loading Track Shaders\n");

    vector<char*> files;
    int i = 0;
    for (; i < 2 && !file.eof(); ++i) {
        string line;
        getline(file, line); 
        string value = line.substr(2, string::npos);
        char * S = new char[value.length() + 1];
        std::strcpy(S,value.c_str());
        files.push_back(S);
    }

    if ((trackShaderHandle = createShaderProgram(files[0], files[1])) == 0)
        return false;

    /*if (trackShaderHandle != 0)
        printf("%s\n", "----> Successfully Compiled Shader");*/

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
        case FAIL_LOADING_TRACK_TEXTURE:
            err.append("Failed to load track texture");
            break;
        case FAIL_LOADING_TRACK_SHADER:
            err.append("Failed to load track shader");
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
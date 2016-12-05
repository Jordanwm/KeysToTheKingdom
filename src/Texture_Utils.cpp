#include "Texture_Utils.h"

#define DEBUG_LOAD_TEXTURE 0

using namespace std;

bool LoadTextures(GLuint* &textureHandles, vector<char*> textureNames)
{
	textureHandles = new GLuint[textureNames.size()];

    for(GLuint i = 0; i < textureNames.size(); i++)
    {
        textureHandles[i] = SOIL_load_OGL_texture(
            textureNames[i],
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS
            | SOIL_FLAG_COMPRESS_TO_DXT
        );

        if (DEBUG_LOAD_TEXTURE)
            cout << textureHandles[i] << endl;

        if (textureHandles[i] == NULL)
            return false;
    }

    return true;
}

void BindTexture(int index, GLuint* textureHandles){
	glGenTextures(0, &textureHandles[index]);
    glBindTexture(GL_TEXTURE_2D, textureHandles[index]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
}
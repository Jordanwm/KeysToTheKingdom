#include "Texture_Utils.h"

using namespace std;

extern GLuint* gSkyboxTextureHandles;
extern vector<GLchar*> gSkyboxTextureNames;

bool LoadTextures()
{
	gSkyboxTextureHandles = new GLuint[6];

    for(GLuint i = 0; i < gSkyboxTextureNames.size(); i++)
    {
        gSkyboxTextureHandles[i] = SOIL_load_OGL_texture(
            gSkyboxTextureNames[i],
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS
            | SOIL_FLAG_COMPRESS_TO_DXT
        );

        if (gSkyboxTextureHandles[i] == NULL)
            return false;
    }

    return true;
}

void BindTexture(int index){
    GLuint* gTextureHandles;
	glGenTextures(0, &gTextureHandles[index]);
    glBindTexture(GL_TEXTURE_2D, gTextureHandles[index]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
}
#include "Texture_Utils.h"

GLuint* gTextureHandles;
vector<GLchar*> gTextureNames;

void LoadTextures()
{
	gTextureHandles = new GLuint[7];
    
    gTextureNames.push_back("textures/troubled-waters_rt.tga");
    gTextureNames.push_back("textures/troubled-waters_lf.tga");
    gTextureNames.push_back("textures/troubled-waters_up.tga");
    gTextureNames.push_back("textures/troubled-waters_dn.tga");
    gTextureNames.push_back("textures/troubled-waters_bk.tga");
    gTextureNames.push_back("textures/troubled-waters_ft.tga");
    gTextureNames.push_back("textures/brick.png");

    for(GLuint i = 0; i < gTextureNames.size(); i++)
    {
        gTextureHandles[i] = SOIL_load_OGL_texture(
            gTextureNames[i],
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS
            | SOIL_FLAG_COMPRESS_TO_DXT
        );

        if (gTextureHandles[i] == NULL)
            cout << "ERROR" << endl;
    }
}

void BindTexture(int index){
	glGenTextures(0, &gTextureHandles[index]);
    glBindTexture(GL_TEXTURE_2D, gTextureHandles[index]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
}
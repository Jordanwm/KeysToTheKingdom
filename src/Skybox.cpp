#include "Skybox.h"

#include "Texture_Utils.h"

extern GLuint* gSkyboxTextureHandles;

Skybox::Skybox(){}

void Skybox::GenerateQuad(int s){
    switch(s){
        case RIGHT:
            glNormal3f(-1,0,0);
            glTexCoord2f(1,1);
            glVertex3f(100, -100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1, 0);
            glVertex3f(100, 100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(0,0);
            glVertex3f(100, 100, 100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(0, 1);
            glVertex3f(100, -100, 100);
            break;
        case LEFT:
            glNormal3f(-1,0,0);
            glTexCoord2f(1,1);
            glVertex3f(-100, -100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1, 0);
            glVertex3f(-100, 100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(0,0);
            glVertex3f(-100, 100, 100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(0, 1);
            glVertex3f(-100, -100, 100);
            break;
        case TOP:
            glNormal3f(-1,0,0);
            glTexCoord2f(0,0);
            glVertex3f(-100, 100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1, 0);
            glVertex3f(100, 100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1,1);
            glVertex3f(100, 100, 100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(0, 1);
            glVertex3f(-100, 100, 100);
            break;
        case BOTTOM:
            glNormal3f(-1,0,0);
            glTexCoord2f(0,0);
            glVertex3f(-100, -100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1, 0);
            glVertex3f(100, -100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1,1);
            glVertex3f(100, -100, 100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(0, 1);
            glVertex3f(-100, -100, 100);
            break;
        case FRONT:
            glNormal3f(-1,0,0);
            glTexCoord2f(0,0);
            glVertex3f(100, 100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(0, 1);
            glVertex3f(100, -100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1,1);
            glVertex3f(-100, -100, -100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1, 0);
            glVertex3f(-100, 100, -100);
            break;
        case BACK:
            glNormal3f(-1,0,0);
            glTexCoord2f(0,0);
            glVertex3f(100, 100, 100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1, 0);
            glVertex3f(100, -100, 100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(1,1);
            glVertex3f(-100, -100, 100);
            
            glNormal3f(-1,0,0);
            glTexCoord2f(0, 1);
            glVertex3f(-100, 100, 100);
        default:
            break;
    }
}

void Skybox::Draw()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    int width,height; 
    for(int i = 0; i < 6; i++)
    {
        BindTexture(i, gSkyboxTextureHandles);

        glPushMatrix();
            glColor3f(1, 1, 1);
            glBegin(GL_QUADS);
                GenerateQuad(i);
            glEnd();
        glPopMatrix();
    }    

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

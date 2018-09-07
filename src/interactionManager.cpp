//
// Created by Sarah Gross on 4/22/18
//
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>

#include "interactionManager.h"

bool mouseDown = false;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

float zoom = 0.0f;

int mode = 0;

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (0.0, 0.0, -10.0, 0.0, 5.0, 10.0, 0.0, 1.0, 0.0);
}

void setCamera()
{
    glTranslatef(0, 0, zoom);
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
}

void resetCamera()
{
    xrot=0;yrot=0;zoom=0;
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 's':
            mode = 1 - mode;
            glutPostRedisplay();
            break;

        default:
            break;
    }
}

// Function for rotating the mesh with the mouse
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseDown = true;

        xdiff = x - yrot;
        ydiff = -y + xrot;
    }
    else
    {
        mouseDown = false;

        // Wheel reports as button 3(scroll up) and button 4(scroll down)
        if ((button == 3) || (button == 4)) // It's a wheel event
        {
            // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
            if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events

            if(button == 3)
            {
                zoom += 1.0;
            }
            else
            {
                zoom -= 1.0;
            }
            glutPostRedisplay();
        }
    }
}

// Function for rotating the pyramid with the mouse
void mouseMotion(int x, int y)
{
    if (mouseDown)
    {
        yrot = x - xdiff;
        xrot = y + ydiff;

        glutPostRedisplay();
    }
}

void renderBitmapString(float x,float y,void *font,std::string string) {
    char *cstr = &string[0u];
    char *c;
    glRasterPos2i(x, y);
    for (c=cstr; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
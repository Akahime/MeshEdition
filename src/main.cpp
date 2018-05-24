#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "../Core/OBJFileUtil.h"
#include "MeshOperator.h"
#include "interactionManager.h"


using namespace MeshLib;

Solid *mesh;
MeshOperator mo;
OBJFileUtil of;

std::string fileName;
std::string infoString="";
int iterations = 0;
static int menu_id;

void displayMesh(int mode)
{
    SolidFaceIterator faceIterator(mesh);
    switch (mode)
    {
        case 0:
            for(; !faceIterator.end(); ++faceIterator)
            {
                Point p[3] = {(*faceIterator)->halfedge()->source()->point(),(*faceIterator)->halfedge()->target()->point(),(*faceIterator)->halfedge()->he_next()->target()->point()};

                glBegin(GL_TRIANGLES);
                glNormal3f((*faceIterator)->norm()(0), (*faceIterator)->norm()(1), (*faceIterator)->norm()(2));
                for(unsigned int j=0; j<3;++j)
                {
                    glVertex3f(p[j](0),p[j](1),p[j](2));
                }
                glEnd();
            }
            break;
        case 1:
            for(; !faceIterator.end(); ++faceIterator)
            {
                Vertex *v[3] = {(*faceIterator)->halfedge()->source(),(*faceIterator)->halfedge()->target(),(*faceIterator)->halfedge()->he_next()->target()};

                glBegin(GL_TRIANGLES);
                for(unsigned int j=0; j<3;++j)
                {
                    glNormal3f(v[j]->normal()(0),v[j]->normal()(1),v[j]->normal()(2));
                    glVertex3f(v[j]->point()(0),v[j]->point()(1),v[j]->point()(2));
                }
                glEnd();
            }
            break;
        default:
            break;

    }
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mode == 0)
    {
        glShadeModel(GL_FLAT);
    }
    else
    {
        glShadeModel(GL_SMOOTH);
    }

    renderBitmapString(2,7, GLUT_BITMAP_HELVETICA_18, infoString);
    
    glPushMatrix();

    setCamera();

    //glTranslatef(-5,-5,-20);
    //glutSolidSphere (1.0, 20, 16);
    //glScalef(10,10,10);

    displayMesh(mode);
    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}

void initLights(void)
{
    //GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 10.0 };
    GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};

    GLfloat light_position[] = { 1.0, 1.0, 0, 0.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 1.0, 0.1};
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void init()
{
    mesh = new Solid();
    std::ifstream in(fileName);
    of.readToSolid(mesh, in);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    initLights();
    glEnable(GL_DEPTH_TEST);
}

void menu(int num){
    clock_t start, end;
    switch(num)
    {
        case 0:
            resetCamera();
            break;
        case 1:
            start = clock();
            mo.LoopSubdivisionOneStep(mesh);
            iterations += 1;
            end = clock();
            infoString = "Loop subdivision completed. Time = " + std::to_string((double)(end - start) / (double)CLOCKS_PER_SEC)+"s";
            break;
        case 2:
            start = clock();
            mo.Simplification(mesh);
            end = clock();
            infoString = "Simplification completed. Time = " + std::to_string((double)(end - start) / (double)CLOCKS_PER_SEC)+"s";
            break;
        case 3:
            std::ofstream out((fileName.substr(0, fileName.rfind(".")) + ".loop."+std::to_string(iterations)+".obj").c_str());
            of.saveSolid(mesh, out);
            infoString = "File successfully saved in "+(fileName.substr(0, fileName.rfind(".")) + ".loop."+std::to_string(iterations)+".obj");
            break;
    }
    glutPostRedisplay();
}

void createMenu(){
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Reset camera",0);
    glutAddMenuEntry("Loop subdivision",1);
    glutAddMenuEntry("Simplification",2);
    glutAddMenuEntry("Save object",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}



int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "USAGE: [.exe] [in.obj]" << std::endl;
        return -1;
    }

    if( access( argv[1], F_OK ) == -1 )
    {
        std::cout << "No file with name "<<argv[1]<<" was found." << std::endl;
        return -1;
    }
    fileName = argv[1];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Display and simplify Mesh");
    init ();
    createMenu();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutMainLoop();

	return 0;
}

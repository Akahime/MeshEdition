#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "../Core/OBJFileUtil.h"
#include "MeshOperator.h"

using namespace MeshLib;

Solid *mesh;
OBJFileUtil of;

std::string fileName;
int iterations = 2, mode = 0;

/*
Point* vertexNormal(Vertex vert)
{
    VertexFaceIterator vfIterator(vert);
    Point *N = new Point();
    for (; !vvIterator.end(); ++vvIterator)
    {
        *N += (*vvIterator)->norm();
    }
    *N = *N / N->norm();
    return N;
}*/


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


void loopSubdivision()
{
    //Make Operations
    MeshOperator mo;
    clock_t start = clock();
    for (unsigned i = 0; i < iterations; ++i)
    {
        std::cout << "Iteration " << i << std::endl;
        mo.LoopSubdivisionOneStep(mesh);
    }
    clock_t end = clock();
    std::cout << "Time consumed: " << (double)(end - start) / (double)CLOCKS_PER_SEC << std::endl;

    // Write out the resultant obj file
    std::ofstream out((fileName.substr(0, fileName.rfind(".")) + ".loop."+std::to_string(iterations)+".obj").c_str());
    of.saveSolid(mesh, out);
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    loopSubdivision();
    glLoadIdentity();

    glTranslatef(-5,-5,-20);
    gluLookAt (0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 1.0, 0.0);
    //glutSolidSphere (1.0, 20, 16);

    glScalef(10,10,10);
    displayMesh(mode);

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
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "USAGE: [.exe] [in.obj]" << std::endl;
        return -1;
    }
    fileName = argv[1];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Display and simplify Mesh");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMainLoop();

	return 0;
}

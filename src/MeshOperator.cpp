//
// Created by Sarah Gross on 5/19/18
//
#include "MeshOperator.h"
#include "../Core/iterators.h"
#include <vector>
#include <cmath>

using namespace MeshLib;

# define PI           3.14159265358979323846

MeshOperator::MeshOperator() {}

MeshOperator::~MeshOperator() {}

void MeshOperator::LoopSubdivisionOneStep(Solid *mesh)
{
    Solid meshCopy;
    mesh->copy(meshCopy);

    int i=0;

    // Copy original mesh edges
    SolidEdgeIterator edgeIterator(mesh);
    Edge *oldMeshEdges[mesh->numEdges()];
    for(i=0; !edgeIterator.end(); i++, ++edgeIterator)
    {
        oldMeshEdges[i] = *edgeIterator;
    }

    // Copy original mesh vertices
    SolidVertexIterator vertexIterator(mesh);
    Vertex *oldMeshVertices[mesh->numVertices()];
    for(i=0; !vertexIterator.end(); i++, ++vertexIterator)
    {
        oldMeshVertices[i] = *vertexIterator;
    }

    // Get face with maximum id (for using split function)
    SolidFaceIterator faceIterator(mesh);
    int maxFaceId = 0;
    for(i=0; !faceIterator.end(); i++, ++faceIterator)
    {
        if((*faceIterator)->id() > maxFaceId)
        {
            maxFaceId = (*faceIterator)->id();
        }
    }

    // Loops through Edges of new mesh (only those common with old mesh) and split them
    std::vector<Edge*> edgesToSwap;
    Edge *newEdgesFromSplit[2]={NULL,NULL};

    SolidEdgeIterator newEdgeIterator(&meshCopy);
    for(i=0; i<(sizeof(oldMeshEdges)/sizeof(*oldMeshEdges)); i++, ++newEdgeIterator)
    {
        // Edge we are working on
        Edge *oldEdge = oldMeshEdges[i];
        Point oldPoint1 = mesh->edgeVertex1(oldEdge)->point();
        Point oldPoint2 = mesh->edgeVertex2(oldEdge)->point();

        // New vertex from split
        newEdgesFromSplit[0] = NULL;
        newEdgesFromSplit[1] = NULL;
        Vertex *newVertex = mesh->edgeSplit( oldEdge , newEdgesFromSplit, maxFaceId);

        // Position new vertex
        if(mesh->isBoundary(*newEdgeIterator)) // If boundary = formula 1/2
        {
            newVertex->point() = (oldPoint1 + oldPoint2)/2;
        }
        else // If not boundary = formula 3/8 x 1/8
        {
            HalfEdge *oldEdgeHalfs[2] = {(*newEdgeIterator)->halfedge(0),(*newEdgeIterator)->halfedge(1)};
            newVertex->point() = (oldPoint1 + oldPoint2)*3/8 + (oldEdgeHalfs[0]->he_next()->target()->point() + oldEdgeHalfs[1]->he_next()->target()->point())/8;
        }

        // Insert the new edges gotten from split in the swap list (Solid::edgeSplit only sends edges that we need to swap)
        if(newEdgesFromSplit[0] != NULL) edgesToSwap.push_back(newEdgesFromSplit[0]);
        if(newEdgesFromSplit[1] != NULL) edgesToSwap.push_back(newEdgesFromSplit[1]);
    }


    //new position of old vertices determined here
    SolidVertexIterator newVertexIterator(&meshCopy);
    double beta; int n;
    for(i=0; i<(sizeof(oldMeshVertices)/sizeof(*oldMeshVertices)); i++,++newVertexIterator)
    {
        Vertex *oldVert = oldMeshVertices[i];
        
        VertexVertexIterator vertNeighboursIterator(*newVertexIterator); //Iterate over all vertices which are neighbours of oldVert
        Point newPoint = Point(0,0,0);

        if(mesh->isBoundary(oldVert)) // If boundary = formula 3/4 x 1/8
        {
            // We make the ponderated sum of neighbours which are also at boundaries
            for(n=0; !vertNeighboursIterator.end(); ++vertNeighboursIterator)
            {
                if(mesh->isBoundary(*vertNeighboursIterator))
                {
                    newPoint += (*vertNeighboursIterator)->point();
                    n++;
                }
            }
            newPoint = oldVert->point()*3/4 + newPoint/8;
        }
        else // If boundary = formula (1-nB) x B
        {
            // We make the ponderated sum of neighbours
            for(n=0; !vertNeighboursIterator.end(); n++, ++vertNeighboursIterator)
            {
                newPoint += (*vertNeighboursIterator)->point();
            }

            beta = (n > 3) ? (1.0/n)*((5.0/8.0)-pow(((3.0/8.0)+(1.0/4.0)*(cos(2*PI/n))),2)) : 3/16;
            newPoint = oldVert->point() * (1 - n * beta) + (newPoint) * beta;

        }
        oldVert->point() = newPoint;
    }

    // Swap edges
    for(std::vector<Edge*>::iterator swapit = edgesToSwap.begin(); swapit != edgesToSwap.end(); swapit++)
    {
        Edge *e=*swapit;
        mesh->edgeSwap(e,((e->halfedge(0))->he_next())->target(),((e->halfedge(1))->he_next())->target());
    }

}

void Simplification(Solid *mesh)
{

}
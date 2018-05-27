//
// Created by Sarah Gross on 5/19/18
//
#include "MeshOperator.h"
#include "../Core/iterators.h"
#include "../Core/mutablePriorityQueue.h"
#include <vector>

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

    // Loops through Edges (only those common with old mesh) and split them
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


    // Position all vertices
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

void MeshOperator::Simplification(Solid *mesh)
{
    glm::mat4 K;
    HalfEdge *h;

    // Compute quadrics for each face
    SolidFaceIterator faceIterator(mesh);

    for(; !faceIterator.end(); ++faceIterator)
    {
        // We want the vector (a, b, c, d) such as the face equation is ax+by+cz+d=0
        // (a,b,c) is the normal, we find d using one vertex in the face
        Face *f = (*faceIterator);

        // Outer product of glm is unfortunately down ...
        double a = f->norm()(0);
        double b = f->norm()(1);
        double c = f->norm()(2);
        double d = -(f->norm()*f->halfedge()->target()->point());

        K = glm::mat4(glm::vec4(a*a, a*b, a*c, a*d), glm::vec4(b*a, b*b, b*c, b*d), glm::vec4(c*a, c*b, c*c, c*d), glm::vec4(a*d, b*d, c*d, d*d));
        (*faceIterator)->quadric() = K;
    }

    // Compute quadrics for each vertex
    SolidVertexIterator vertexIterator(mesh);
    for(; !vertexIterator.end(); ++vertexIterator)
    {
        K = glm::mat4x4(0.0f);
        h = (*vertexIterator)->halfedge();

        // Iterate over the neighbouring faces
        for(VertexFaceIterator vfIterator(h->source()); !vfIterator.end (); ++vfIterator){
            K += (*vfIterator)->quadric();
        }
    }

    // For each edge, compute edge record
    SolidEdgeIterator edgeIterator(mesh);
    MutablePriorityQueue<EdgeRecord> queue;
    for(; !edgeIterator.end(); ++edgeIterator)
    {
        (*edgeIterator)->record = EdgeRecord((*edgeIterator));
        queue.insert((*edgeIterator)->record);
    }

    // Until target number of triangles is reached, collapse cheapest edge
    int currFaces = mesh->numFaces();
    int targetFaces = currFaces/4;
    while(currFaces > targetFaces)
    {
        // COLLAPSE EDGE ROUTINE
        // Get cheapest edge
        Edge *cheapEdge = queue.top().edge;

        // Remove it from queue
        queue.pop();

        // Compute the new quadric
        K = cheapEdge->halfedge(0)->source()->quadric() + cheapEdge->halfedge(0)->target()->quadric();

        // Remove edges touching its endpoints from the queue
        for(int i=0; i<2; i++)
        {
            h = cheapEdge->halfedge(i);

            //Iterate over neighbouring edges
            for(VertexEdgeIterator veIterator(h->target()); !veIterator.end (); ++veIterator){
                queue.remove((*veIterator)->record);
            }
        }

        // Collapse edge (merges source and target to source vertex)
        Vertex *newVert = cheapEdge->halfedge(0)->source();
        mesh->collapseEdge(cheapEdge);

        //Set the quadric of new vertex
        newVert->quadric() = K;

        // Add in queue new records of edges touching new vertex
        for(VertexEdgeIterator veIterator(newVert); !veIterator.end (); ++veIterator){
            (*veIterator)->record = EdgeRecord((*veIterator));
            queue.insert((*veIterator)->record);
        }

        // Update number of faces
        currFaces = mesh->numFaces();
    }
}
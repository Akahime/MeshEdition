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
    /*
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

    // Position old vertices
    SolidVertexIterator newVertexIterator(&meshCopy);
    for(i=0; i<(sizeof(oldMeshVertices)/sizeof(*oldMeshVertices)); i++, ++newVertexIterator)
    {
        Vertex *oldVert = oldMeshVertices[i];

        VertexVertexIterator vertNeighboursIterator(oldVert);
        Point *newPoint = new Point();
        int n;

        if(mesh->isBoundary(*newVertexIterator)) // If boundary = formula 3/4 x 1/8
        {
            // We make the ponderated sum of neighbours which are also at boundaries
            for(n=0; !vertNeighboursIterator.end(); n++, ++vertNeighboursIterator)
            {
                if(mesh->isBoundary(*vertNeighboursIterator))
                {
                    *newPoint += (*vertNeighboursIterator)->point()*1/8;
                }
            }

            *newPoint += oldVert->point()*3/4;

            if(n != 2)
            {
                std::cout<<"Error on boundaries";
            }
        }
        else // If not boundary = formula 1-nB - B
        {
            // We make the ponderated sum of all neighbours
            for(n=0; !vertNeighboursIterator.end(); n++, ++vertNeighboursIterator)
            {
                *newPoint += (*vertNeighboursIterator)->point();
            }

            int beta = (n > 3) ? 3/(8*n) : 3/16;

            *newPoint = *newPoint * beta + oldVert->point()*(1 - n * beta);
        }
        
        oldVert->point() = *newPoint;
    }
    
    // Swap Edges
    for(std::vector<Edge*>::iterator swapIterator = edgesToSwap.begin(); swapIterator != edgesToSwap.end(); ++swapIterator)
    {
        Edge *e=*swapIterator;
        mesh->edgeSwap(e,((e->halfedge(0))->he_next())->target(),((e->halfedge(1))->he_next())->target());
    }
     */



    //storing all vertices
    int i=0;
    SolidVertexIterator veriter(mesh);
    Vertex *allvert[mesh->numVertices()];
    for(i=0;!veriter.end();i++,++veriter)
    {
        allvert[i]=*veriter;
    }

    //storing all edges
    SolidEdgeIterator eiter(mesh);
    Edge *alledges[mesh->numEdges()];
    for(i=0;!eiter.end();i++,++eiter)
    {
        alledges[i]=*eiter;
    }


    std::vector<Edge*> swapList;
    Edge *splitSwap[2]={NULL,NULL}; //2 edges

    //getting max face for id of new faces
    SolidFaceIterator fciter(mesh);
    int maxfcid=-1;
    for(; !fciter.end(); ++fciter)
    {
        if((*fciter)->id()>maxfcid)
            maxfcid=(*fciter)->id();
    }

    //splitting and swapping (flipping) by iterating through edges
    SolidEdgeIterator eitercpy(&meshCopy);
    int j;
    double x,y,z;
    for(i=0,j=0;i<(sizeof(alledges)/sizeof(*alledges));i++,++eitercpy) // We should not split an edge already split so limit to alledges, but iterate over the new mesh
    {
        splitSwap[0]=splitSwap[1]=NULL;
        Vertex *evert1= mesh->edgeVertex1(alledges[i]);
        Vertex *evert2= mesh->edgeVertex2(alledges[i]);
        Vertex *ver1=mesh->edgeSplit(alledges[i],splitSwap,maxfcid); // New created vertex by split, don't have a position

        //splitSwap contains 2 new created edges
        if(splitSwap[0]!=NULL)
            swapList.push_back(splitSwap[0]);
        if(splitSwap[1]!=NULL)
            swapList.push_back(splitSwap[1]);

        //position of new vertex only (middle of edge) : first if it's boundary, else normal case
        if(mesh->isBoundary(*eitercpy))
        {
            //printf("Check");
            x=(evert1->point()(0)/2)+(evert2->point()(0)/2);
            y=(evert1->point()(1)/2)+(evert2->point()(1)/2);
            z=(evert1->point()(2)/2)+(evert2->point()(2)/2);
            //printf("\n%f %f %f\n",x,y,z);
        }
        else
        {
            x=((evert1->point()(0))+(evert2->point()(0)))*(3.0/8.0);
            y=((evert1->point()(1))+(evert2->point()(1)))*(3.0/8.0);
            z=((evert1->point()(2))+(evert2->point()(2)))*(3.0/8.0);
            x+=(((((*eitercpy)->halfedge(0))->he_next())->target())->point()(0)/8.0)+(((((*eitercpy)->halfedge(1))->he_next())->target())->point()(0)/8.0);
            y+=(((((*eitercpy)->halfedge(0))->he_next())->target())->point()(1)/8.0)+(((((*eitercpy)->halfedge(1))->he_next())->target())->point()(1)/8.0);
            z+=(((((*eitercpy)->halfedge(0))->he_next())->target())->point()(2)/8.0)+(((((*eitercpy)->halfedge(1))->he_next())->target())->point()(2)/8.0);
        }

        ver1->point()=Point(x,y,z);
    }


    //new position of old vertices determined here
    SolidVertexIterator veritercpy(&meshCopy);
    double tem1,tem2,tem3,alpha;
    for(i=0;i<(sizeof(allvert)/sizeof(*allvert));i++,++veritercpy)
    {
        Vertex *oldv=allvert[i];
        //printf("\n%f %f %f -> ",oldv->point()(0),oldv->point()(1),oldv->point()(2));
        VertexVertexIterator vviter(*veritercpy); //Iterate over all vertices around *veritercpy = *oldv
        //printf("Im here\n");
        tem1=tem2=tem3=0;
        j=0;
        if(mesh->isBoundary(allvert[i]))
        {
            //printf("Hi");
            for(j=0;!vviter.end();++vviter)
            {
                if(mesh->isBoundary(*vviter))
                {
                    tem1+=(*vviter)->point()(0);
                    tem2+=(*vviter)->point()(1);
                    tem3+=(*vviter)->point()(2);
                    j++;
                }
            }
            x=tem1/8.0 + oldv->point()(0)*(3.0/4.0);
            y=tem2/8.0 + oldv->point()(1)*(3.0/4.0);
            z=tem3/8.0 + oldv->point()(2)*(3.0/4.0);
        }
        else
        {
            tem1=tem2=tem3=0;
            for(j=0;!vviter.end();++vviter)
            {
                tem1+=(*vviter)->point()(0);
                tem2+=(*vviter)->point()(1);
                tem3+=(*vviter)->point()(2);
                j++;
            }
            //printf("%d\n",j);
            if(j>3)
                alpha=(1.0/j)*((5.0/8.0)-pow(((3.0/8.0)+(1.0/4.0)*(cos(2*PI/j))),2));
            else if(j==3)
                alpha=3/16;

            x=((1-j*alpha)*oldv->point()(0)) + (alpha*(tem1));
            y=((1-j*alpha)*oldv->point()(1)) + (alpha*(tem2));;
            z=((1-j*alpha)*oldv->point()(2)) + (alpha*(tem3));;
        }
        oldv->point()=Point(x,y,z);
        //printf("\n%f %f %f %f-> ",x,y,z, (1.0/4.0)*cos(2*PI/j));
        //printf("\n%f %f %f -> ",oldv->point()(0),oldv->point()(1),oldv->point()(2));
    }

    // Swap edges
    for(std::vector<Edge*>::iterator swapit = swapList.begin(); swapit != swapList.end(); swapit++)
    {
        Edge *e=*swapit;
        mesh->edgeSwap(e,((e->halfedge(0))->he_next())->target(),((e->halfedge(1))->he_next())->target());
    }


}

void Simplification(Solid *mesh){

}
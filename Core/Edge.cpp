//#include "stdafx.h"

#include "Edge.h"
#include "HalfEdge.h"
#include "Face.h"

#include "Point.h"
#include "Vertex.h"
using namespace MeshLib;

bool Edge::coface(Vertex * v)
{
	HalfEdge * he0 = m_halfedge[0];
	if(he0->face()->include_vertex(v))
		return true;
	if(m_halfedge[1] != NULL && m_halfedge[1]->face()->include_vertex(v))
		return true;
	return false;
}

bool Edge::on_sphere (Point p, double radius){
	Vertex * v1 = m_halfedge[0]->source ();
	Vertex * v2 = m_halfedge[0]->target ();
	Point p1 = v1->point ();
	Point p2 = v2->point ();
	if(((p1-p).norm ()-radius)*((p2-p).norm ()-radius)<=0)
		return true;
	return false;
}

void Edge::crosspoint (Point p, double radius, double & b0, double & b1)
{
	Point p0 = m_halfedge[0]->source ()->point ();
	Point p1 = m_halfedge[0]->target ()->point ();
	if((p-p0).norm ()<1e-15)
	{
		b0 = radius;
		b1 = radius;
		return;
	}
	double cost = (p1-p0) * (p-p0)/(p1-p0).norm ()/(p-p0).norm ();
	double a = (p-p0).norm ();
	if(radius*radius-a*a+a*a*cost*cost<0){
		b0 = -1;
		b1 = -1;
		return;
	}
	double t = sqrt(fabs(a*a*cost*cost-a*a+radius*radius));
	b0 = a * cost + t;
	b1 = a * cost - t;
}

bool Edge::coface(Edge * e)
{
		HalfEdge * he1 = m_halfedge[0];
		HalfEdge * he2 = m_halfedge[1];
		HalfEdge * hee1 = e->halfedge (0);
		HalfEdge * hee2 = e->halfedge (1);
		if(he1->face ()==hee1->face ())
			return true;
		if(he2 != NULL)
		{
			if( he2->face () == hee1->face ()) return true;
		}
		if(hee2 != NULL)
		{
			if(hee2->face () == he1->face ()) return true;
		}
		if(he2 != NULL && hee2 != NULL)
		{
			if(he2->face () == hee2->face ()) return true;
		}
		return false;
};

Vertex * Edge::other_vertex(Vertex *v)
{
	HalfEdge * he = m_halfedge[0];
	if(he->target () == v) return he->source ();
	else if (he->source () == v) return he->target ();
	return NULL;

}
double Edge::length()
{
	HalfEdge * he = m_halfedge[0];
	Point p1 = he->target ()->point ();
	Point p2 = he->source ()->point ();
	return (p1-p2).norm ();
}

Vertex * Edge::conjunction(Edge *e)
{
	if((m_halfedge[0]->source()->id() == e->vertex(0)) || (m_halfedge[0]->source()->id() == e->vertex (1)))
		return m_halfedge[0]->source();
	else if(m_halfedge[0]->target()->id() == e->vertex(0) || m_halfedge[0]->target()->id() == e->vertex (1))
		return m_halfedge[0]->target();
	else return NULL;

}

void Edge::get_vertices(Vertex *&v1, Vertex *&v2)
{
	v1 = m_halfedge[0]->source();
	assert(v1);
	v2 = other_vertex(v1);
	assert(v2);
}

// Code by Sarah G for Simplification
EdgeRecord::EdgeRecord(Edge *e)
{
	HalfEdge* he = e->halfedge(0);
	glm::mat4x4 K = he->target()->quadric() + he->source()->quadric();

	// Solve Ax = b -- Section 4 of paper by Garland
	glm::mat4x4 A = K;
	A[3][0] = 0; A[3][1] = 0; A[3][2] = 0; A[3][3] = 1;
	glm::vec4 b = glm::vec4(0,0,0,1);

	if(glm::determinant(A) != 0) // A is invertible
	{
		glm::vec4 x = glm::inverse(A) * b; // solve Ax = b for x, by hitting both sides with the inverse of A

		// Store the optimal point
		optimalPoint = Point(x[0],x[1],x[2]);

		// Compute the cost
		cost = glm::dot(x, (K * x));
	}
	else
	{
		std::cout<<"Error : EdgeRecord matrix not invertible !!"<<std::endl;

		// TODO
	}
	edge = e;

}
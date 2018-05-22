#ifndef _MESHLIB_EDGE_H_
#define _MESHLIB_EDGE_H_

#include <iostream>
#include <string>
#include <assert.h>
#include <math.h>
#include "Vertex.h"

namespace MeshLib{

class HalfEdge;
class Vertex;
class Trait;

//!  EdgeKey class. 
/*!
  This class define edgekey to simplify edge operation.
*/
class EdgeKey
{

	friend class Edge;
public:

	//!  Constructor 1.
	EdgeKey( ){ m_s = 0; m_t = 0 ; };
	//!  Constructor 2 to make sure source ID < target ID.
	/*!
	  \param s an int which is edge source vertex ID.
	  \param t an int which is edge target vertex ID.
	*/
	EdgeKey( int s, int t ){ if( s < t ){ m_s = s; m_t = t; } else { m_s = t; m_t = s; } ;};
	//!  Destructor.
	~EdgeKey(){};

	//!  == operator.
    /*!      
      \param key a EdgeKey to compare with this edgekey.
      \return the boolean result of the == comparison of two edgekeys.
    */
	bool operator == ( const EdgeKey & key ) const { return m_s == key.m_s  && m_t == key.m_t; };
	//!  < operator.
    /*!      
      \param key a EdgeKey to compare with this edgekey.
      \return the boolean result of the < comparison of two edgekeys.
    */
	bool operator <  ( const EdgeKey & key ) const { return (m_s < key.m_s) || ( m_s == key.m_s && m_t < key.m_t ); };
	//!  != operator.
    /*!      
      \param key a EdgeKey to compare with this edgekey.
      \return the boolean result of the != comparison of two edgekeys.
    */
	bool operator != ( const EdgeKey & key ) const { return m_s != key.m_s  ||   m_t != key.m_t; };
	//!  <= operator.
    /*!      
      \param key a EdgeKey to compare with this edgekey.
      \return the boolean result of the <= comparison of two edgekeys.
    */
	bool operator <= ( const EdgeKey & key ) const { return *this < key || * this == key ;  };

	int s(){ return m_s;};
	int t(){ return m_t;};
	
private:

	//!  Edgekey source ID.
	int m_s;
	//!  Edgekey target ID.
	int m_t;

};


class Edge
{
public:
	Vertex * conjunction(Edge * e);
	double length();
	bool include_vertex(Vertex * v)
	{
		//updated by kewu zhang on April 1
		if(key.m_s == v->id () || key.m_t == v->id ())
			return true;
		return false;
	};
	Vertex * other_vertex(Vertex *v);
	//this function returns the two point
	//it is assumed that  this edge do have cross point with the given condition
	//va = esource()+b0(etarget-esource)/(etarget-esource).norm();
	//vb = esource()+b1(etarget-esource)/(etarget-esource).norm();
	void crosspoint(Point p, double radius, double &b0, double & b1);
	bool on_sphere(Point p, double radius);
	bool coface(Edge * e);
	bool coface(Vertex * v);
	Edge(){ m_halfedge[0] = NULL; m_halfedge[1] = NULL; m_trait = NULL; key.m_s = 0; key.m_t = 0; };
	Edge(int s, int e){ 		m_halfedge[0] = NULL;
		m_halfedge[1] = NULL;
		m_trait = NULL; 
		key = EdgeKey(s, e);
	};
	~Edge(){ };
	
	HalfEdge * & halfedge( int i ) { assert( 0<=i && i < 2 ); return m_halfedge[i];};
	Trait    * & trait()    { return m_trait;};
	bool		 boundary() { return (m_halfedge[0] == NULL && m_halfedge[1] != NULL ) || (m_halfedge[0] != NULL && m_halfedge[1] == NULL ); };
	std::string & string()  { return m_string;};

	HalfEdge * & other( HalfEdge * he ) { return (he != m_halfedge[0] )?m_halfedge[0]:m_halfedge[1]; };

    bool operator== (const Edge & e) const
    {
        return key == e.key;
    };

	EdgeKey & ekey(  ) 
    {
		return key;
    };

	EdgeRecord * record()
	{
		return m_record;
	};

    bool operator< (const Edge & e) const
    {
        return key < e.key;
    };

	int & vertex( int id ) { assert( id >= 0 && id < 2 ); return id==0? key.m_s : key.m_t; };

	void get_vertices(Vertex *&v1, Vertex *&v2);

private:

	HalfEdge  * m_halfedge[2];
	EdgeKey key;
	std::string m_string;   //string
	Trait	  * m_trait;
	EdgeRecord * m_record;
};

/*! Added by Sarah G for Mesh Simplification */
// An edge record keeps track of all the information about edges
// that we need while applying our mesh simplification algorithm.
class EdgeRecord {
	public:
		EdgeRecord( void ) {}
		EdgeRecord( Edge& _edge );
		// The second constructor takes an edge, and computes all
		// the essential data.  In particular, it computes the sum
		// of the quadrics at the two endpoints, and solves for the
		// optimal midpoint position as measured by this quadric.
		// It also stores the value of this quadric as the "score"
		// used by the priority queue.

		Edge * edge; // the edge referred to by this record

		Point optimalPoint; // the optimal point, if we were
		// to collapse this edge next

		double cost; // the cost associated with collapsing this edge,
		// which is very (very!) roughly something like
		// the distance we'll deviate from the original
		// surface if this edge is collapsed
};

/*! Added by Sarah G for Mesh Simplification */
inline bool operator<(const EdgeRecord& r1, const EdgeRecord& r2) {
	if (r1.cost != r2.cost) {
		return (r1.cost < r2.cost);
	}

	Edge * e1 = r1.edge;
	Edge * e2 = r2.edge;
	return &*e1 < &*e2;
}


}//name space MeshLib

#endif //_MESHLIB_EDGE_H_ defined

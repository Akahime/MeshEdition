#ifndef _OBJFILEREADER_H
#define _OBJFILEREADER_H

#include <fstream>
#include "Solid.h"
#include "Array.h"
#include "Point.h"
#include "iterators.h"

namespace MeshLib
{
	//!	 OBJFileReader.
	/*!	This class defines reading file in format obj/n
		and generating corresponding mesh file/n
	*/

	class OBJFileUtil
	{
	private:
		//!	vertex id
		int vID;
		//!	face id
		int fID;
		//!	texture array
		Array <MeshLib::Point> tArray;
		//!	normal array
		Array <MeshLib::Point> nArray;

	public:
		//!	 Constructor
		OBJFileUtil ();
		
		//!	Destructor
		~OBJFileUtil();

		//!	read input file, handle different contents
		void readToSolid( Solid *mesh, std::istream &in);

		//!	save mesh in .obj file
		void saveSolid(Solid *mesh, std::ofstream &os);

		//! update vertex by append texture, normal inf
		int modifyVertexInf(Solid *sol, char * str);

		//! modify normal information
		/*!
			read information from file, append to vertex string.
		*/
		void modifyNormalInf(Solid * sol, int vid, int nid);
		
		//! modify texture information
		/*!
			read texture information from file, append to vertex string
		*/
		void modifyTextureInf(Solid *sol, int vid, int tid);
	

		//how about w value, the format?
		////////////////////////////////
		//...............................

		//! double to string convertor
		std::string d2String( double value);


	};
}
#endif


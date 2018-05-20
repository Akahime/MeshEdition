#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>
#include <stdio.h>

#include "../Core/OBJFileUtil.h"
#include "MeshOperator.h"

using namespace MeshLib;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "USAGE: [.exe] [in.obj] [iteration]" << std::endl;
        return -1;
    }

    int iteration = atoi(argv[2]);

	// Read in the obj file
	Solid mesh;
	OBJFileUtil of;
	std::ifstream in(argv[1]);
	of.readToSolid(&mesh, in);

    std::cout<<argv[1]<<" , "<<argv[2]<<std::endl;
	
    //Make Operations
    MeshOperator mo;
    clock_t start = clock();
    for (unsigned i = 0; i < iteration; ++i)
    {
        std::cout << "Iteration " << i << std::endl;
        mo.LoopSubdivisionOneStep(&mesh);
    }
    clock_t end = clock();
    std::cout << "Time consumed: " << (double)(end - start) / (double)CLOCKS_PER_SEC << std::endl;

	// Write out the resultant obj file
    std::string fileName = argv[1];
	std::ofstream out((fileName.substr(0, fileName.rfind(".")) + ".loop."+std::to_string(iteration)+".obj").c_str());
	of.saveSolid(&mesh, out);
	return 0;
}

//
// Created by Sarah Gross on 5/19/18
//

#ifndef MESHEDITION_MESHOPERATOR_H
#define MESHEDITION_MESHOPERATOR_H

#endif //MESHEDITION_MESHOPERATOR_H

#include "../Core/Solid.h"

namespace MeshLib
{
    class MeshOperator
    {
    public:
        //!	 Constructor
        MeshOperator ();

        //!	Destructor
        ~MeshOperator();

        void LoopSubdivisionOneStep(Solid *mesh);
    };
}
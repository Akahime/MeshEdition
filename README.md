README
Author: Sarah Gross

# Overview

This project creates mesh operations on objects.

demo.gif shows a demonstration of the result.
![alt text](https://github.com/Akahime/OpenglIsland/blob/master/demo.gif)

# Building and running the Code

This project was originally built using the IDE CLion using CMakeLists.txt.

## Linux

Download Cmake :
https://askubuntu.com/questions/829310/how-to-upgrade-cmake-in-ubuntu/829311

Create a new folder "build" at the root :

    mkdir build
    cd build
    cmake ..
    make


To run the program, go to build folder and run :
    `./MeshEdition`

To visualize the .obj files, go can load them in:
https://3dviewer.net/


## Windows

Download Cmake (https://cmake.org/download/)
Follow this tutorial to build and run the code :
https://cmake.org/runningcmake/


# Source Files and Directory Structure


The code is in the src folder.


README              -- this file
CMakeLists          -- info for building the code
demo.mp4            -- video demonstration

src/                == Project-specific files.
    main            -- contains main function for project
Core                -- useful framework for mesh construction (CREDIT SNRAO 310 ON GITHUB)


Theory : 
Subdivision:
+ https://github.com/cmu462/Scotty3D/wiki/Developer-Manual-(MeshEdit) 
+ https://graphics.stanford.edu/~mdfisher/subdivision.html 
+ http://www.cs.cmu.edu/afs/cs/academic/class/15462-s14/www/lec_slides/Subdivision.pdf
Simplification:
+ https://classes.soe.ucsc.edu/cmps160/Spring05/finalpages/scyiu/
+ https://www.cs.cmu.edu/~./garland/Papers/quadrics.pdf
 
Code : https://github.com/snrao310/LoopSubdivition-for-HalfEdgeMesh-Library
Display obj : https://github.com/Hai-Lin/Subdivision
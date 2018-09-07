cmake_minimum_required(VERSION 3.6)
project(MeshEdition)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(. include src Core ${OPENGL_INCLUDE_DIR})

set(SOURCE_FILES
        src/main.cpp
        src/MeshOperator.cpp
        src/MeshOperator.h
        src/interactionManager.cpp
        src/interactionManager.h
        Core/Array.h
        Core/avltree.h
        Core/DList.h
        Core/Edge.cpp
        Core/Edge.h
        Core/EditTrait.h
        Core/Face.cpp
        Core/Face.h
        Core/FaceNormalTrait.h
        Core/FException.cpp
        Core/FException.h
        Core/HalfEdge.cpp
        Core/HalfEdge.h
        Core/iterators.h
        Core/OBJFileUtil.cpp
        Core/OBJFileUtil.h
        Core/Parser.cpp
        Core/Parser.h
        Core/Point.cpp
        Core/Point.h
        Core/SList.h
        Core/Solid.cpp
        Core/Solid.h
        Core/SolidDelegate.cpp
        Core/SolidDelegate.h
        Core/string_token_iterator.h
        Core/StringTokenizer.cpp
        Core/StringTokenizer.h
        Core/TopologyException.cpp
        Core/TopologyException.h
        Core/Trait.cpp
        Core/Trait.h
        Core/Vertex.cpp
        Core/Vertex.h
        Core/mutablePriorityQueue.h)

add_executable(MeshEdition ${SOURCE_FILES})
target_link_libraries(MeshEdition ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
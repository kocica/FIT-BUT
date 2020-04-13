/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    5/12/2019
 **/

#include <iostream>
#include <math.h>
#include <limits>

#include "tree_mesh_builder.h"

TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree")
{

}

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field)
{
    unsigned totalTriangles{0};

    // 1. Create cube using recursive function
    #pragma omp parallel
    {
        #pragma omp single //nowait
        {
            totalTriangles = marchCubesRec(field, Vec3_t<float>{0, 0, 0}, mGridSize);
        }
    }

    // 2. Return total number of triangles generated.
    return totalTriangles;
}

unsigned TreeMeshBuilder::marchCubesRec(const ParametricScalarField &field, Vec3_t<float> cubeOffset, size_t gridSize)
{
    // Temporary array of results - one for each executed task, so we can avoid locks (critic section)
    std::valarray<unsigned> res{ 0, 0, 0, 0, 0, 0, 0, 0 };

    // 1. Check whether there might be isosurface
    CubeCornerVerts_t cubeCorners;
    transformCubeVertices(cubeOffset, sc_vertexNormPos, cubeCorners);

    Vec3_t<float> midReal{cubeCorners[0].x + (gridSize/2.) * mGridResolution,
                          cubeCorners[0].y + (gridSize/2.) * mGridResolution,
                          cubeCorners[0].z + (gridSize/2.) * mGridResolution};

    const float formulaLhs = evaluateFieldAt(midReal, field);
    const float formulaRhs = mIsoLevel + ((sqrt(3)/2.) * gridSize * mGridResolution);

    if(formulaLhs > formulaRhs)
    {
        return 0;
    }

    // 2. In case we reach cutoff, create cube and return number of triangles
    if((gridSize <= 1) || (gridSize == minGridEdgeSize))
    {
        return buildCube(cubeOffset, field);
    }

    // 3. Update actual grid edge size to half
    gridSize = gridSize / 2.;

    // 4. Exceute task for each of 8 (octree) childs
    
    #pragma omp task shared(res, field) //firstprivate(gridSize) final(gridSize <= 1)
    {
        res[0] = marchCubesRec(field, cubeOffset, gridSize);
    }

    #pragma omp task shared(res, field) //firstprivate(gridSize) final(gridSize <= 1)
    {
        cubeOffset.x += gridSize;
        res[1] = marchCubesRec(field, cubeOffset, gridSize);
        cubeOffset.x -= gridSize;
    }

    #pragma omp task shared(res, field) //firstprivate(gridSize) final(gridSize <= 1)
    {
        cubeOffset.y += gridSize;
        res[2] = marchCubesRec(field, cubeOffset, gridSize);
        cubeOffset.y -= gridSize;
    }

    #pragma omp task shared(res, field) //firstprivate(gridSize) final(gridSize <= 1)
    {
        cubeOffset.x += gridSize;
        cubeOffset.y += gridSize;
        res[3] = marchCubesRec(field, cubeOffset, gridSize);
        cubeOffset.y -= gridSize;
        cubeOffset.x -= gridSize;
    }

    // NOTE: Update Z-axis outside of private block, so its propagated to rest of the tasks
    cubeOffset.z += gridSize;

    #pragma omp task shared(res, field) //firstprivate(gridSize) final(gridSize <= 1)
    {
        res[4] = marchCubesRec(field, cubeOffset, gridSize);
    }

    #pragma omp task shared(res, field) //firstprivate(gridSize) final(gridSize <= 1)
    {
        cubeOffset.x += gridSize;
        res[5] = marchCubesRec(field, cubeOffset, gridSize);
        cubeOffset.x -= gridSize;
    }

    #pragma omp task shared(res, field) //firstprivate(gridSize) final(gridSize <= 1)
    {
        cubeOffset.y += gridSize;
        res[6] = marchCubesRec(field, cubeOffset, gridSize);
        cubeOffset.y -= gridSize;
    }

    #pragma omp task shared(res, field) //firstprivate(gridSize) final(gridSize <= 1)
    {
        cubeOffset.x += gridSize;
        cubeOffset.y += gridSize;
        res[7] = marchCubesRec(field, cubeOffset, gridSize);
    }

    // 5. Wait for all executed tasks and summarize results from each task
    #pragma omp taskwait
    {
        return res.sum();
    }
}

float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field)
{
    // NOTE: This method is called from "buildCube(...)"!

    // 1. Store pointer to and number of 3D points in the field
    //    (to avoid "data()" and "size()" call in the loop).
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());

    float value = std::numeric_limits<float>::max();

    // 2. Find minimum square distance from points "pos" to any point in the
    //    field.
    for(unsigned i = 0; i < count; ++i)
    {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

        // Comparing squares instead of real distance to avoid unnecessary
        // "sqrt"s in the loop.
        value = std::min(value, distanceSquared);
    }

    // 3. Finally take square root of the minimal square distance to get the real distance
    return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const Triangle_t &triangle)
{
    // NOTE: This method is called from "buildCube(...)"!

    // Store generated triangle into vector (array) of generated triangles.
    // The pointer to data in this array is return by "getTrianglesArray(...)" call
    // after "marchCubes(...)" call ends.
    #pragma omp critical
    {
        mTriangles.push_back(triangle);
    }
}

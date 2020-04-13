/**
 * @file    loop_mesh_builder.h
 *
 * @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP loops
 *
 * @date    5/12/2019
 **/

#ifndef LOOP_MESH_BUILDER_H
#define LOOP_MESH_BUILDER_H

#include <vector>
#include "base_mesh_builder.h"

class LoopMeshBuilder : public BaseMeshBuilder
{
public:
    LoopMeshBuilder(unsigned gridEdgeSize);

protected:
    std::vector<Triangle_t> mTriangles;


    unsigned marchCubes(const ParametricScalarField &field);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const { return mTriangles.data(); }
};

#endif // LOOP_MESH_BUILDER_H

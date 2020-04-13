/**
 * @file    tree_mesh_builder.h
 *
 * @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    5/12/2019
 **/

#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H

#include "base_mesh_builder.h"
#include <valarray>

class TreeMeshBuilder : public BaseMeshBuilder
{
public:
    TreeMeshBuilder(unsigned gridEdgeSize);

protected:
    std::vector<Triangle_t> mTriangles;

    /// Minimum grid edge size, i.e. cutoff
    size_t minGridEdgeSize = 0;

    unsigned marchCubes(const ParametricScalarField &field);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const { return mTriangles.data(); }

private:
    unsigned marchCubesRec(const ParametricScalarField &field, Vec3_t<float> cubeOffset, size_t gridSize);
};

#endif // TREE_MESH_BUILDER_H

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "core/graphics/mesh.h"
#include "core/resources/rawmesh.h"

class PrimitiveMesh : public RawMesh
{
public:
    PrimitiveMesh(QString name);
    virtual void load();
    virtual bool unload();

    static PrimitiveMesh* buildCube();
    static PrimitiveMesh* buildSphere(int rings, int segments);
    static PrimitiveMesh* buildPlane();
};


#endif //PRIMITIVES_H

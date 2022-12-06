#include "DataStructs.h"

Vertex::Vertex(float x, float y, float z, Mesh *m) : x(x), y(y), z(z), id(m->numVertices) { m->addVertex(this); }

Vertex::Vertex(float x, float y, float z) {
    this->x=x;
    this->y=y;
    this->z=z;
}

int Vertex::getId() { return id; }

void Vertex::move(float dx, float dy, float dz) {
    x += dx;
    y += dy;
    z += dz;
}

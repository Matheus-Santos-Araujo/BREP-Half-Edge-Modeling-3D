#include "DataStructs.h"

Mesh::Mesh(int id)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   : id(id), initialized(false) {
    numFaces = 0;
    numVertices = 0;
    numLoops = 0;
    numEdges = 0;

    tx = ty = tz = 0.6;
    sx = sy = sz = 2;
    
    //Quaternion identidade
    orientation = Quat(0.0, 0.0, 0.0, 1.0);
}

Mesh::~Mesh() {
    std::list<Vertex*>::iterator vIter;
    for(vIter = vertices.begin(); vIter != vertices.end(); vIter++) {
        delete *vIter;
    }
    vertices.clear();

    std::list<Edge*>::iterator eIter;
    for(eIter = edges.begin(); eIter != edges.end(); eIter++) {
        delete *eIter;
    }
    edges.clear();
    
    std::list<Face*>::iterator fIter;
    for(fIter = faces.begin(); fIter != faces.end(); fIter++) {
        delete *fIter;
    }
    faces.clear();
}

int Mesh::getId() {
    return id;
}

void Mesh::addVertex(Vertex *v) {
    vertices.push_back(v);
    if(!initialized) {
        initialized = true;
    }
    numVertices++;
}

void Mesh::addFace(Face *f) {
    faces.push_back(f);
    numFaces++;
}

void Mesh::addEdge(Edge *e) {
    edges.push_back(e);
    numEdges++;
}

// Operações geométricas
void Mesh::translate(float dx, float dy, float dz) {
    tx += dx;
    ty += dy;
    tz += dz;
}

void Mesh::scale(float dx, float dy, float dz) {
    sx = dx;
    sy = dy;
    sz = dz;
}

void Mesh::rotate(float angle, float x, float y, float z) {
    Quat q;
    q.fromAxisAngle(Vec3(x, y, z), angle);
    orientation = orientation*q;
}

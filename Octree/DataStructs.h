#pragma once
#include "vectorops.h"
#include <vector>

class HalfEdge;
class Mesh;

class Vertex {
public:
    Vertex(float x, float y, float z, Mesh* m);
    Vertex(float x, float y, float z);

    int getId();
    void move(float dx, float dy, float dz);

    void setPosition(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }

    int id;
    float x, y, z;
    //Ponteiro para a half edge que sai desse vértice
    HalfEdge* hed;
};

class Mesh;

class Edge {
public:
    Edge(Mesh* m);
    ~Edge();
    int id;
    HalfEdge* hed1, * hed2;
};

class Edge;
class Loop;

enum HEd { PLUS, MINUS };

class HalfEdge {
public:
    HalfEdge();
    ~HalfEdge();

    HalfEdge* mate();

    Vertex* origin;
    Loop* loop;
    Edge* edge;
    HalfEdge* prox, * ant;
};

HalfEdge* addhe(Edge* e, Vertex* v, HalfEdge* h, HEd sign);
HalfEdge* delhe(HalfEdge* he);

class Face;

class Loop {
public:
    Loop(Face* f);
    void setNormal();
    Vec3 normal;
    int id;
    HalfEdge* hed;
    Face* face;
};

#include <list>

class Mesh {
public:
    Mesh(int id);
    ~Mesh();
    int getId();

    void addVertex(Vertex* v);
    void addFace(Face* f);
    void addEdge(Edge* e);

    //Op. Geomtétricas
    void translate(float dx, float dy, float dz);
    void scale(float dx, float dy, float dz);
    void rotate(float angle, float x, float y, float z);

    int numFaces;
    int numVertices;
    int numLoops;
    int numEdges;
    std::list<Vertex*> vertices;
    std::list<Face*> faces;
    std::list<Edge*> edges;

    int id;
    bool initialized;

    //Informações sobre as operações geométricas
    float tx, ty, tz;
    float sx, sy, sz;
    Quat orientation;
};

#include <list>
class Mesh;

class Face {
public:
    Face(Mesh* m);
    ~Face();
    void addLoop(Loop* l);

    int getId();

    int id;

    std::list<Loop*> loops;
    //Loop exterior, que define a face
    Loop* outter;
    Mesh* solid;
};

#include <list>

class BREP {
public:
    BREP() { qtd = 0; }
    ~BREP() { this->clear(); }

    void clear() {
        std::list<Mesh*>::iterator iter;
        for (iter = meshes.begin(); iter != meshes.end(); iter++) {
            delete* iter;
        }
        meshes.clear();
        qtd = 0;
    }

    void paint() {
        std::list<Mesh*>::iterator mIter;

        // Faces
        for (mIter = meshes.begin(); mIter != meshes.end(); mIter++) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPushMatrix();
            float m[16];
            (*mIter)->orientation.toMatrix(m);
            glMultMatrixf(m);
            glScalef((*mIter)->sx, (*mIter)->sy, (*mIter)->sz);
            glTranslatef((*mIter)->tx, (*mIter)->ty, (*mIter)->tz);
            std::list<Face*>::iterator fIter;

            glPushName((*mIter)->id);
            glColor4f(0.0, 0.9, 0.9, 0.5);
            fIter = (*mIter)->faces.begin();
            fIter++;
            for (; fIter != (*mIter)->faces.end(); fIter++) {
                std::list<Loop*>::iterator lIter;
                for (lIter = (*fIter)->loops.begin(); lIter != (*fIter)->loops.end(); lIter++) {
                    HalfEdge* he = (*lIter)->hed;
                    glPushName((*lIter)->id);
                    glColor4f(0.0, 0.9, 0.9, 0.5);
                    glNormal3f((*lIter)->normal.x, (*lIter)->normal.y, (*lIter)->normal.z);
                    glBegin(GL_POLYGON);
                    do {
                        Vertex* vert = he->origin;
                        glVertex3f(vert->x, vert->y, vert->z);
                    } while ((he = he->prox) != (*lIter)->hed);
                    glEnd();
                    glPopName();
                }
            }
            glPopName();
            glPopMatrix();
        }

        // Arestas
        for (mIter = meshes.begin(); mIter != meshes.end(); mIter++) {
            glPushMatrix();
            float m[16];
            (*mIter)->orientation.toMatrix(m);
            glMultMatrixf(m);
            glScalef((*mIter)->sx, (*mIter)->sy, (*mIter)->sz);
            glTranslatef((*mIter)->tx, (*mIter)->ty, (*mIter)->tz);
            std::list<Edge*>::iterator eIter;
            glPushName((*mIter)->id);
            for (eIter = (*mIter)->edges.begin(); eIter != (*mIter)->edges.end(); eIter++) {
                glColor4f(0.0, 1.0, 0.0, 1.0);
                glPushName((*eIter)->id);
                glBegin(GL_LINES);
                glVertex3f((*eIter)->hed1->origin->x, (*eIter)->hed1->origin->y, (*eIter)->hed1->origin->z);
                glVertex3f((*eIter)->hed2->origin->x, (*eIter)->hed2->origin->y, (*eIter)->hed2->origin->z);
                glEnd();
                glPopName();
            }
            glPopName();
            glPopMatrix();
        }
    }

    bool isEmpty() { return meshes.empty(); }

    Mesh* getSolid(int idSolid) {
        Mesh* m;
        std::list<Mesh*>::iterator iter;
        for (iter = meshes.begin(); iter != meshes.end(); iter++)
            if ((*iter)->getId() == idSolid) {
                m = *iter;
                return m;
            }
        return NULL;
    }

    Face* getFace(Mesh* m, int idFace) {
        Face* f;
        std::list<Face*>::iterator iter;
        for (iter = m->faces.begin(); iter != m->faces.end(); iter++)
            if ((*iter)->getId() == idFace) {
                f = *iter;
                return f;
            }
        return NULL;
    }

    HalfEdge* getHEd(Face* f, int idVertex1, int idVertex2) {
        HalfEdge* h;
        std::list<Loop*>::iterator iter;
        for (iter = f->loops.begin(); iter != f->loops.end(); iter++)
        {
            h = (*iter)->hed;
            do {
                if (h->origin->getId() == idVertex1 &&
                    h->prox->origin->getId() == idVertex2)
                    return h;
            } while ((h = h->prox) != (*iter)->hed);
        }
        return NULL;
    }

    Loop* getLoop(int solidId, int id) {
        Mesh* m = getSolid(solidId);
        std::list<Face*>::iterator fIter;
        for (fIter = m->faces.begin(); fIter != m->faces.end(); fIter++) {
            std::list<Loop*>::iterator lIter;
            for (lIter = (*fIter)->loops.begin(); lIter != (*fIter)->loops.end(); lIter++)
                if ((*lIter)->id == id)
                    return *lIter;
        }
        return NULL;
    }

    Vertex* getVertex(int solidId, int id) {
        Mesh* m = getSolid(solidId);
        std::list<Vertex*>::iterator vIter;
        for (vIter = m->vertices.begin(); vIter != m->vertices.end(); vIter++)
            if ((*vIter)->id == id)
                return *vIter;
        return NULL;
    }

    Edge* getEdge(int solidId, int id) {
        Mesh* m = getSolid(solidId);
        std::list<Edge*>::iterator eIter;
        for (eIter = m->edges.begin(); eIter != m->edges.end(); eIter++)
            if ((*eIter)->id == id)
                return *eIter;
        return NULL;
    }

    HalfEdge* getHEd(Face* f, int idVertex1) {
        HalfEdge* h;
        std::list<Loop*>::iterator iter;
        for (iter = f->loops.begin(); iter != f->loops.end(); iter++)
        {
            h = (*iter)->hed;
            do {
                if (h->origin->getId() == idVertex1)
                    return h;
            } while ((h = h->prox) != (*iter)->hed);
        }
        return NULL;
    }

    void mvfs(float x, float y, float z);
    bool mev(HalfEdge* he1, HalfEdge* he2, int idSolid, int idFace, int idVertex1, float x, float y, float z);
    bool mef(HalfEdge* h1, HalfEdge* h2, int idSolid, int idFace, int idVertex1, int idVertex2);
    void varr(int idSolid, int idFace, float dx, float dy, float dz);
    void rvarr(int idSolid, int idFace, int disc, float angle);
    
    void Cube(float minX, float minY, float minZ, float size);
    void Cylinder(float pX, float pY, float pZ, float radius, float height, int disc);
    void Sphere(float pX, float pY, float pZ, float radius, int disc);
    void Tema();

    std::list<Mesh*> meshes;
    int qtd;
};

class Bezier {
public:
    Bezier(Point3D p2, Point3D p3);
    std::vector<Point3D>& getPoints() { return points; }
    Point3D calc(double u);
    void paint();
    
    std::vector<Point3D> points;
    int order;
};
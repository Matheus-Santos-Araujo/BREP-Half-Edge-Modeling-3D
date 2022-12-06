#include "DataStructs.h"
#include <iostream>
#include "vectorops.h"
#include <cmath>
#define M_PI 3.14

void BREP::mvfs(float x, float y, float z) {
    // Cria objetos
    Mesh *m;
    Face *f;
    Loop *l;
    Vertex *v;
    HalfEdge *he;
    // Inicializa objetos com parâmetros de entrada
    m = new Mesh(qtd++);
    f = new Face(m);
    l = new Loop(f);
    v = new Vertex(x, y, z, m);
    he = new HalfEdge;

    f->outter = l;
    l->hed = he;
    he->loop = l;
    he->prox = he->ant = he;
    he->origin = v;
    he->edge = NULL;
    meshes.push_back(m);
}

bool BREP::mev(HalfEdge* he1, HalfEdge* he2, int idSolid, int idFace, int idVertex1, float x, float y, float z) {
    if (he1 == NULL || he2 == NULL) {
        // Se forem novas half edges as instância e inicializa
        Mesh* s;
        HalfEdge* he;
        Face* fn;

        if ((s = getSolid(idSolid)) == NULL)
            return false;
        if ((fn = getFace(s, idFace)) == NULL)
            return false;
        if ((he = getHEd(fn, idVertex1)) == NULL)
            return false;

         he1 = he;
         he2 = he;
    }

    // Usa o solid p/ inicializar 
    HalfEdge* henew;
    Face* f = he1->loop->face;
    Vertex* v = new Vertex(x, y, z, he1->loop->face->solid);
    Edge* e = new Edge(he1->loop->face->solid);

    // Busca a prox half edge
    henew = he1;
    while (he1 != he2) {
        henew->origin = v;
        henew = henew->mate()->prox;
    }

    // Add novas half edges com sinais em ordem
    addhe(e, he2->origin, he1, PLUS);
    addhe(e, v, he2, MINUS);

    // Atualiza lista
    v->hed = he2->ant;
    he2->origin->hed = he2;
    return true;
}

bool BREP::mef(HalfEdge* h1, HalfEdge* h2, int idSolid, int idFace, int idVertex1, int idVertex2) {
    if (h1 == NULL || h2 == NULL) {
        // Se forem novas half edges as instância e inicializa
        Mesh* s;
        Face* f;
        HalfEdge* he1, * he2;

        if ((s = getSolid(idSolid)) == NULL)
            return false;
        if ((f = getFace(s, idFace)) == NULL)
            return false;
        if ((he1 = getHEd(f, idVertex1)) == NULL)
            return false;

        he2 = he1;
        if (he2->origin->getId() != idVertex2)
            do {
                he2 = he2->prox;
                if (he2 == he1)
                    return false;
            } while (he2->origin->getId() != idVertex2);

            h1 = he1;
            h2 = he2;
    }
        Face *f = new Face(h1->loop->face->solid);
        Loop* l;
        Edge* e;
        HalfEdge* he, * nhe1, * nhe2, * temp;

        // Usa o solid p/ inicializar 
        f = new Face(h1->loop->face->solid);
        l = new Loop(f);
        e = new Edge(h1->loop->face->solid);
        f->outter = l;

        // Busca a prox half edge
        he = h1;
        while (he != h2) {
            he->loop = l;
            he = he->prox;
        }

        // Add novas half edges com sinais em ordem
        nhe1 = addhe(e, h2->origin, h1, PLUS);
        nhe2 = addhe(e, h1->origin, h2, MINUS);

        // Atualiza lista
        nhe1->ant->prox = nhe2;
        nhe2->ant->prox = nhe1;
        temp = nhe1->ant;
        nhe1->ant = nhe2->ant;
        nhe2->ant = temp;

        l->hed = nhe1;
        h2->loop->hed = nhe2;
        h2->loop->setNormal();
        // Define a normal do loop
        l->setNormal();
    return true;
}

// Deslocamento (ampliação do sólido)
void BREP::varr(int idSolid, int idFace, float dx, float dy, float dz) {
    // Pega o sólido e a face
    Mesh *m = getSolid(idSolid);
    Face *f = getFace(m, idFace);
    // Inicializa a half edge inicial e a half edge e o vertice que varrerá o sólido
    HalfEdge *first, *current;
    Vertex *v;
    
    // Percorrendo todos os loops
    std::list<Loop*>::iterator lIter;
    for(lIter = f->loops.begin(); lIter != f->loops.end(); lIter++) {
        // Pega primeiro
        first = (*lIter)->hed;
        // Pega próximo
        current = first->prox;
        // Pega vertice de origem do proximo
        v = current->origin;
        // Pega a half edge atual e desloca os pontos
        mev(current, current, NULL, NULL, NULL, v->x + dx, v->y + dy, v->z + dz);
        // Enquanto first ñ chegar ao inicial
        while(current != first) {
            // Pega vertice de origem do proximo
            v = current->prox->origin;
            // Pega a proxima half edge e desloca os pontos
            mev(current->prox, current->prox, NULL, NULL, NULL, v->x + dx, v->y + dy, v->z + dz);
            // Cria a face
            mef(current->ant, current->prox->prox, NULL, NULL, NULL, NULL);
            current = current->prox->mate()->prox;
        }
        mef(current->ant, current->prox->prox, NULL, NULL, NULL, NULL);

    }
}

// Deslocamento (ampliação esférica)
void BREP::rvarr(int idSolid, int idFace, int disc, float angle) {
    // Inicializa half edges utilitárias
    HalfEdge *first, *cfirst, *last, *current = NULL;
    // Pega o sólido e a face
    Mesh *m = getSolid(idSolid);
    Face *f = getFace(m, idFace);
    // Inicializa quaternion de rotação
    Quat q;
    q.fromAxisAngle(Vec3(1.0, 0.0, 0.0), angle/(float)disc);
    // Lista de loops
    std::list<Loop*>::iterator lIter = f->loops.begin();
    first = (*lIter)->hed;
    // Busca aresta inicial e final
    while(first->edge != first->prox->edge) 
        first = first->prox;
    last = first->prox;
    while(last->edge != last->prox->edge)
        last = last->prox;
    // Laço rotação p/ gerar a esfera
    cfirst = first;
    while(disc--) {
        Vec3 vcoord(cfirst->prox->prox->origin->x, cfirst->prox->prox->origin->y, cfirst->prox->prox->origin->z);
        Vec3 v = rotateVec(vcoord, q);
        mev(cfirst->prox, cfirst->prox, NULL, NULL, NULL, v.x, v.y, v.z);
        current = cfirst->prox;
        while(current != last->prox) {
            vcoord = Vec3(current->ant->origin->x, current->ant->origin->y, current->ant->origin->z);
            v = rotateVec(vcoord, q);
            mev(current->ant, current->ant, NULL, NULL, NULL, v.x, v.y, v.z);
            mef(current->ant->ant, current->prox, NULL, NULL, NULL, NULL);
            current = current->prox->prox->mate();
        }
        last = current;
        cfirst = cfirst->prox->prox->mate();
    }
    while(cfirst != current) {
        mef(cfirst, cfirst->prox->prox->prox, NULL, NULL, NULL, NULL);
        cfirst = cfirst->ant->mate()->ant;
    }
}
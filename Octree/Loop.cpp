#include "DataStructs.h"

// Novo loop em uma face
Loop::Loop(Face *f) {
    face = f;
    face->addLoop(this);

    this->id = f->solid->numLoops++;
    }

// Define o vetor normal ao loop
void Loop::setNormal() {
    // Calcula vetor da direção inicial
    HalfEdge *h = this->hed;
    HalfEdge *h2 = h->prox;
    Vec3 v(h2->origin->x - h->origin->x, h2->origin->y - h->origin->y, h2->origin->z - h->origin->z);

    // Percorre half edges
    while((h = h->prox) != this->hed) {
        h2 = h->prox;
        Vec3 u(h2->origin->x - h->origin->x, h2->origin->y - h->origin->y, h2->origin->z - h->origin->z);
        //Verifica se dois vetores são colineares
        if(dot(u, v) == u.length()*v.length())
            continue;
        else {
            this->normal = normalize(cross(v, u));
            break;
        }
    }
}


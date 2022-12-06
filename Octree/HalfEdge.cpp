#include "DataStructs.h"

HalfEdge::HalfEdge() {}

// Nova Half Edge
HalfEdge* addhe(Edge *e, Vertex *v, HalfEdge *h, HEd sign) {
    HalfEdge *he;
    if (h->edge == 0)
        he = h;
    else {
        he = new HalfEdge;
        h->ant->prox = he;
        he->ant = h->ant;
        h->ant = he;
        he->prox = h;
    }

    he->edge = e;
    he->origin = v;
    he->loop = h->loop;
    if(sign == PLUS)
        e->hed1 = he;
    else 
        e->hed2 = he;

    return he;
}

HalfEdge::~HalfEdge() {}

// Retorna HE da outra ponta
HalfEdge* HalfEdge::mate() { return (this == edge->hed1) ? this->edge->hed2 : this->edge->hed1; }

// Deleta Half Edge
HalfEdge* delhe(HalfEdge *he) {
    if(he->edge == 0) {
        delete he;
    } else if (he->prox == he) {
        he->edge = 0;
        return he;
    }
    else {
        he->ant->prox = he->prox;
        he->prox->ant = he->ant;
        delete he;
        return he->ant;
    }
}


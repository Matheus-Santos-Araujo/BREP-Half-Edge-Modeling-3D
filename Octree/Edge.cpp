#include "DataStructs.h"

Edge::Edge(Mesh *m) : id(m->numEdges)  { m->addEdge(this); }
Edge::~Edge() { delete hed1; delete hed2; }

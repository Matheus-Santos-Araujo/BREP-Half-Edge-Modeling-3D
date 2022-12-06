#include "DataStructs.h"
#include "vectorops.h"
#include <iostream>

void BREP::Cube(float minX, float minY, float minZ, float size) {
    // Inicialização: 1 ponto e aresta
    mvfs(minX, minY, minZ + size);
    // 2 ponto e aresta
    mev(NULL, NULL, qtd - 1, 0, 0, minX + size, minY, minZ + size);
    // 2 ponto e aresta
    mev(NULL, NULL, qtd - 1, 0, 1, minX + size, minY + size, minZ + size);
    // 3 ponto e aresta
    mev(NULL, NULL, qtd - 1, 0, 2, minX, minY + size, minZ + size);
    // Face
    mef(NULL, NULL, qtd - 1, 0, 3, 0);
    // Deslocamento negativo no eixo z
    varr(qtd - 1, 0, 0, 0, -size);

    return;
}

void BREP::Cylinder(float pX, float pY, float pZ, float radius, float height, int disc) {
    int num = 0;
    // Inicialização: 1 ponto e aresta
    mvfs(radius * cos(0) + pX, pY, radius * sin(0) + pZ);

    for (float i = 2.0 * M_PI / (float)disc; i <= 2.0 * M_PI; i += 2.0 * M_PI / (float)disc) {
        if (i + 2.0 * M_PI / (float)disc >= 2.0 * M_PI) {
            // Ponto e aresta gerados pela circuferencia
            mev(NULL, NULL, qtd - 1, 0, num, radius * cos(i) + pX, pY, radius * sin(i) + pZ);
            // Aresta e face gerados pela esfera
            mef(NULL, NULL, qtd - 1, 0, num, 0);
            num++;
        } else {
            // Ponto e aresta gerados pela circuferencia
            mev(NULL, NULL, qtd - 1, 0, num, radius * cos(i) + pX, pY, radius * sin(i) + pZ);
            num++;
        }
    }

    // Deslocamento positivo no eixo y
    varr(qtd - 1, 0, 0.0, height, 0.0);
}

void BREP::Sphere(float pX, float pY, float pZ, float radius, int disc) {
    float step = M_PI / disc;
    // Inicialização: 1 ponto e aresta
    mvfs(-1.0 * radius, 0, 0);
    for (int i = 0, idNum = 0; i < disc; i++, idNum++) {
        float angle = M_PI - step * (i + 1);
        // Ponto e aresta gerados pela circuferencia
        mev(NULL, NULL, qtd - 1, 0, idNum, radius * cos(angle), radius * sin(angle), 0.0);
    }
    // Deslocamento positivo com angulo 2pi e 
    rvarr(qtd - 1, 0, 2 * disc, 2 * M_PI);
}

void BREP::Tema() {
    float radius = 4.0;
    int disc = 20;
    float step = M_PI / disc;

    //Semi-esfera
    mvfs(-1.0 * radius, 0, 0);
    for (int i = 0, idNum = 0; i < disc; i++, idNum++) {
        float angle = M_PI - step * (i + 1);
        mev(NULL, NULL, qtd - 1, 0, idNum, radius * cos(angle), radius * sin(angle), 0.0);
    }
    rvarr(qtd - 1, 0, 2 * disc, M_PI);

    //Cilindro central
    mvfs(radius, 0, 0);
    int idNum = 0;
    for (int i = 0, idNum = 0; i < 2 * disc; i++, idNum++) {
        float angle = step * (i + 1);
        mev(NULL, NULL, qtd - 1, 0, idNum, radius * cos(angle), radius * sin(angle), 0.0);
    }
    mef(NULL, NULL, qtd - 1, 0, idNum, 0);
    varr(qtd - 1, 0, 0, 0, -8);

    //Cubo central
    Cube(-0.5, -3.0, -9, 1);

    //Cilindro direita
    radius = 1.5;
    mvfs(radius + 6, 0, 0);
    for (int i = 0, idNum = 0; i < 2 * disc; i++, idNum++) {
        float angle = step * (i + 1);
        mev(NULL, NULL, qtd - 1, 0, idNum, radius * cos(angle) + 6, radius * sin(angle), 0.0);
    }
    mef(NULL, NULL, qtd - 1, 0, idNum, 0);
    varr(qtd - 1, 0, 0, 0, -8);

    //Cubo direita baixo
    Cube(5.5, -1.0, -9, 1);

    //Cilindro esquerda
    mvfs(radius - 6, 0, 0);
    for (int i = 0, idNum = 0; i < 2 * disc; i++, idNum++) {
        float angle = step * (i + 1);
        mev(NULL, NULL, qtd - 1, 0, idNum, radius * cos(angle) - 6, radius * sin(angle), 0.0);
    }
    mef(NULL, NULL, qtd - 1, 0, idNum, 0);
    varr(qtd - 1, 0, 0, 0, -8);

    //Cubo esquerda baixo
    Cube(-6.5, -1.0, -9, 1);

    //Cubo direita cima
    Cube(4.0, -0.75, -3.0, 1.5);

    //Cubo esquerda cima
    Cube(-4.75, -0.75, -3, 1);
}

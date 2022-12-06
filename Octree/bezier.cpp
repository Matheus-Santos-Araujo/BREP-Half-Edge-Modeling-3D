#include "DataStructs.h"
#include "cmath"
#include "vectorops.h"

// Pontos de controle
Bezier::Bezier(Point3D p2, Point3D p3) {
    Point3D p1 = Point3D(5.0, 0.0, 2.0);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    Point3D p4 = Point3D(-10, 0.0, 0.0);
    points.push_back(p4);
    order = 3;
}

static int factorial(int x) {
    if (x == 0) return 1;

    int value = x;

    for (int i = 1; i < x; i++) { value *= i; }

    return value;
}

// Calculo de novos pontos com interpolação de polinômio
Point3D Bezier::calc(double u) {
    std::vector<Point3D> pts = points;
    // Escala dos pontos para coeficiente binomial
    for (int i = 0; i <= order; i++) {
        double x = (factorial(order) / (factorial(i) * factorial(order - i))) * pow(u, i) * pow(1.0 - u, order - i);

        pts[i].esc(x);
    }

    // Soma pelo anterior (pesos)
    for (int x = 1; x <= order; x++) { pts[x].sum(pts[x - 1]); }

    // Retorno do ponto interpolado (ultimo após escala e soma)
    return pts[order];
}

void Bezier::paint() {
    if (points.size() == 0)
        return;

    // Pontos
    glColor3f(1, 0, 0);
    glPointSize(10);
    glBegin(GL_POINTS);
    for (int x = 0; x <= order; x++) {
        glVertex3f(points[x].getX(), points[x].getY(), points[x].getZ());
    }
    glEnd();

    // Linhas retas entre pontos
    glLineWidth(5.0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(4, 0x5555);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    for (int x = 1; x <= order; x++) {
        glVertex3f(points[x].getX(), points[x].getY(), points[x].getZ());
        glVertex3f(points[x - 1].getX(), points[x - 1].getY(), points[x - 1].getZ());
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    // Curva
    Point3D p, p2;
    double v = 0.05;
    // Gera os pontos a cada passo e conecta a linha
    for (double u = 0.0; u < 1.0; u += v) {
        p = calc(u);
        p2 = calc(u + v);

        glColor3f(0, 0.9, 0.9);
        glLineWidth(5.0);

        glBegin(GL_LINES);
        glVertex3f(p.getX(), p.getY(), p.getZ());
        glVertex3f(p2.getX(), p2.getY(), p2.getZ());
        glEnd();
    }

}
#pragma once

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <Windows.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "vectorops.h"
#include "DataStructs.h"

BREP brep;
Bezier curve(Point3D {0, -5, 0 }, Point3D { 0, 5, 0 });
boolean c = false;

struct Camera {
	point eye, lookat, up;

	Camera(const point& _position, const point& _view, const point& _up) : eye(_position), lookat(_view), up(_up) {
		up.norm();
	}
};

Camera cam = Camera({ 1.0f, -10.0f, 20.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
GLfloat light[4] = { 10.0f, 10.0f, 10.0f, 1.0f };

void disp(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z, cam.lookat.x, cam.lookat.y, cam.lookat.z, cam.up.x, cam.up.y, cam.up.z);

	brep.paint();
	if (c == true) {
		curve.paint();
	}
	glPushMatrix();
	glTranslatef(light[0], light[1], light[2]);
	glutWireSphere(0.2, 20, 16);
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 4.0f, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'E':
	case 'e':
		c = false;
		brep.clear();
		brep.Sphere(1, 1, 1, 5, 10);
		break;
	case 'B':
	case 'b':
		c = false;
		brep.clear();
		brep.Cube(-3, -3, -3, 5);
		break;
	case 'C':
	case 'c':
		c = false;
		brep.clear();
		brep.Cylinder(0, -5, -2, 5, 10, 20);
		break;
	case 'T':
	case 't':
		c = false;
		brep.clear();
		brep.Tema();
		break;
	case 'I':
	case 'i':
		c = false;
		brep.getSolid(0)->tx += 0.2;
		brep.getSolid(0)->ty += 0.2;
		brep.getSolid(0)->tz += 0.2;
		break;
	case 'K':
	case 'k':
		c = false;
		brep.getSolid(0)->tx -= 0.2;
		brep.getSolid(0)->ty -= 0.2;
		brep.getSolid(0)->tz -= 0.2;
		break;
	case 'U':
	case 'u':
		c = false;
		brep.getSolid(0)->sx += 0.5;
		brep.getSolid(0)->sy += 0.5;
		brep.getSolid(0)->sz += 0.5;
		break;
	case 'D':
	case 'd':
		c = false;
		brep.getSolid(0)->sx -= 0.5;
		brep.getSolid(0)->sy -= 0.5;
		brep.getSolid(0)->sz -= 0.5;
		break;
	case 'Y':
	case 'y':
		brep.clear();
		c = true;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glLightfv(GL_LIGHT0, GL_POSITION, light);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}
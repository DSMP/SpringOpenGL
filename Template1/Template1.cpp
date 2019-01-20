// Template1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "include\GL\GL.H"
#include "include\GL\GLU.H"
#include "include\GL\glut.h"
#include "Template1.h"


GLfloat M_PI = 3.14f;
GLint mass = 10;
GLfloat k_coeff = 1000;
GLint previous_x = 0;
GLint previous_y = 0;
GLfloat period = (float)(sqrt(mass / k_coeff));
bool growing = true;
int maxDistance = 1;
double b = 0.6;

void hexagon(int a) {
	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	  // Top face (y = 1.0f)
	  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube
}
void coil(int a) {
	glPushMatrix();
	glPointSize(2.0f);
	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_POINTS);
	for (double t = 0; t < 8*M_PI; t+=0.2)
	{
		for (double u = 0; u < 2*M_PI; u+=0.5)
		{
			double x = cos(t)*(3+cos(u));
			double y = sin(t)*(3 + cos(u));
			double z = b*t + sin(u);

			glVertex3f(x, y, z);
		}
	}
	glEnd();
	glPopMatrix();
}
void MyDisplay(void) {
	// The new scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hexagon(1);
	coil(0.6);

	// The end of scene
	glFlush();//start processing buffered OpenGL routines
}
void MyInit(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);//select clearing (background) color
									 /* initialize viewing values */
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//=1
	
	gluPerspective(70.0, 1.777777777777778, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//=1
	gluLookAt(0, 0, -30, 10, 0, 0, -1, 0, 0);
}
void OnMotion(int x, int y)
{
	printf(" OnMotion prev: %d %d now: %d %d ", previous_x, previous_y, x, y);
	if (previous_x < x)
	{
		printf("previous_x < x");
		gluLookAt(/*eye*/-0.001, 0, 0,/*look at*/0, 0, -0.05,/*up*/0, 1, 0);
	}
	if (previous_x > x)
	{
		printf("previous_x > x");
		gluLookAt(/*eye*/0.001, 0, 0,/*look at*/0, 0, -0.05,/*up*/0, 1, 0);
	}
	if (previous_y < y)
	{
		printf("previous_y < y");
		gluLookAt(/*eye*/0, 0.001, 0,/*look at*/0, 0, -0.05,/*up*/0, 1, 0);
	}
	if (previous_y > y)
	{
		printf("previous_y > y");
		gluLookAt(/*eye*/-0, -0.001, 0,/*look at*/0, 0, -0.05,/*up*/0, 1, 0);
	}
	previous_x = x;
	previous_y = y;
}
void render(int a)
{
	hexagon(a);
	coil(a);
	glutPostRedisplay();
	Growing();
	glutTimerFunc(25, render, a);
}
void Growing()
{
	if (growing)
	{
		b+=0.01;
	}
	else
	{
		b -= 0.01;
	}
	if (b <= 0.3)
	{
		growing = true;
	}
	if (b >= maxDistance)
	{
		growing = false;
	}
}

int main(int argc, char** argv) { //<- for normal API
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);//single buffer and RGBA
	glutInitWindowSize(1100, 600);//initial window size
	glutInitWindowPosition(50, 50);
	glutCreateWindow("My window");//create widnow, hello title bar
	MyInit();
	glutDisplayFunc(MyDisplay);//
	glutMotionFunc(OnMotion);
	glutTimerFunc(100, render, 0.6);
	glutMainLoop();//enter main loop and process events
	return 0;
}
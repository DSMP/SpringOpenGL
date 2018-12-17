// Template1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "include\GL\GL.H"
#include "include\GL\GLU.H"
#include "include\GL\glut.h"


void triangle(int a) {
	glBegin(GL_POLYGON);// OpenGL’a state
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);//Red;
	glVertex3f(0, 0,-9.99);
	glColor4f(0.0f, 1.0f, 0.0f, 5.0f);//Green
	glVertex3f(5, 5, -9.99);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);//Blue
	glVertex3f(0, 5, -9.99);
	glEnd();
}
void MyDisplay(void) {
	// The new scene
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	triangle(1);

	// The end of scene
	glFlush();//start processing buffered OpenGL routines
}
void MyInit(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);//select clearing (background) color
									 /* initialize viewing values */
	glEnable(GL_DEPTH_TEST);
	//glViewport(0, 0, 300, 300);//window origin and size
	glMatrixMode(GL_PROJECTION);//
	glLoadIdentity();//=1
	//glOrtho(-300, 300, -300, 300, 0, 100);
	
	gluPerspective(100.0, 1.777777777777778, 1, 10);
	//gluPerspective(0.3, 1.777777777777778, 0.5, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//=1
	//gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

int main(int argc, char** argv) { //<- for normal API
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);//single buffer and RGBA
	glutInitWindowSize(600, 600);//initial window size
	glutInitWindowPosition(50, 50);
	glutCreateWindow("My window");//create widnow, hello title bar
	MyInit();
	glutDisplayFunc(MyDisplay);//
	glutMainLoop();//enter main loop and process events
	return 0;
}